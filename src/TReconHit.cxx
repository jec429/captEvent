#include <TGeoManager.h>

#include "TReconHit.hxx"
#include "TManager.hxx"
#include "TGeomIdManager.hxx"

ClassImp(CP::TReconHit);
ClassImp(CP::TWritableReconHit);

TMatrixD* CP::TReconHit::fRotation = NULL;

CP::TReconHit::TReconHit() 
    : fCharge(-9999), fChargeUncertainty(-9999),
      fTime(-9999), fTimeUncertainty(-9999), 
      fTimeRMS(-9999) {}

CP::TReconHit::TReconHit(const CP::TWritableReconHit& h)  
    : THit(h), fCharge(h.fCharge), fChargeUncertainty(h.fChargeUncertainty),
      fTime(h.fTime), fTimeUncertainty(h.fTimeUncertainty), 
      fTimeRMS(h.fTimeRMS), fPosition(h.fPosition),
      fUncertainty(h.fUncertainty), fRMS(h.fRMS),
      fConstituents(h.fConstituents) {}

CP::TReconHit::~TReconHit() { }

double CP::TReconHit::GetCharge(void) const {return fCharge;}

double CP::TReconHit::GetChargeUncertainty(void) const {
    return fChargeUncertainty;
}

double CP::TReconHit::GetTime(void) const {return fTime;}

double CP::TReconHit::GetTimeUncertainty(void) const {return fTimeUncertainty;}

double CP::TReconHit::GetTimeRMS(void) const {return fTimeRMS;}

const TVector3& CP::TReconHit::GetPosition(void) const {
    return fPosition;
}

const TMatrixD& CP::TReconHit::GetRotation(void) const {
    if (!fRotation) {
        fRotation = new TMatrixD(3,3);
        (*fRotation)(0,0) = 1.0;
        (*fRotation)(1,1) = 1.0;
        (*fRotation)(2,2) = 1.0;
    }

    return (*fRotation);
}

const TVector3& CP::TReconHit::GetRMS(void) const {
    return fRMS;
}

const TVector3& CP::TReconHit::GetUncertainty(void) const {
    return fUncertainty;
}

CP::TGeometryId CP::TReconHit::GetGeomId(int i) const {
    CP::THandle<CP::THit> hit = GetConstituent(i);
    return hit->GetGeomId();
}

int CP::TReconHit::GetGeomIdCount() const {
    return fConstituents.size();
}

CP::TChannelId CP::TReconHit::GetChannelId(int i) const {
    CP::THandle<CP::THit> hit = GetConstituent(i);
    return hit->GetChannelId();
}

int CP::TReconHit::GetChannelIdCount() const {
    return fConstituents.size();
}

const CP::TDigitProxy& CP::TReconHit::GetDigit(int i) const {
    CP::THandle<CP::THit> hit = GetConstituent(i);
    return hit->GetDigit();
}

int CP::TReconHit::GetDigitCount() const {
    return fConstituents.size();
}

CP::THandle <CP::THit> CP::TReconHit::GetConstituent(int i) const {
    if (i<0 || fConstituents.size()<= (unsigned) i) throw EHitOutOfRange();
    return fConstituents[i];
}

int CP::TReconHit::GetConstituentCount() const {
    return fConstituents.size();
}

// TWritableReconHits.
CP::TWritableReconHit::TWritableReconHit(const CP::TWritableReconHit& h) 
    : CP::TReconHit(h) {}

CP::TWritableReconHit::TWritableReconHit(CP::THandle<CP::THit> hit) {
    fConstituents.push_back(hit);
    fCharge = -9999.;
    fTime = -9999.;
    fTimeUncertainty = -9999;
}

CP::TWritableReconHit::TWritableReconHit(CP::THandle<CP::THit> hit1,
                                         CP::THandle<CP::THit> hit2) {
    fConstituents.push_back(hit1);
    fConstituents.push_back(hit2);
    fCharge = -9999.;
    fTime = -9999.;
    fTimeUncertainty = -9999;
}

void CP::TWritableReconHit::AddHit(CP::THandle<CP::THit> hit) {
    fConstituents.push_back(hit);
}

CP::TWritableReconHit::~TWritableReconHit() {}

//////////////////////////////////////////////////
// Setter methods for CP::TWritableReconHit
//////////////////////////////////////////////////

void CP::TWritableReconHit::SetCharge(double q) {fCharge = q;}

void CP::TWritableReconHit::SetChargeUncertainty(double q) {
    fChargeUncertainty = q;
}

void CP::TWritableReconHit::SetTime(double t) {fTime = t;}

void CP::TWritableReconHit::SetTimeUncertainty(double tunc) {
    fTimeUncertainty = tunc;
}

void CP::TWritableReconHit::SetTimeRMS(double tunc) {
    fTimeRMS = tunc;
}

void CP::TWritableReconHit::SetPosition(const TVector3& pos) {
    fPosition = pos;
}

void CP::TWritableReconHit::SetUncertainty(const TVector3& unc){
    fUncertainty = unc;
}

void CP::TWritableReconHit::SetRMS(const TVector3& unc){
    fRMS = unc;
}

void CP::TReconHit::ls(Option_t *opt) const {
    CP::THit::ls(opt);

    TROOT::IncreaseDirLevel();
    for (std::vector< CP::THandle < CP::THit > >::const_iterator h
             = fConstituents.begin();
         h != fConstituents.end();
         ++h) {
        h->ls(opt);
    }
    TROOT::DecreaseDirLevel();
}
