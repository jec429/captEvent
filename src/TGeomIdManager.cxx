#include <memory>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <typeinfo>

#include <TSystem.h>
#include <TObject.h>
#include <TFile.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoMatrix.h>
#include <TVector3.h>
#include <TKey.h>
#include <TString.h>
#include <TPRegexp.h>
#include <TGeoPhysicalNode.h>

#include "TCaptLog.hxx"
#include "TGeometryId.hxx"
#include "CaptGeomId.hxx"
#include "CaptGeomIdDef.hxx"
#include "TGeomIdManager.hxx"
#include "TManager.hxx"
#include "TEvent.hxx"
#include "TGeomIdFinder.hxx"
#include "TCaptIdFinder.hxx"

CP::TGeomIdManager::TGeomIdManager() {
    ResetGeometry();
}

CP::TGeomIdManager::~TGeomIdManager() {}

bool CP::TGeomIdManager::CdId(TGeometryId id) const {
    GeomIdKey gid = MakeGeomIdKey(id);
    GeomIdMap::const_iterator pair = fGeomIdMap.find(gid);
    if (pair != fGeomIdMap.end()) return CdKey(pair->second);
    return false;
}

bool CP::TGeomIdManager::FindGeometryId(TGeometryId& id) const {
    // Save the current node.
    CP::TManager::Get().Geometry();
    gGeoManager->PushPath();

    bool success = true;
    do {
        // Get the key into the map for the current node.
        RootGeoKey rik = MakeRootGeoKey();
        if (gGeoManager->GetTopNode() == gGeoManager->GetCurrentNode()) {
            success = false;
            break;
        }
        // Find the key in the map.
        RootIdMap::const_iterator rim = fRootIdMap.find(rik);
        // Don't have a geometry id for this node.
        if (rim == fRootIdMap.end()) {
            gGeoManager->CdUp();
            continue;
        }
        id = MakeGeometryId(rim->second);
        break;
    } while (true);

    // Return the the original state.
    gGeoManager->PopPath();

    return success;
}

bool CP::TGeomIdManager::GetPosition(TGeometryId id, TVector3& position) const {
    CP::TManager::Get().Geometry();
    gGeoManager->PushPath();
    bool success = CdId(id);
    if (success) {
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        position.SetXYZ(master[0], master[1], master[2]);
        gGeoManager->PopPath();
    }
    else {
        position.SetXYZ(0,0,0);
    }
    return success;
}

bool CP::TGeomIdManager::GetGeometryId(double x, double y, double z, 
                                       TGeometryId& id) const {
    CP::TManager::Get().Geometry();
    gGeoManager->PushPath();
    gGeoManager->FindNode(x,y,z);
    bool success = FindGeometryId(id);
    gGeoManager->PopPath();
    return success;
}

void CP::TGeomIdManager::ResetGeometry() {
    fGeomIdMap.clear();
    fRootIdMap.clear();
    fGeomIdHashCode = TSHAHashValue();
    fGeomIdChangedHash = TSHAHashValue();
    fGeomIdAlignmentId = TAlignmentId();
    fGeomEventContext = TEventContext();
    SetGeoManager(gGeoManager);
    if (!gGeoManager) {
        CaptNamedDebug("Geometry","ResetGeometry with invalid gGeoManager");
        return;
    }

    BuildHashCode();
    if (!GetHash().Valid()) {
        CaptError("Geometry reset, but no valid hash is available");
        return;
    }

    // See if the geometry already has an alignment applied.
    GetAlignmentCode(fGeomIdAlignmentId);

    BuildGeomIdMap();

    // Lock the geometry into memory.
    gGeoManager->LockGeometry();
}
    
