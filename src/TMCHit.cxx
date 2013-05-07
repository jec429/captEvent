////////////////////////////////////////////////////////////
// $Id: TMCHit.cxx,v 1.14 2010/04/07 19:07:59 mcgrew Exp $
//

#include "TMCHit.hxx"
#include "HEPUnits.hxx"
#include "TG4VHit.hxx"

ClassImp(CP::TMCHit);
ClassImp(CP::TWritableMCHit);

CP::TMCHit::TMCHit() {}

CP::TMCHit::TMCHit(const CP::TWritableMCHit& h) 
    : CP::TSingleHit(h), fTruth(h.fTruth) { }

CP::TMCHit::~TMCHit() { }

/// Print the hit information.
void CP::TMCHit::ls(Option_t *opt) const {
    CP::THit::ls(opt);
    std::string option(opt);
    if (option.find("dump") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        // Print the contributors.
        for (TruthContainer::const_iterator h = fTruth.begin();
             h != fTruth.end();
             ++h) {
	  if(*h) (*h)->ls(opt);
        }
        TROOT::DecreaseDirLevel();
    }
}

CP::TWritableMCHit::TWritableMCHit() {}
CP::TWritableMCHit::TWritableMCHit(const CP::TWritableMCHit& h) 
  : CP::TMCHit(h) {}
CP::TWritableMCHit::~TWritableMCHit() {}

//////////////////////////////////////////////////
// Setter methods for CP::TWritableMCHit
//////////////////////////////////////////////////

void CP::TWritableMCHit::SetGeomId(CP::TGeometryId id) {
    fGeomId = id.AsInt();
}

void CP::TWritableMCHit::SetCharge(double q) {fCharge = q;}

void CP::TWritableMCHit::SetTime(double t) {fTime = t;}

void CP::TWritableMCHit::SetChargeValidity(bool valid) {
    CP::THit::SetChargeValidity(valid);
}

void CP::TWritableMCHit::SetTimeValidity(bool valid) {
    CP::THit::SetTimeValidity(valid);
}
