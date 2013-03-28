#include <TGeoManager.h>

#include "TReconHit.hxx"
#include "TOADatabase.hxx"
#include "TGeomIdManager.hxx"

ClassImp(ND::TReconHit);
ClassImp(ND::TWritableReconHit);

ND::TReconHit::TReconHit() {
    fInitialized = false;
}

ND::TReconHit::TReconHit(const ND::TWritableReconHit& h)  
    : fCharge(h.fCharge), fTime(h.fTime), fTimeUncertainty(h.fTimeUncertainty),
      fReconPosition(h.fReconPosition), fReconUncertainty(h.fReconUncertainty),
      fConstituents(h.fConstituents) {
    // This means that initalize will run the first time the hits are used.
    fInitialized = false;
}

ND::TReconHit::~TReconHit() { }

// Initialization method for TReconHits.
void ND::TReconHit::Initialize(){
    // Initalize the recon hit information from the consituants and stored
    // data.  All constituents must be from the same geometry object.  This is
    // checked in the constructor and setters.
    fInitialized = true;
    fGeomId = fConstituents.at(0)->GetGeomId();
    fIsXHit = fConstituents.at(0)->IsXHit();
    fIsYHit = fConstituents.at(0)->IsYHit();
    fIsZHit = fConstituents.at(0)->IsZHit();
    fSpread = fConstituents.at(0)->GetSpread();

    // To do positions we need the geometry.
    TGeoManager* geom = ND::TOADatabase::Get().Geometry();
    geom->PushPath();
    TOADatabase::Get().GeomId().CdId(fGeomId);
    double local[3] = {fReconPosition.X(),
                       fReconPosition.Y(),
                       fReconPosition.Z()};
    double master[3] = {0,0,0};
    geom->LocalToMaster(local,master);
    fPosition.SetXYZ(master[0],master[1],master[2]);
  
    /// \bug The uncertainty is stored as the 1 sigma offset, so we'll need to
    /// subtract the raw position in the end.  I think this should be
    /// converted to use LocalToMasterVect() so that the raw position is not
    /// handled explicitly.
    double masterunc[3] = {0,0,0};
    double localunc[3] = {
        fReconUncertainty.X(),
        fReconUncertainty.Y(),
        fReconUncertainty.Z()
    };
    geom->LocalToMaster(localunc,masterunc);
    TVector3 Offset(masterunc);
    // The uncertainty is stored as the 1 sigma offset, so we'll need to 
    // subtract the raw position.
    fUncertainty = Offset -  fConstituents.at(0)->GetPosition();
    geom->PopPath();

}

// Getter methods.
ND::TGeometryId ND::TReconHit::GetGeomId(void) const {
    if (!fInitialized) const_cast<ND::TReconHit*>(this)->Initialize();
    return fGeomId;
}

double ND::TReconHit::GetCharge(void) const {return fCharge;}

double ND::TReconHit::GetTime(void) const {return fTime;}

const TVector3& ND::TReconHit::GetPosition(void) const {
    if (!fInitialized) const_cast<ND::TReconHit*>(this)->Initialize();
    return fPosition;
}

const TVector3& ND::TReconHit::GetSpread(void) const {
    if (!fInitialized) const_cast<ND::TReconHit*>(this)->Initialize();
    return fSpread;
}

const TVector3& ND::TReconHit::GetUncertainty(void) const {
    if (!fInitialized) const_cast<ND::TReconHit*>(this)->Initialize();
    return fUncertainty;
}

double ND::TReconHit::GetTimeUncertainty(void) const {return fTimeUncertainty;}

bool ND::TReconHit::IsXHit(void) const {
    if (!fInitialized) const_cast<ND::TReconHit*>(this)->Initialize();
    return fIsXHit;
}
bool ND::TReconHit::IsYHit(void) const {
    if (!fInitialized) const_cast<ND::TReconHit*>(this)->Initialize();
    return fIsYHit;
}
bool ND::TReconHit::IsZHit(void) const {
    if (!fInitialized) const_cast<ND::TReconHit*>(this)->Initialize();
    return fIsZHit;
}