bool CP::TGeomIdManager::LoadGeometry(TFile& file, 
                                      const CP::TSHAHashValue& hc,
                                      const CP::TAlignmentId& align) {
    if (!file.IsOpen()) {
        CaptError("Geometry file not available");
        return false;
    }
    file.cd();

    std::ostringstream rexp;
    rexp << std::hex << std::nouppercase << std::setfill('0');
    rexp << "^CAPTAINGeometry-";
    if (hc(0) != 0) rexp << std::setw(8) << hc(0);
    else rexp << "[[:xdigit:]]{8}";

    if (hc(1) != 0) rexp << "-" << std::setw(8) << hc(1);
    else rexp << "-[[:xdigit:]]{8}";

    if (hc(2) != 0) rexp << "-" << std::setw(8) << hc(2);
    else rexp << "-[[:xdigit:]]{8}";

    if (hc(3) != 0) rexp << "-" << std::setw(8) << hc(3);
    else rexp << "-[[:xdigit:]]{8}";

    if (hc(4) != 0) rexp << "-" << std::setw(8) << hc(4);
    else rexp << "-[[:xdigit:]]{8}";
    TPRegexp geometryRegExp(rexp.str().c_str());

    rexp << ":";
    if (align(0) != 0) rexp << std::setw(8) << align(0);
    else rexp << "[[:xdigit:]]{8}";

    if (align(1) != 0) rexp << "-" << std::setw(8) << align(1);
    else rexp << "-[[:xdigit:]]{8}";

    if (align(2) != 0) rexp << "-" << std::setw(8) << align(2);
    else rexp << "-[[:xdigit:]]{8}";

    if (align(3) != 0) rexp << "-" << std::setw(8) << align(3);
    else rexp << "-[[:xdigit:]]{8}";

    if (align(4) != 0) rexp << "-" << std::setw(8) << align(4);
    else rexp << "-[[:xdigit:]]{8}";
    TPRegexp alignedRegExp(rexp.str().c_str());


    // Find the right key.
    TKey* defaultKey = NULL;
    TKey* geometryKey = NULL;
    TKey* alignedKey = NULL;
    TIter next(file.GetListOfKeys());
    TKey* key;
    while ((key = dynamic_cast<TKey*>(next()))) {
        if (!defaultKey && std::string(key->GetName()) == "CAPTAINGeometry") {
            defaultKey = key;
        }
        if (!geometryKey && TString(key->GetName()).Contains(geometryRegExp)) {
            geometryKey = key;
        }
        if (!alignedKey && TString(key->GetName()).Contains(alignedRegExp)) {
            alignedKey = key;
        }
    }
    
    key = defaultKey;
    if (geometryKey) key = geometryKey;
    if (alignedKey) key = alignedKey;

    if (!key) {
        return false;
    }

    // Unprotect the geometry so we can load a new one.
    if (gGeoManager) gGeoManager->UnlockGeometry();

    TGeoManager *saveGeom = gGeoManager;
    TGeoManager *geom=
        dynamic_cast<TGeoManager*>(file.Get(key->GetName()));
    if (!geom) {
        gGeoManager = saveGeom;
        return false;
    }

    std::string fileName(gSystem->BaseName(file.GetName()));
    CaptLog("Geometry read from " << fileName);

    // Check to see if the hash code should be taken from the file name.
    TSHAHashValue newCode;
    if (!GetHashCode(newCode)
        && fileName.find("geom-") != std::string::npos
        && fileName.find(".root") != std::string::npos
        && ParseHashCode(fileName.substr(
                             fileName.find("geom-")+5,100),newCode)) {
        // The hash code is saved in the file name so save it 
        SaveHashCode(newCode);
    }

    ResetGeometry();

    return true;
}

std::string
CP::TGeomIdManager::FindGeometryFile(const TSHAHashValue& hc) const {
    std::string result = "";

    std::string packageRoot(gSystem->Getenv("CAPTEVENTROOT"));
    std::string packageConfig(gSystem->Getenv("CAPTEVENTCONFIG"));
    std::string geometryName = packageRoot + "/" + packageConfig;
    void* dirp = gSystem->OpenDirectory(geometryName.c_str());
    if (!dirp) {
        CaptSevere("Geometry directory not available:"
                    "  Run captain-get-geometry.");
        return result;
    }

    // Build a regular expression to look for the file name.
    std::ostringstream rexp;
    rexp << std::hex << std::nouppercase << std::setfill('0');
    rexp << "geom-";
    if (hc(0) != 0) rexp << std::setw(8) << hc(0);
    else rexp << "[[:xdigit:]]{8}";

    if (hc(1) != 0) rexp << "-" << std::setw(8) << hc(1);
    else rexp << "-[[:xdigit:]]{8}";

    if (hc(2) != 0) rexp << "-" << std::setw(8) << hc(2);
    else rexp << "-[[:xdigit:]]{8}";

    if (hc(3) != 0) rexp << "-" << std::setw(8) << hc(3);
    else rexp << "-[[:xdigit:]]{8}";

    if (hc(4) != 0) rexp << "-" << std::setw(8) << hc(4);
    else rexp << "-[[:xdigit:]]{8}";

    rexp << ".root$";

    TPRegexp regExp(rexp.str().c_str());
    while (const char *fileName = gSystem->GetDirEntry(dirp)) {
        if (!TString(fileName).Contains(regExp)) continue;
        // This is the geometry we are looking for.
        result = geometryName + "/" + fileName;
        break;
    }
    gSystem->FreeDirectory(dirp);

    return result;
}

