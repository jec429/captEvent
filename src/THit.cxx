////////////////////////////////////////////////////////////
// $Id: THit.cxx,v 1.24 2012/07/11 16:31:20 mcgrew Exp $
//
#include <TClass.h>
#include <TStreamerInfo.h>

#include <TGeoManager.h>

#include "HEPUnits.hxx"
#include "THit.hxx"
#include "TDigitProxy.hxx"
#include "TOADatabase.hxx"
#include "TGeomIdManager.hxx"
#include "TCaptLog.hxx"

ClassImp(CP::THit);

CP::THit::THit() {
    SetBit(kCanDelete,false);
}

CP::THit::~THit() {}

// Get the geometry identifier for this hit.
CP::TGeometryId CP::THit::GetGeomId(void) const {throw;}

#ifdef USE_DEPRECATED_GetGeoNodeId
int CP::THit::GetGeoNodeId(void) const {
    ND280Error("Deprecated method being used");
    CP::TOADatabase::Get().Geometry();
    gGeoManager->PushPath();
    TOADatabase::Get().GeomId().CdId(GetGeomId());
    int node = gGeoManager->GetCurrentNodeId();
    gGeoManager->PopPath();
    return node;
}
#endif

// The center of the volume associated with this hit.
const TVector3& CP::THit::GetPosition(void) const {throw;}

// Return the calibrated "charge" for the hit.
double CP::THit::GetCharge(void) const {throw;}

bool CP::THit::HasValidCharge() const {return !TestBit(kInvalidCharge);}

void CP::THit::SetChargeValidity(bool valid) {
    SetBit(kInvalidCharge, !valid);
}

// Return the calibrated "time" for the hit.
double CP::THit::GetTime(void) const {throw;}

bool CP::THit::HasValidTime() const {return !TestBit(kInvalidTime);}

void CP::THit::SetTimeValidity(bool valid) {
    SetBit(kInvalidTime, !valid);
}

// Return the position spread.
const TVector3& CP::THit::GetSpread(void) const {throw;}

// Return the "uncertainty" of the hit position.  This is the position
// resolution of the hit.
const TVector3& CP::THit::GetUncertainty(void) const {throw;}

// Return the "uncertainty" for the time measurement.  This is the timing
// resolution of the sensor.
double CP::THit::GetTimeUncertainty(void) const {return 1.0*unit::ns;}

// Has X information.
bool CP::THit::IsXHit(void) const {throw;}

// Has Y information.
bool CP::THit::IsYHit(void) const {throw;}

// Has Z information.
bool CP::THit::IsZHit(void) const {throw;}

// Return a contributing hit.
CP::THandle<CP::THit> CP::THit::GetContributor(int i) const {
    throw CP::EHitOutOfRange();
}

// Return the number of hits that contribute to this hit.
int CP::THit::GetContributorCount() const {return 0;}

// Return the digit that generated this hit. 
const CP::TDigitProxy& CP::THit::GetDigit(int i) const {
    throw CP::EHitOutOfRange();
}

// Return the number of digits that made this hit.
int CP::THit::GetDigitCount() const {return 0;}

// Return the associated channel id.
CP::TChannelId CP::THit::GetChannelId(int i) const {
    throw CP::EHitOutOfRange();
}

// Return the number of channel ids associated with this hit.
int CP::THit::GetChannelIdCount() const {return 0;}

// Print the hit information.
void CP::THit::ls(Option_t *opt) const {
    CP::ls_header(this,opt);
    int prec = std::cout.precision();
    std::cout << " Id: " << GetGeomId().AsInt();
    std::cout << " P: " << std::setprecision(0)
              << "(" << GetPosition().X()/unit::cm << " cm"
              << ", " << GetPosition().Y()/unit::cm << " cm"
              << ", " << GetPosition().Z()/unit::cm << " cm"
              << ", " << GetTime()/unit::ns << " ns)";
    if (IsXHit()) std::cout << "X";
    if (IsYHit()) std::cout << "Y";
    if (IsZHit()) std::cout << "Z";
    std::cout << " Q: " << std::setprecision(1) << GetCharge() << " pe"
              << std::endl;
    std::string option(opt);
    if (option.find("dump") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        TROOT::IndentLevel();
        std::cout << "Spread:      " 
                  << std::setprecision(0)
                  << "(" << GetSpread().X()/unit::cm << " cm"
                  << ", " << GetSpread().Y()/unit::cm << " cm"
                  << ", " << GetSpread().Z()/unit::cm << " cm)"
                  << std::endl;
        TROOT::IndentLevel();
        std::cout << "Uncertainty: " 
                  << std::setprecision(2)
                  << "(" << GetUncertainty().X()/unit::cm << " cm"
                  << ", " << GetUncertainty().Y()/unit::cm << " cm"
                  << ", " << GetUncertainty().Z()/unit::cm << " cm)"
                  << std::endl;
        TROOT::DecreaseDirLevel();
    }
    std::cout.precision(prec);
}
