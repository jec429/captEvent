//////////////////////////////////////////////////////////////
// $Id: TManager.cxx,v 1.53 2012/03/07 02:09:48 mcgrew Exp $
//
// Implement the class CP::TManager which provides the user interface
// to the calibrations and geometry databases.  This class is a
// singleton.
//

#include <ctime>
#include <memory>
#include <map>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>

#include <TSystem.h>
#include <TTimeStamp.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TKey.h>
#include <TTree.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TGeoPhysicalNode.h>

#include "TEvent.hxx"
#include "TManager.hxx"
#include "TEventFolder.hxx"
#include "TDigitManager.hxx"
#include "TGeomIdManager.hxx"
#include "TSHA1.hxx"

ClassImp(CP::TManager);

/// The static member pointer to the singleton.
CP::TManager* CP::TManager::fManager = NULL;

namespace {
    typedef std::pair < std::time_t , CP::TSHAHashValue> HashValue;
    typedef std::vector< HashValue > Hash;

    bool hashCompare(const HashValue& lhs, const HashValue& rhs) {
        return lhs.first < rhs.first;
    }

    /// Implement a default GeometryLookup class.  This is used unless another
    /// one is explicitly registered.
    class DefaultGeometryLookup: public CP::TManager::GeometryLookup {
        Hash fHashs;
    public:
        virtual ~DefaultGeometryLookup() {}
        // Open the context file and read the list of geometries.
        DefaultGeometryLookup() {
            // Generate the name of the file.
            std::string geometryName(gSystem->Getenv("OAEVENTROOT"));
            geometryName += "/";
            geometryName += gSystem->Getenv("OAEVENTCONFIG");
            geometryName += "/GEOMETRY.LIST" ;
            // Attache the file to a stream.
            std::ifstream geoContext(geometryName.c_str());
            std::string line;
            while (std::getline(geoContext,line)) {
                std::size_t comment = line.find("#");
                std::string tmp = line;
                if (comment != std::string::npos) tmp.erase(comment);
                // The minimum valid date, time and hash is 22 characters.  
                if (tmp.size()<20) continue;
                std::istringstream parser(tmp);
                int yr, mn, da, hr, m; 
                unsigned int hash0 = 0;
                unsigned int hash1 = 0;
                unsigned int hash2 = 0;
                unsigned int hash3 = 0;
                unsigned int hash4 = 0;
                char c;
                parser >> yr >> c >> mn >> c >> da;
                if (parser.fail()) {
                    CaptError("Could not parse date: "<< line);
                    continue;
                }
                parser >> hr >> c >> m;
                if (parser.fail()) {
                    CaptError("Could not parse time: "<< line);
                    continue;
                }
                parser >> std::hex >> hash0;
                if (parser.fail()) {
                    CaptError("Could not parse hash: "<< line);
                    continue;
                }
                parser >> c >> std::hex >> hash1;
                if (c != '-' || parser.fail()) {
                    CaptError("Could not parse hash: "<< line);
                    continue;
                }
                parser >> c >> std::hex >> hash2;
                if (c != '-' || parser.fail()) {
                    CaptError("Could not parse hash: "<< line);
                    continue;
                }
                parser >> c >> std::hex >> hash3;
                if (c != '-' || parser.fail()) {
                    CaptError("Could not parse hash: "<< line);
                    continue;
                }
                parser >> c >> std::hex >> hash4;
                if (c != '-' || parser.fail()) {
                    CaptError("Could not parse hash: "<< line);
                    continue;
                }
                yr -= 1900;
                TTimeStamp cTime(yr, mn, da, hr, m, 0 /*s*/, 0 /*ns*/,
                                 true /* Apply shift */,-9*60*60 /*JST*/);
                fHashs.push_back(
                    Hash::value_type(cTime.GetSec(),
                                     CP::TSHAHashValue(hash0,
                                                       hash1,
                                                       hash2,
                                                       hash3,
                                                       hash4)));
            }
            std::sort(fHashs.begin(),fHashs.end(),hashCompare);
            CaptVerbose("Available Geometries:");
            for (Hash::iterator h = fHashs.begin(); h != fHashs.end(); ++h) {
                CaptVerbose("  First valid date: " 
                             << TTimeStamp(h->first).AsString("c")
                             << " UTC -- Hash code: " 
                             << std::hex << std::setw(8) << std::showbase
                             << h->second);
            }
        }