bool CP::TGeomIdManager::ReadGeometry(const CP::TSHAHashValue& hc) {
    std::string inputName = FindGeometryFile(hc);
    if (inputName.empty()) {
        CaptSevere("No geometry matchs hash: " << hc);
        return false;
    }
    TFile* inputPtr(TFile::Open(inputName.c_str(),"OLD"));
    if (!inputPtr) {
        CaptError("Cannot open geometry file: " << inputName);
        return false;
    }
    std::auto_ptr<TFile> inFile(inputPtr);
    return LoadGeometry(*inFile,hc);
}

void CP::TGeomIdManager::SaveHashCode(const TSHAHashValue& hc) {
    std::ostringstream hash;
    if (!hc.Valid()) {
        CaptSevere("Trying to save invalid hash code");
        return;
    }

    std::string name(gGeoManager->GetName());

    // Make sure we are starting from a standarized name.
    if (name.find("CAPTAINGeometry-")!=0) name = "CAPTAINGeometry-";

    hash << std::hex << std::nouppercase << std::setfill('0');
    hash << "-" << std::setw(8) << hc(0);
    hash << "-" << std::setw(8) << hc(1);
    hash << "-" << std::setw(8) << hc(2);
    hash << "-" << std::setw(8) << hc(3);
    hash << "-" << std::setw(8) << hc(4);

    std::string::size_type start = name.find("-");
    std::string::size_type length = std::min(name.length()-start,
                                             name.find(":")-start);
    name.replace(start, length, hash.str());

    gGeoManager->SetName(name.c_str());
}

bool CP::TGeomIdManager::GetHashCode(CP::TSHAHashValue& hc) const {
    hc = CP::TSHAHashValue();
    std::string name(gGeoManager->GetName());
    // Check that the name is "reasonable".
    if (name.find("CAPTAINGeometry-") != 0) {
        return false;
    }
    if (name.size() < 58) {
        CaptNamedDebug("Geometry","Name length wrong: " << name.size());
        return false;
    }
    if (!ParseHashCode(name.substr(14,256),hc)) {
        CaptNamedDebug("Geometry","Name not parsed: " << name.substr(14,256));
        return false;
    }
    return true;
}

void CP::TGeomIdManager::SaveAlignmentCode(const CP::TAlignmentId& aid) {
    std::ostringstream hash;
    std::string name(gGeoManager->GetName());

    // Make sure we are starting from a standarized name.
    if (name.find("CAPTAINGeometry-")!=0) {
        CaptSevere("Cannot save alignment id.  Invalid geometry name");
        return;
    }

    if (name.length()<58) {
        CaptSevere("Cannot save alignment id.  No hash code saved");
        return;
    }

    if (!aid.Valid()) {
        std::string::size_type start = name.find(":");
        if (start != std::string::npos) {
            name.erase(start);
            gGeoManager->SetName(name.c_str());
        }
        return;
    }

    hash << std::hex << std::nouppercase << std::setfill('0');
    hash << ":" << std::setw(8) << aid(0);
    hash << "-" << std::setw(8) << aid(1);
    hash << "-" << std::setw(8) << aid(2);
    hash << "-" << std::setw(8) << aid(3);
    hash << "-" << std::setw(8) << aid(4);

    std::string::size_type start = name.find(":");
    if (start == 58) {
        std::string::size_type length = name.length()-start;
        name.replace(start, length, hash.str());
    }
    else if (start == std::string::npos) {
        name = name + hash.str();
    }

    gGeoManager->SetName(name.c_str());
}

bool CP::TGeomIdManager::GetAlignmentCode(CP::TAlignmentId& aid) const {
    aid = CP::TAlignmentId();
    std::string name(gGeoManager->GetName());
    // Check that the name is "reasonable".
    if (name.find("CAPTAINGeometry-") != 0) {
        return false;
    }
    if (name.find(":") != 58) {
        return false;
    }
    if (name.size() < 102) {
        CaptNamedDebug("Geometry","Name length wrong: " << name.size());
        return false;
    }
    if (!ParseHashCode(name.substr(59,256),aid)) {
        CaptNamedDebug("Geometry","Name not parsed: " << name.substr(59,256));
        return false;
    }
    return true;
}

