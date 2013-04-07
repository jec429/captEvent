////////////////////////////////////////////////////////////
// $Id: TMultiHit.cxx,v 1.10 2012/10/09 18:54:20 mcgrew Exp $
//
#include <cmath>
#include <algorithm>

#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TGeoNode.h>

#include "HEPUnits.hxx"
#include "TMultiHit.hxx"
#include "TManager.hxx"

ClassImp(CP::TMultiHit);


CP::TMultiHit::TMultiHit() 
    : fInitialized(false),
      fGeomId(0), fCharge(0), fTime(0), 
      fPosition(0,0,0), 
      fIsXHit(false), fIsYHit(false), fIsZHit(false),
      fSpread(100*unit::meter,100*unit::meter,100*unit::meter),
      fUncertainty(100*unit::meter,100*unit::meter,100*unit::meter),
      fTimeUncertainty(1*unit::ns) {
    SetBit(kCanDelete,false);
}

CP::TMultiHit::TMultiHit(const CP::TMultiHit& h) 
    : CP::THit(h), fHits(h.fHits),  fInitialized(false),
      fGeomId(h.fGeomId),
      fCharge(h.fCharge), fTime(h.fTime), 
      fPosition(h.fPosition),
      fIsXHit(h.fIsXHit), fIsYHit(h.fIsYHit), fIsZHit(h.fIsZHit),
      fSpread(h.fSpread),
      fUncertainty(h.fUncertainty),
      fTimeUncertainty(h.fTimeUncertainty) {
    SetBit(kCanDelete,false);
}

CP::TMultiHit::TMultiHit(iterator f, iterator l) 
    : fInitialized(false),
      fGeomId(0), fCharge(0), fTime(0), 
      fPosition(0,0,0), 
      fIsXHit(false), fIsYHit(false), fIsZHit(false),
      fSpread(100*unit::meter,100*unit::meter,100*unit::meter),
      fUncertainty(100*unit::meter,100*unit::meter,100*unit::meter) {
    SetBit(kCanDelete,false);
    fHits.clear();
    fHits.insert(fHits.end(),f,l);
    Initialize();
}

CP::TMultiHit::~TMultiHit() {}

//////////////////////////////////////////////////
// Getter methods for CP::TMultiHit
//////////////////////////////////////////////////

CP::TGeometryId CP::TMultiHit::GetGeomId(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    return fGeomId;
}

const TVector3& CP::TMultiHit::GetPosition(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (size()<1) throw EMultiHitIllegal();
    return fHits.front()->GetPosition();
}

double CP::TMultiHit::GetCharge(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    return fCharge;
}

double CP::TMultiHit::GetTime(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (size()<1) throw EMultiHitIllegal();
    return fHits.front()->GetTime();
}

const TVector3& CP::TMultiHit::GetSpread(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (size()<1) throw EMultiHitIllegal();
    return fHits.front()->GetSpread();
}

const TVector3& CP::TMultiHit::GetUncertainty(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (size()<1) throw EMultiHitIllegal();
    return fHits.front()->GetUncertainty();
}

double CP::TMultiHit::GetTimeUncertainty(void) const {
    return 1.0*unit::ns;
}

CP::TChannelId CP::TMultiHit::GetChannelId(int i) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (i<0 || (int) size()<=i) throw CP::EHitOutOfRange();
    return fHits[i]->GetChannelId();
}

int CP::TMultiHit::GetChannelIdCount() const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    return size();
}

bool CP::TMultiHit::IsXHit(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (size()<1) throw EMultiHitIllegal();
    return fHits.front()->IsXHit();
}

bool CP::TMultiHit::IsYHit(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (size()<1) throw EMultiHitIllegal();
    return fHits.front()->IsYHit();
}

bool CP::TMultiHit::IsZHit(void) const {
    if (!fInitialized) const_cast<CP::TMultiHit*>(this)->Initialize();
    if (size()<1) throw EMultiHitIllegal();
    return fHits.front()->IsZHit();
}

CP::TMultiHit::iterator CP::TMultiHit::begin() const {
    return fHits.begin();
}

CP::TMultiHit::iterator CP::TMultiHit::end() const {
    return fHits.end();
}

const CP::THandle<CP::TSingleHit>& CP::TMultiHit::operator [](int index) const {
    return fHits[index];
}

unsigned int CP::TMultiHit::size() const {
    return fHits.size();
}

namespace {
    bool fHitsCompare(const CP::THandle<CP::TSingleHit>& a, 
                      const CP::THandle<CP::TSingleHit>& b) {
        return (a->GetTime() < b->GetTime());
    }
}

void CP::TMultiHit::Initialize() {
    fInitialized = true;
    if (size()<1) throw EMultiHitIllegal();
    std::sort(fHits.begin(),fHits.end(), fHitsCompare);
    fGeomId = fHits.front()->GetGeomId();
    fCharge = 0.0;
    for (iterator h = begin(); h != end(); ++h) {
        if (fGeomId != (*h)->GetGeomId()) throw EMultiHitIllegal();
        fCharge += (*h)->GetCharge();
    }
}

void CP::TMultiHit::ls(Option_t *opt) const {
    CP::THit::ls(opt);
    std::string option(opt);
    if (option.find("dump") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        for (iterator v = begin();
             v != end(); 
             ++v) {
            (*v)->ls(opt);
        }
        TROOT::DecreaseDirLevel();
    }
}