        // Return the geometry matching the event context.
        virtual CP::TSHAHashValue GetHash(
            const CP::TEvent* const event) {
            if (fHashs.empty()) return CP::TSHAHashValue();
            // If no event so no default geometry.
            if (!event) return CP::TSHAHashValue();
            // Check to see if there is a hash code for this event.
            CP::TSHAHashValue hc = event->GetGeometryHash();
            if (hc.Valid()) return hc;
            // Find the first context after the event context
            Hash::iterator h;
            for (h = fHashs.begin(); h != fHashs.end(); ++h) {
                if (h->first >= event->GetContext().GetTimeStamp()) break;
            }
            // Before the first context, so use the front.
            if (h == fHashs.begin()) return fHashs.front().second;
            // After  the last context, so use the last.
            else if (h == fHashs.end()) return fHashs.back().second;
            // Use the context before the current.
            return (--h)->second;
        }
    };
    // Create a private geometry lookup.
    DefaultGeometryLookup* gDefaultGeometryLookup = NULL;
}

/// The private constructor for the database
CP::TManager::TManager() {
    fCurrentInputFile = NULL;
    fParticleData = NULL;
    fDigits = NULL;
    fGeomId = NULL;
    fGeometryLookup = NULL;
    fAlignmentLookup = NULL;
}

CP::TManager& CP::TManager::Get(void) {
    if (!fManager) {
        CaptVerbose("Create a new CP::TManager object");
        fManager = new CP::TManager;
    }
    return *fManager;
}

TGeoManager* CP::TManager::Geometry(CP::TEvent* event) {
    // If an event wasn't provided, then try getting the current event.
    if (!event) event = CP::TEventFolder::GetCurrentEvent();
    TGeoManager* geom = GeomId().GetGeometry(event);
    if (!geom) throw CP::ENoGeometry();
    return geom;
}

void CP::TManager::SetCurrentInputFile(TFile* input) {
    fCurrentInputFile = input;
}

void CP::TManager::SetGeometryOverride(const std::string& geomFile) {
    GeomId().SetGeometryFileOverride("");
    GeomId().SetGeometryHashOverride(CP::TSHAHashValue());
    if (geomFile != "") GeomId().SetGeometryFileOverride(geomFile);

    
}

void CP::TManager::SetGeometryOverride(const CP::TSHAHashValue& hc) {
    GeomId().SetGeometryFileOverride("");
    GeomId().SetGeometryHashOverride(hc);
}

void CP::TManager::RegisterGeometryCallback(
    CP::TManager::GeometryChange* callback) {
    fGeometryCallbacks.insert(callback);
}

void CP::TManager::RemoveGeometryCallback(
    CP::TManager::GeometryChange* callback) {
    fGeometryCallbacks.erase(callback);
}

void CP::TManager::ClearGeometryCallbacks() {
    fGeometryCallbacks.clear();
}

void CP::TManager::ApplyGeometryCallbacks(const CP::TEvent* event) {
    for (std::set<CP::TManager::GeometryChange*>::iterator 
             c = fGeometryCallbacks.begin();
         c != fGeometryCallbacks.end();
         ++c) {
        if (!(*c)) continue;
        (*c)->Callback(event);
    }
}

CP::TManager::GeometryLookup* CP::TManager::RegisterGeometryLookup(
    CP::TManager::GeometryLookup* lookup) {
    CP::TManager::GeometryLookup* old = fGeometryLookup;
    if (!lookup) {
        if (!gDefaultGeometryLookup) {
            gDefaultGeometryLookup = new DefaultGeometryLookup;
        }
        fGeometryLookup = gDefaultGeometryLookup;
    }
    else fGeometryLookup = lookup;
    return old;
}