bool CP::TGeomIdManager::ParseHashCode(std::string name,
                                       CP::TSHAHashValue& hc) const {
    unsigned int htemp[5];

    std::string parseName = name;

    int dash = parseName.find('-');
    if (dash != 8) {
        CaptNamedDebug("Geometry",name);
        CaptNamedDebug("Geometry","Bad code -- '-' in wrong place: " << dash);
        return false;
    }
    std::istringstream hash0(parseName.substr(0,8));
    hash0 >> std::hex >> htemp[0];

    parseName = parseName.substr(dash+1,100);
    dash = parseName.find('-');
    if (dash != 8) {
        CaptNamedDebug("Geometry",name);
        CaptNamedDebug("Geometry","Bad code -- '-' in wrong place: " << dash);
        return false;
    }
    std::istringstream hash1(parseName.substr(0,8));
    hash1 >> std::hex >> htemp[1];

    parseName = parseName.substr(dash+1,100);
    dash = parseName.find('-');
    if (dash != 8) {
        CaptNamedDebug("Geometry",name);
        CaptNamedDebug("Geometry","Bad code -- '-' in wrong place: " << dash);
        return false;
    }
    std::istringstream hash2(parseName.substr(0,8));
    hash2 >> std::hex >> htemp[2];

    parseName = parseName.substr(dash+1,100);
    dash = parseName.find('-');
    if (dash != 8) {
        CaptNamedDebug("Geometry",name);
        CaptNamedDebug("Geometry","Bad code -- '-' in wrong place: " << dash);
        return false;
    }
    std::istringstream hash3(parseName.substr(0,8));
    hash3 >> std::hex >> htemp[3];

    parseName = parseName.substr(dash+1,100);
    if (parseName.size() < 8) {
        CaptNamedDebug("Geometry",name);
        CaptNamedDebug("Geometry","Bad code -- last value short: " 
                        << parseName.size());
        return false;
    }
    std::istringstream hash4(parseName);
    hash4 >> std::hex >> htemp[4];

    hc = CP::TSHAHashValue(htemp);

    return true;
}

bool CP::TGeomIdManager::CdKey(const RootGeoKey& key) const {
    // This is the only place that should need to be changed if the RootGeoKey
    // type is redefined.
    int nodeid = key;
    gGeoManager->CdNode(nodeid);
    return true;
}

CP::TGeomIdManager::RootGeoKey CP::TGeomIdManager::MakeRootGeoKey() const {
    // This knows how to translate the current TGeoManager node into a
    // RootGeoKey object.  The initial implementation is trivially simple.
    return gGeoManager->GetCurrentNodeId();
}

CP::TGeomIdManager::GeomIdKey 
CP::TGeomIdManager::MakeGeomIdKey(TGeometryId id) const {
    // This knows how to take TGeometryId object and translate it into a
    // GeomIdKey that can be used with the fGeomIdMap.  For some values of the
    // TGeometryId objects, this will have to apply a mask to the integer
    // value (since a root volume may be associated with more than one
    // TGeometryId values).
    GeomIdKey gik = id.AsInt();
    return gik;
}

CP::TGeometryId CP::TGeomIdManager::MakeGeometryId(GeomIdKey key) const {
    // This knows how to take a GeomIdKey and translate it into a
    // TGeometryId object.
    return TGeometryId(key);
}

