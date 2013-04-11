////////////////////////////////////////////////////////////
// $Id: TSingleHit.cxx,v 1.22 2012/06/18 15:32:24 mcgrew Exp $
//
#include <cmath>

#include <TGeoManager.h>
#include <TGeoBBox.h>
#include <TVector3.h>

#include "TGeomIdManager.hxx"
#include "TSingleHit.hxx"
#include "TManager.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TSingleHit);

CP::TSingleHit::TSingleHit() 
    : fGeomId(0), fCharge(0), fTime(0), fChannelId(0),
      fInitialized(false),
      fPosition(0,0,0), 
      fIsXHit(false), fIsYHit(false), fIsZHit(false),
      fSpread(100*unit::meter,100*unit::meter,100*unit::meter),
      fUncertainty(100*unit::meter,100*unit::meter,100*unit::meter),
      fTimeUncertainty(1*unit::ns) {
    SetBit(kCanDelete,false);
}

CP::TSingleHit::TSingleHit(const CP::TSingleHit& h) 
    : CP::THit(h), fGeomId(h.fGeomId),
      fCharge(h.fCharge), fTime(h.fTime), 
      fChannelId(h.fChannelId),
      fInitialized(h.fInitialized),
      fPosition(h.fPosition),
      fIsXHit(h.fIsXHit), fIsYHit(h.fIsYHit), fIsZHit(h.fIsZHit),
      fSpread(h.fSpread),
      fUncertainty(h.fUncertainty),
      fTimeUncertainty(h.fTimeUncertainty) {
    SetBit(kCanDelete,false);
}

CP::TSingleHit::~TSingleHit() { }

//////////////////////////////////////////////////
// Getter methods for CP::TSingleHit
//////////////////////////////////////////////////

CP::TGeometryId CP::TSingleHit::GetGeomId(void) const {
    return TGeometryId(fGeomId);
}

double CP::TSingleHit::GetCharge(void) const {return fCharge;}

double CP::TSingleHit::GetTime(void) const {return fTime;}

// Return the associated channel id.
CP::TChannelId CP::TSingleHit::GetChannelId(int i) const {return fChannelId;}

// Return the number of channel ids associated with this hit.
int CP::TSingleHit::GetChannelIdCount() const {
    if (!fChannelId.IsValid()) return 0;
    return 1;
}

const TVector3& CP::TSingleHit::GetPosition(void) const {
    if (!fInitialized) const_cast<CP::TSingleHit*>(this)->Initialize();
    return fPosition;
}

const TVector3& CP::TSingleHit::GetSpread(void) const {
    if (!fInitialized) const_cast<CP::TSingleHit*>(this)->Initialize(); 
    return fSpread;
}

const TVector3& CP::TSingleHit::GetUncertainty(void) const {
    if (!fInitialized) const_cast<CP::TSingleHit*>(this)->Initialize();
    return fUncertainty;
}

double CP::TSingleHit::GetTimeUncertainty(void) const {
    if (!fInitialized) const_cast<CP::TSingleHit*>(this)->Initialize();
    return fTimeUncertainty;
}

bool CP::TSingleHit::IsXHit(void) const {
    if (!fInitialized) const_cast<CP::TSingleHit*>(this)->Initialize();
    return fIsXHit;
}

bool CP::TSingleHit::IsYHit(void) const {
    if (!fInitialized) const_cast<CP::TSingleHit*>(this)->Initialize();
    return fIsYHit;
}

bool CP::TSingleHit::IsZHit(void) const {
    if (!fInitialized) const_cast<CP::TSingleHit*>(this)->Initialize();
    return fIsZHit;
}

bool CP::TSingleHit::InitializeGeneric() {
    TGeoManager* geom = CP::TManager::Get().Geometry();
    geom->PushPath();
    CP::TManager::Get().GeomId().CdId(GetGeomId());
    TGeoNode* node = geom->GetCurrentNode();

    // Find the global position
    double local[3] = {0,0,0};
    double master[3] = {0,0,0};
    geom->LocalToMaster(local,master);
    fPosition.SetXYZ(master[0],master[1],master[2]);
    
    // Find the spread.
    TGeoBBox *shape = dynamic_cast<TGeoBBox*>(node->GetVolume()->GetShape());
    double spread[3] = {0,0,0};
    double localX[3] = {shape->GetDX(),0,0};
    double tmp[3];
    geom->LocalToMasterVect(localX,tmp);
    for (int i=0; i<3; ++i) {
        spread[i] = std::max(spread[i],std::abs(tmp[i]));
    }
    double localY[3] = {0,shape->GetDY(),0};
    geom->LocalToMasterVect(localY,tmp);
    for (int i=0; i<3; ++i) {
        spread[i] = std::max(spread[i],std::abs(tmp[i]));
    }
    double localZ[3] = {0,0,shape->GetDZ()};
    geom->LocalToMasterVect(localZ,tmp);
    for (int i=0; i<3; ++i) {
        spread[i] = std::max(spread[i],std::abs(tmp[i]));
    }

    fSpread.SetXYZ(spread[0],spread[1],spread[2]);
    fUncertainty = fSpread*(2.0/std::sqrt(12.0*fCharge));
    if (HasValidTime()) {
        fTimeUncertainty = 2.5*unit::ns/std::sqrt(12.0);
    }
    else {
        fTimeUncertainty = 580*unit::ns/std::sqrt(12.0);
    }

    // Determine if the hit is an X, Y, or Z hit.
    fIsXHit = (fSpread.X() < 10*unit::cm);
    fIsYHit = (fSpread.Y() < 10*unit::cm);
    fIsZHit = (fSpread.Z() < 10*unit::cm);

    geom->PopPath();
    
    return true;
}

void CP::TSingleHit::Initialize(void) {
#ifdef TSINGLE_HIT_SAVE_POSITION
    TVector3 origPos = fPosition;
#endif
  
    try {
        do {
            // Try initializations looking for the first one to work.  The
            // initializations should be ordered from the most specific to the
            // most general.
            InitializeGeneric();
        } while (false);
        fInitialized = true;
    }
    catch (std::exception& e) {
        CaptSevere("TSingleHit Exception: " << e.what());
        return;
    }
    catch (...) {
        // Just don't crash!
        CaptSevere("TSingleHit Exception: unknown");
        return;
    }
  
#ifdef TSINGLE_HIT_SAVE_POSITION
    if (origPos.Mag()>1*unit::mm
        && (fPosition-origPos).Mag()>1*unit::mm) {
        CaptError("Position and node position mismatch"
                   << std::endl
                   << "  Geometry Id " << GetGeomId().AsInt() << std::endl
                   << "  Saved Position is " 
                   << " " << fPosition.X()
                   << " " << fPosition.Y()
                   << " " << fPosition.Z()
                   << std::endl
                   << "  Node Position is  " 
                   << " " << origPos.X()
                   << " " << origPos.Y()
                   << " " << origPos.Z());
    }
#endif
}