CP::TSHAHashValue CP::TManager::FindEventGeometry(
    const CP::TEvent *const event) const {
    TSHAHashValue hashValue;
    if (!fGeometryLookup) {
        if (!gDefaultGeometryLookup) {
            gDefaultGeometryLookup = new DefaultGeometryLookup;
        }
        const_cast<CP::TManager*>(this)->fGeometryLookup 
            = gDefaultGeometryLookup;
    }
    try {
        hashValue = fGeometryLookup->GetHash(event);
    }
    catch (...) {
        hashValue = TSHAHashValue();
    }
    return hashValue;
}

CP::TManager::AlignmentLookup* CP::TManager::RegisterAlignmentLookup(
    CP::TManager::AlignmentLookup* lookup) {
    CP::TManager::AlignmentLookup* old = fAlignmentLookup;
    fAlignmentLookup = lookup;
    return old;
}

void CP::TManager::AlignGeometry(const CP::TEvent* const event) {
    GeomId().ApplyAlignment(event);
}

CP::TAlignmentId CP::TManager::ApplyAlignmentLookup(
    const CP::TEvent* const event) {
    CP::TAlignmentId id;

    GeomId().GetGeoManager();
    TObjArray* physicalNodes = gGeoManager->GetListOfPhysicalNodes();
    if (physicalNodes) {
        CaptNamedInfo("Geometry","Clear existing physical nodes: " 
                  << physicalNodes->GetEntries());
        gGeoManager->ClearPhysicalNodes(true);
    }

    if (fAlignmentLookup) {
        // Save the current geometry state.
        CaptInfo("Apply alignment to event");
        gGeoManager->PushPath();
        
        // Let alignment code know that the alignment is starting.
        id = fAlignmentLookup->StartAlignment(event);

        if (!id.Valid()) CaptNamedInfo("Geometry",
                                        "No alignment should be apply");

        gGeoManager->UnlockGeometry();
        int alignmentCount = 0;
        // Apply alignment to each geometry element.
        TGeometryId geomId;
        while (TGeoMatrix* a = fAlignmentLookup->Align(event,geomId)) {
            std::auto_ptr<TGeoMatrix> align(a);
            std::string path = geomId.GetName();
            TGeoPhysicalNode* pNode 
                = gGeoManager->MakePhysicalNode(path.c_str());
            TGeoMatrix* orig = pNode->GetNode()->GetMatrix();
            TGeoHMatrix h = (*orig) * (*align);
            if (pNode->IsAligned()) {
                pNode->Align(new TGeoHMatrix(h));
                delete orig;
            }
            else {
                pNode->Align(new TGeoHMatrix(h));
            }
            ++ alignmentCount;
        }

        if (alignmentCount > 0) {
            CaptInfo("Applied " << alignmentCount << " alignment matrices.");
        }

        gGeoManager->PopPath();
        gGeoManager->RefreshPhysicalNodes(true);
        
        if (id.Valid() && !alignmentCount) {
            CaptError("Invalid alignment applied to geometry");
            throw EBadAlignment();
        }
        
        if (!id.Valid() && alignmentCount) {
            CaptError("Invalid alignment applied to geometry");
            throw EBadAlignment();
        }

    }

    // There isn't an alignment id, so create one.  This is the hash of an
    // empty string.
    if (!id.Valid()) {
        CaptNamedDebug("Geometry","No alignment id, so create an empty one");
        TSHA1 sha;
        unsigned int message[5];
        if (sha.Result(message)) {
            id = TAlignmentId(message);
        }
        else {
            CaptError("SHA1 Calculation failed");
        }
    }

    return id;
}

bool CP::TManager::CheckAlignment(const CP::TEvent* const event) {
    if (!fAlignmentLookup) return false;
    return fAlignmentLookup->CheckAlignment(event);
}

TDatabasePDG& CP::TManager::ParticleData(void) {
    if (!fParticleData) {
        fParticleData = TDatabasePDG::Instance();
    }
    return *fParticleData;
}

CP::TPackageSet& CP::TManager::PackageSet(void) {
    return fPackageSet;
}

CP::TDigitManager& CP::TManager::Digits(void) {
    if (!fDigits) {
        fDigits = new CP::TDigitManager();
    }
    return *fDigits;
}

CP::TGeomIdManager& CP::TManager::GeomId(void) {
    if (!fGeomId) {
        fGeomId = new CP::TGeomIdManager();
    }
    return *fGeomId;
}