void CP::TGeomIdManager::BuildGeomIdMap() {
    // DO NOT CALL TManager::Get().Geometry() HERE

    // Make sure that the node id array has been initialized in the
    // TGeoNodeCache.
    if (!gGeoManager->GetCache()->HasIdArray()) {
        gGeoManager->GetCache()->BuildIdArray();
    }

    // Set the top volume.
    TGeoVolume* top = gGeoManager->GetTopVolume();
    std::string topName(top->GetName());
#ifdef RESET_TOP_VOLUME
    if (topName != "t2k") {
        CaptWarn("Geometry top volume has changed to " << topName);
        top = gGeoManager->GetVolume("t2k");
        if (!top) {
            CaptError("No t2k volume in geometry");
            return;
        }
        CaptWarn("Resetting top volume to " << top->GetName());
        gGeoManager->SetTopVolume(top);
    }
#endif
        
    // Clear the current geom id map.
    fGeomIdMap.clear();
    fRootIdMap.clear();

    // Save the current geometry state.
    gGeoManager->PushPath();
    gGeoManager->CdTop();

    // Create the geometry identifier finders.
    if (!fFinders.empty()) {
        CaptSevere("fFinders vector should be empty");
        fFinders.clear();
    }
    fFinders.push_back(new TCaptIdFinder());

    // Initialize to begin recursion.
    std::vector<std::string> names; names.reserve(20);
    int keepGoing = 0;
    for (std::vector<TGeomIdFinder*>::iterator f = fFinders.begin();
         f != fFinders.end();
         ++f) keepGoing = 2*keepGoing + 1;

    RecurseGeomId(names,keepGoing);

    // Delete all of the geometry identifier finders.
    for (std::vector<TGeomIdFinder*>::iterator f = fFinders.begin();
         f != fFinders.end();
         ++f) {
        delete (*f);
    }
    fFinders.clear();

    // Restore the state.
    gGeoManager->PopPath();

    CaptLog("Geometry identifier map with " 
             << fGeomIdMap.size() << " entries.");

}

int CP::TGeomIdManager::RecurseGeomId(std::vector<std::string>& names,
                                          int keepGoing) {
    // DO NOT CALL TManager::Get().Geometry() HERE

    TGeoNode* node = gGeoManager->GetCurrentNode();
    names.push_back(node->GetName());

    // Call each of the finders that still want to see the geometry.
    int mask = 0;
    for (std::vector<TGeomIdFinder*>::iterator f = fFinders.begin();
         f != fFinders.end();
         ++f) {
        mask = (mask) ? mask << 1: 1;
        if (0 == (keepGoing&mask)) continue;
        // Search with a particular finder.  The finder will throw an
        // EGeomIdStop exception when it doesn't want to search any further.
        try {
            TGeometryId id;
            if ((*f)->Search(names, id)) {
                if (!id.IsValid()) {
                    CaptError("Invalid TGeometryId from " 
                               << typeid((*f)).name());
                    continue;
                }
                if (id.GetSubsystemName() == "node") {
                    CaptError("Plain node TGeometryId from " 
                               << typeid((*f)).name());
                    continue;
                }
                if (id.GetSubsystemName() == "unknown") {
                    CaptError("Unknown TGeometryId from " 
                               << typeid((*f)).name());
                    continue;
                }
                GeomIdKey gik = MakeGeomIdKey(id);
                RootGeoKey rgk = MakeRootGeoKey();
                GeomIdMap::iterator g = fGeomIdMap.find(gik);
                if (g != fGeomIdMap.end()) {
                    CaptError("Duplicate id: " << gik);
                    continue;
                }
                fGeomIdMap[gik] = rgk;
                fRootIdMap[rgk] = gik;
            }
        }
        catch (EGeomIdStop& e) {
            keepGoing &= ~mask;
        }
        catch (...) {
            CaptError("Unknown exception from finder");
        }
    }

    // If some of them want to go deeper in the tree, keep recursing.
    if (keepGoing) {
        // Recurse through all of the daughters.
        for (int i=0; i<node->GetNdaughters(); ++i) {
            gGeoManager->CdDown(i);
            RecurseGeomId(names, keepGoing);
        }
    }

    // Pop this node off the "stack"
    gGeoManager->CdUp();
    names.pop_back();

    return keepGoing;
}

void CP::TGeomIdManager::BuildHashCode() {
    // DO NOT CALL TManager::Get().Geometry() HERE

    // Make sure that the node id array has been initialized in the
    // TGeoNodeCache.
    if (!gGeoManager->GetCache()->HasIdArray()) {
        gGeoManager->GetCache()->BuildIdArray();
    }

    // Check to see if there is already a hash code.  If we have a hash code,
    // then don't recalculate.
    if (GetHashCode(fGeomIdHashCode)) return;

    CaptNamedDebug("Geometry", "Rebuild hash code");

    TGeoVolume* top = gGeoManager->GetTopVolume();
    std::string topName(top->GetName());
    CaptLog("Find Hash for " << topName);

#ifdef RESET_TOP_VOLUME
    // reset the top volume.
    if (topName != "t2k") {
        CaptWarn("Geometry top volume has changed to " << topName);
        top = gGeoManager->GetVolume("t2k");
        if (!top) {
            CaptError("No t2k volume in geometry");
            return;
        }
        CaptWarn("Resetting top volume to " << top->GetName());
        gGeoManager->SetTopVolume(top);
    }
#endif

    // Save the current geometry state.
    gGeoManager->PushPath();
    gGeoManager->CdTop();

    // Clear the message digest.
    fSHA1.Reset();

    // Initialize to begin recursion.
    std::vector<std::string> names; names.reserve(20);
    RecurseHashCode(names);

    // Get the message digest and save it in the geometry.
    unsigned int messageDigest[5];
    if (fSHA1.Result(messageDigest)) {
        fGeomIdHashCode = TSHAHashValue(messageDigest);
        CaptNamedDebug("Geometry","Built hash code: " << fGeomIdHashCode);
        SaveHashCode(fGeomIdHashCode);
    }
    else {
        CaptSevere("TGeomIdManager:: Could not build hash code");
        fGeomIdHashCode = TSHAHashValue();
        SaveHashCode(fGeomIdHashCode);
    }

    // Restore the state.
    gGeoManager->PopPath();
}