ND::THandle <ND::THit> ND::TReconHit::GetContributor(int i) const {
    if (i<0 || fConstituents.size()<= (unsigned) i) throw EHitOutOfRange();
    return fConstituents[i];
}

int ND::TReconHit::GetContributorCount() const {
    return fConstituents.size();
}

// TWritableReconHits.
ND::TWritableReconHit::TWritableReconHit(const ND::TWritableReconHit& h) 
    : ND::TReconHit(h) {}

ND::TWritableReconHit::TWritableReconHit(ND::THandle<ND::THit> hit) {
    fGeomId=hit->GetGeomId();
    fConstituents.push_back(hit);
    fCharge = -9999.;
    fTime = -9999.;
    fTimeUncertainty = -9999;
}

ND::TWritableReconHit::TWritableReconHit(ND::THandle<ND::THit> hit1,
                                         ND::THandle<ND::THit> hit2) {
    // Check that the hits are on the same bar
    if (hit1->GetGeomId() != hit2->GetGeomId()){
        ND280Error("TReconHits can only be created from hits"
                   " with the same geometrical ID");
        throw EReconHitIllegal();
    }  
    fGeomId=hit1->GetGeomId();
    fConstituents.push_back(hit1);
    fConstituents.push_back(hit2);
    fCharge = -9999.;
    fTime = -9999.;
    fTimeUncertainty = -9999;
}

void ND::TWritableReconHit::AddHit(ND::THandle<ND::THit> hit) {
    // Check that the hits are on the same bar
    if (fGeomId != hit->GetGeomId()){
        ND280Error("TReconHits can only be created from hits"
                   " with the same geometrical ID");
        throw EReconHitIllegal();
    }  
    fConstituents.push_back(hit);
}

ND::TWritableReconHit::~TWritableReconHit() {}

//////////////////////////////////////////////////
// Setter methods for ND::TWritableReconHit
//////////////////////////////////////////////////

void ND::TWritableReconHit::SetCharge(double q) {fCharge = q;}

void ND::TWritableReconHit::SetTime(double t) {fTime = t;}

void ND::TWritableReconHit::SetTimeUncertainty(double tunc) {
    fTimeUncertainty = tunc;
}

void ND::TWritableReconHit::SetPosition(TVector3& pos){
    // Need to store in local position.
    
    // To do positions we need the geometry.
    TGeoManager* geom = ND::TOADatabase::Get().Geometry();
    geom->PushPath();
    ND::TOADatabase::Get().GeomId().CdId(fGeomId);
    double local[3] = {0,0,0};
    double master[3] = {pos.X(),pos.Y(),pos.Z()};
    geom->MasterToLocal(master,local);
    fReconPosition = TVector3(local);
    geom->PopPath();
}

void ND::TWritableReconHit::SetUncertainty(TVector3& unc){
    // Save the 1 sigma offset in local.
  
    // To do positions we need the geometry.
    TGeoManager* geom = ND::TOADatabase::Get().Geometry();
    geom->PushPath();
    ND::TOADatabase::Get().GeomId().CdId(fGeomId);

    /// \bug Shouldn't the conversion of the global coordinates uncertainty to
    /// the the local coordinates uncertainty be done using
    /// MasterToLocalVect()?  I think the results will be equivalent, but the
    /// logic would be much clearer.
    TVector3 Offset = unc + fConstituents.at(0)->GetPosition();
    double local[3] = {0,0,0};
    double master[3] = {Offset.X(),Offset.Y(),Offset.Z()};
    geom->MasterToLocal(master,local);
    fReconUncertainty = TVector3(local);
    geom->PopPath();
}

void ND::TReconHit::ls(Option_t *opt) const {
    ND::THit::ls(opt);

    TROOT::IncreaseDirLevel();
    for (std::vector< ND::THandle < ND::THit > >::const_iterator h
             = fConstituents.begin();
         h != fConstituents.end();
         ++h) {
        h->ls(opt);
    }
    TROOT::DecreaseDirLevel();
}
