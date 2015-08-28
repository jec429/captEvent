////////////////////////////////////////////////////////////
// $Id: TPulseHit.cxx,v 1.22 2012/06/18 15:32:24 mcgrew Exp $
//
#include <cmath>

#include <TGeoManager.h>
#include <TGeoBBox.h>
#include <TVector3.h>

#include "TGeomIdManager.hxx"
#include "TPulseHit.hxx"
#include "TManager.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TPulseHit);

CP::TPulseHit::TPulseHit() 
    : fGeomId(0), 
      fCharge(0), fChargeUncertainty(1*unit::coulomb),
      fTime(0), fTimeUncertainty(1*unit::second), fTimeRMS(1*unit::second),
      fTimeStart(0), fTimeStop(0),
      fInitialized(false),
      fPosition(0,0,0), 
      fUncertainty(100*unit::meter,100*unit::meter,100*unit::meter),
      fRMS(100*unit::meter,100*unit::meter,100*unit::meter) {
    SetBit(kCanDelete,false);
}

CP::TPulseHit::TPulseHit(const CP::TPulseHit& h) 
    : CP::THit(h), fGeomId(h.fGeomId),
      fCharge(h.fCharge), fChargeUncertainty(h.fChargeUncertainty),
      fTime(h.fTime), fTimeUncertainty(h.fTimeUncertainty),
      fTimeRMS(h.fTimeRMS),
      fTimeStart(h.fTimeStart), fTimeStop(h.fTimeStop),
      fTimeSamples(h.fTimeSamples),
      fInitialized(h.fInitialized),
      fPosition(h.fPosition),
      fUncertainty(h.fUncertainty),
      fRMS(h.fRMS) {
    SetBit(kCanDelete,false);
}

CP::TPulseHit::~TPulseHit() { }

//////////////////////////////////////////////////
// Getter methods for CP::TPulseHit
//////////////////////////////////////////////////

CP::TGeometryId CP::TPulseHit::GetGeomId(int i) const {
    if (i != 0) throw CP::EHitOutOfRange();
    return TGeometryId(fGeomId);
}

double CP::TPulseHit::GetCharge(void) const {return fCharge;}

double CP::TPulseHit::GetChargeUncertainty(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fChargeUncertainty;
}

double CP::TPulseHit::GetTime(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fTime;
}

double CP::TPulseHit::GetTimeRMS(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fTimeRMS;
}

double CP::TPulseHit::GetTimeStart(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fTimeStart;
}

double CP::TPulseHit::GetTimeStop(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fTimeStop;
}

int CP::TPulseHit::GetTimeSamples(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return (int) fTimeSamples.size();
}

double CP::TPulseHit::GetTimeSample(int i) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fTimeSamples.at(i);
}

const TVector3& CP::TPulseHit::GetPosition(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fPosition;
}

const TMatrixD& CP::TPulseHit::GetRotation(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fRotation;
}

const TVector3& CP::TPulseHit::GetRMS(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize(); 
    return fRMS;
}

const TVector3& CP::TPulseHit::GetUncertainty(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fUncertainty;
}

double CP::TPulseHit::GetTimeUncertainty(void) const {
    if (!fInitialized) const_cast<CP::TPulseHit*>(this)->Initialize();
    return fTimeUncertainty;
}

bool CP::TPulseHit::InitializeGeneric() {
    TGeoManager* geom = CP::TManager::Get().Geometry();
    geom->PushPath();
    if (!CP::TManager::Get().GeomId().CdId(TGeometryId(fGeomId))) {
        geom->PopPath();
        fPosition.SetXYZ(0,0,0);
        double v = 100*unit::meter;
        fUncertainty.SetXYZ(v,v,v);
        fRMS.SetXYZ(v,v,v);
        fRotation.ResizeTo(3,3);
        fRotation.Zero();
        fRotation(0,0) = 1;
        fRotation(1,1) = 1;
        fRotation(2,2) = 1;
        return false;
    }
    TGeoNode* node = geom->GetCurrentNode();

    // Find the global position
    double local[3] = {0,0,0};
    double master[3] = {0,0,0};
    geom->LocalToMaster(local,master);
    fPosition.SetXYZ(master[0],master[1],master[2]);
    
    // Find the size of the object.
    TGeoBBox *shape = dynamic_cast<TGeoBBox*>(node->GetVolume()->GetShape());
    fUncertainty.SetXYZ(shape->GetDX(), shape->GetDY(), shape->GetDZ());
    fUncertainty = fUncertainty*(2.0/std::sqrt(12.0));
    
    fRMS = fUncertainty;
    if (fRMS.X() < 1.5*unit::mm/std::sqrt(12.0)) {
        fRMS.SetX(1.5*unit::mm/std::sqrt(12.0));
    }
    if (fRMS.Y() < 1.5*unit::mm/std::sqrt(12.0)) {
        fRMS.SetY(1.5*unit::mm/std::sqrt(12.0));
    }
    if (fRMS.Z() < 1.5*unit::mm/std::sqrt(12.0)) {
        fRMS.SetZ(1.5*unit::mm/std::sqrt(12.0));
    }

    // Need to check if the TGeomManager current matrix is an active or
    // passive rotation.
    fRotation.ResizeTo(3,3);
    fRotation.SetMatrixArray(geom->GetCurrentMatrix()->GetRotationMatrix());

    geom->PopPath();
    
    return true;
}

void CP::TPulseHit::Initialize(void) {
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
        CaptSevere("TPulseHit Exception: " << e.what());
        return;
    }
    catch (...) {
        // Just don't crash!
        CaptSevere("TPulseHit Exception: unknown");
        return;
    }
}