void CP::TGeomIdManager::RecurseHashCode(std::vector<std::string>& names) {
    // DO NOT CALL TManager::Get().Geometry() HERE

    TGeoNode* node = gGeoManager->GetCurrentNode();
    names.push_back(node->GetName());

    // Add this node to the SHA1 message digest.  The hash key is built out of
    // the name of the path, and the local position of the node in the parent
    // coordinate system.
    fSHA1 << gGeoManager->GetPath();
    const double*const translation = node->GetMatrix()->GetTranslation();
    fSHA1.Input(translation[0]);
    fSHA1.Input(translation[1]);
    fSHA1.Input(translation[2]);

    for (int i=0; i<node->GetNdaughters(); ++i) {
        gGeoManager->CdDown(i);
        RecurseHashCode(names);
    }

    // Pop this node off the "stack"
    gGeoManager->CdUp();
    names.pop_back();
}

std::string CP::TGeomIdManager::GetPath(TGeometryId id) const {
    if (!gGeoManager) return "not-available";
    if (fGeomIdMap.empty()) return "not-available";
    if (id == TGeometryId()) return "empty";

    gGeoManager->PushPath();

    if (!CdId(id)) {
        gGeoManager->PopPath();
        return "invalid";
    }
    std::string result = gGeoManager->GetPath();
    gGeoManager->PopPath();
    return result;
}

void CP::TGeomIdManager::SetGeoManager(TGeoManager* g) {
    gGeoManager = fGeoManager = g;
}


TGeoManager* CP::TGeomIdManager::GetGeoManager() {
    gGeoManager = fGeoManager;
    return gGeoManager;
}

bool CP::TGeomIdManager::FindAndLoadGeometry(CP::TEvent* event) {
    // Check to see if the geometry has already been overloaded
    if (GetGeometryHashOverride().Equivalent(GetHash())) {
        CaptNamedDebug("Geometry","Correct geometry override already loaded");
        return false;
    }

    // Check to see if we have a hash code to override the default geometry.
    if (GetGeometryHashOverride().Valid()) {
        CaptNamedDebug("Geometry","Override standard geometry");
        if (GetGeometryHashOverride().Valid()) {
            if (ReadGeometry(GetGeometryHashOverride())) {
                CaptNamedInfo("Geometry","Override geometry hash " 
                          << GetGeometryHashOverride());
                SetGeometryHashOverride(GetHash());
                return true;
            }
        }
    }

    // Check to see if we have a file to override the default geometry.
    if (!GetGeometryFileOverride().empty()) {
        CaptNamedDebug("Geometry","Override standard geometry with a file");
        TFile *filePtr 
            = TFile::Open(GetGeometryFileOverride().c_str(),"OLD");
        if (!filePtr) {
            CaptNamedWarn("Geometry",
                          " Geometry override file does not exist.");
        }
        else {
            std::auto_ptr<TFile> file(filePtr);        
            if (LoadGeometry(*file,CP::TSHAHashValue())) {
                CaptNamedInfo("Geometry","Override geometry from "
                              << file->GetName());
                SetGeometryHashOverride(GetHash());
                return true;
            }
        }
    }

    // Check to see if there is a geometry in the current file.  This
    // overrides the default geometry.
    TSHAHashValue hc;
    TAlignmentId aid;
    if (event) {
        hc = event->GetGeometryHash();
        aid = event->GetAlignmentId();
    }
    TFile* currentInputFile = TManager::Get().CurrentInputFile();
    if (!currentInputFile) {
        CaptNamedWarn("Geometry",
                       " Input file not available to provide geometry");
    }
    else if (LoadGeometry(*currentInputFile,hc,aid)) {
        CaptNamedInfo("Geometry",
                       "Geometry loaded from " << currentInputFile->GetName());
        return true;
    }
    else if (hc.Valid()) {
        CaptNamedWarn("Geometry",
                       "Event needs geom w/ " << hc << ", but not in file");
    }
    
    // As a last resort, try to use the geometry that was specified as the
    // default.  While this is the last resort, this is the "normal" code
    // path. 
    hc = CP::TManager::Get().LookupGeometry(event);
    if (hc.Valid() && !GetHash().Equivalent(hc)) {
        CaptNamedInfo("Geometry","Look for geometry with " << hc);
        if (ReadGeometry(hc)) {
            CaptNamedInfo("Geometry","Geometry loaded with from database");
            return true;
        }
    }

    CaptNamedDebug("Geometry","Geometry not loaded (no valid method found)");
    return false;
}

namespace {
    // A local (and very simple) lock class to prevent GetGeometry from being
    // called recursively.
    class LocalGeometryLock {
    public:
        LocalGeometryLock() {++fLockCount;}
        ~LocalGeometryLock() {
            --fLockCount;
            if (fLockCount<0) fLockCount = 0;
        }
        bool IsLocked() {return fLockCount>1;}
        int LockCount() {return fLockCount;}
    private:
        static int fLockCount;
    };
    int LocalGeometryLock::fLockCount=0;
};

TGeoManager* CP::TGeomIdManager::GetGeometry(CP::TEvent* event) {
    LocalGeometryLock geomLock;
    if (geomLock.IsLocked()) {
        // The geometry is currently being accessed, so don't do any geometry
        // checks.
        CaptNamedWarn("Geometry","Recursive Geometry Access: Lock count is "
                       << geomLock.LockCount());
        return gGeoManager;
    }

    // Make sure that gGeoManager points to the current value of fGeoManager.
    // This is a bit redundant since it will be done (again) by
    // CheckGeometry(), but it doesn't hurt to be careful.
    GetGeoManager();

    // Save the current gFile so we can return to the current state.
    TFile* oldFile = gFile;
    std::string oldDirectory;
    if (oldFile) oldDirectory = oldFile->GetPath();
    
    // Check to see if the geometry might have changed.  If it might have
    // changed, then reload the geometry.
    bool geometryChanging = false;
    if (CheckGeometry(event)) {
        CaptNamedDebug("Geometry","Check for the correct geometry");
        if (FindAndLoadGeometry(event)) {
            geometryChanging = true;
            CaptNamedDebug("Geometry", "Loaded geometry w/ hash: "
                            << GetHash());
        }
    }
    
    if (event) {
        if (event->GetGeometryHash().Valid()
            && !event->GetGeometryHash().Equivalent(GetHash())) {
            CaptNamedWarn("Geometry","Event geometry has been changed");
            CaptNamedDebug("Geometry","    Old hash: " 
                            << event->GetGeometryHash());
            CaptNamedDebug("Geometry","    New hash: " 
                            << GetHash());
        }
        event->SetGeometryHash(GetHash());
    }

    // Check to see if the alignment might have changed.
    if (CheckAlignment(event)) {
        geometryChanging = true;
        ApplyAlignment(event);
    }

    if (event) {
        if (event->GetAlignmentId().Valid()
            && !event->GetAlignmentId().Equivalent(GetAlignmentId())) {
            CaptNamedWarn("Geometry","Event alignment has changed");
            CaptNamedDebug("Geometry","    Old hash: " 
                            << event->GetAlignmentId());
            CaptNamedDebug("Geometry","    New hash: " 
                            << GetAlignmentId());
        }
        event->SetAlignmentId(GetAlignmentId());
    }
    
    // Make sure that gFile hasn't changed during this call.
    if (oldFile && oldFile != gFile) oldFile->cd(oldDirectory.c_str());

    // Make sure we have a valid geometry and return an exception if we do
    // not.
    if (!GetGeoManager()) {
        CaptError("No Geometry is available.");
        throw ENoGeometry();
    }

    // Notify the users that the geometry changed or doesn't match the last
    // "changed hash".
    if (geometryChanging || !fGeomIdChangedHash.Equivalent(GetHash())) {
        fGeomIdChangedHash = GetHash();
        CP::TManager::Get().ApplyGeometryCallbacks(event);
        CaptLog("Loaded " << GetGeoManager()->GetName());
    }

    // Stash the current time stamp.
    if (event) SetGeomEventContext(event->GetContext());
    
    return GetGeoManager();
}

bool CP::TGeomIdManager::CheckGeometry(const CP::TEvent* const event) {
    // If we don't have any geometry manager, then we have to load one.
    if (!GetGeoManager()) {
        CaptNamedInfo("Geometry","Load Geometry -- Not currently loaded");
        return true;
    }

    // Check to see if the current geometry hash code is valid.
    if (!GetHash().Valid()) {
        CaptNamedInfo("Geometry","Load Geometry -- Current Hash Invalid");
        return true;
    }

    // Check to see if the geometry has been overriden, and already matchs the
    // override geometry.
    if (GetGeometryHashOverride().Equivalent(GetHash())) return false;

    // There is an override geometry, so load the requested one.
    if (GetGeometryHashOverride().Valid()) return true;

    // There is an override file, so load that file.
    if (!GetGeometryFileOverride().empty()) return true;
        
    // If we don't have an event so don't change the geometry.
    if (!event) return false;

    // If the new event requests the same geometry, don't load it again.
    if (event->GetGeometryHash().Equivalent(GetHash())) return false;
    
    // If the event doesn't have a valid geometry hash, then only check the
    // geometry if the run number as changed.  First make sure that the
    // geometry manager has seen a valid event, and then don't check the
    // geometry if the run numbers match.
    if (!event->GetGeometryHash().Valid()
        && GetGeomEventContext().GetRun() != CP::TEventContext::Invalid
        && GetGeomEventContext().GetRun() == event->GetContext().GetRun()) {
        return false; 
    }

    return true;
}

bool CP::TGeomIdManager::CheckAlignment(const CP::TEvent* const event) {
    // We don't have an alignment yet so we need to load one.
    if (!GetAlignmentId().Valid()) return true;

    // Don't change the alignment if there isn't an event and we already have
    // a valid alignment.
    if (!event) return false;

    // Don't change the alignment if the event already uses the current
    // alignment.  This can be overridden by reseting the event alignment id.
    if (event->GetAlignmentId().Equivalent(GetAlignmentId())) return false;

    // The event doesn't use the current alignment.  This means it either
    // doesn't have an alignment, or was calibrated using a different
    // alignment.  Use the database code to see if the alignment needs to be
    // checked.  If this returns false we will keep the current alignment.  If
    // it returns true the geometry will be realigned.
    return CP::TManager::Get().CheckAlignment(event);
}

void CP::TGeomIdManager::ApplyAlignment(const CP::TEvent* const event) {
    GetGeoManager();
    if (!gGeoManager) {
        CaptSevere("ApplyAlignment called with invalid geometry");
        return;
    }

    if (!GetHash().Valid()) {
        CaptError("ApplyAlignment called with invalid geometry tables");
        return;
    }

    CaptNamedDebug("Geometry","Apply alignment to event");

    CP::TAlignmentId id;

    TObjArray* physicalNodes = gGeoManager->GetListOfPhysicalNodes();
    if (physicalNodes) {
        CaptNamedInfo("Geometry","Clear existing physical nodes: " 
                  << physicalNodes->GetEntries());
        gGeoManager->ClearPhysicalNodes(true);
    }

    if (CP::TManager::Get().HaveAlignment()) {
        // Save the current geometry state.
        CaptInfo("Apply alignment to event");
        gGeoManager->PushPath();
        
        // Let alignment code know that the alignment is starting.
        id = CP::TManager::Get().StartAlignment(event);

        if (!id.Valid()) CaptNamedInfo("Geometry",
                                        "No alignment should be apply");

        gGeoManager->UnlockGeometry();
        int alignmentCount = 0;
        // Apply alignment to each geometry element.
        std::pair<TGeometryId, TGeoMatrix*> 
            alignPair = CP::TManager::Get().Alignment(event);
        while (alignPair.second) {
            TGeometryId geomId = alignPair.first;
            std::auto_ptr<TGeoMatrix> align(alignPair.second);
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
            alignPair = CP::TManager::Get().Alignment(event);
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

    fGeomIdAlignmentId = id;

    SaveAlignmentCode(fGeomIdAlignmentId);
}

