
////////////////////////////////////////////////////////////
// $Id: THit.cxx,v 1.24 2012/07/11 16:31:20 mcgrew Exp $
//
#include "HEPUnits.hxx"
#include "TUnitsTable.hxx"
#include "THit.hxx"
#include "TDigitProxy.hxx"
#include "TManager.hxx"
#include "TGeomIdManager.hxx"
#include "TCaptLog.hxx"

#include <TClass.h>
#include <TStreamerInfo.h>
#include <TGeoManager.h>

ClassImp(CP::THit);

CP::THit::THit() {
    SetBit(kCanDelete,false);
}

CP::THit::~THit() {}

double CP::THit::GetCharge(void) const {throw CP::EHit();}

double CP::THit::GetChargeUncertainty() const {throw CP::EHit();}

bool CP::THit::HasValidCharge() const {return !TestBit(kInvalidCharge);}

void CP::THit::SetChargeValidity(bool valid) {SetBit(kInvalidCharge, !valid);}

double CP::THit::GetTime(void) const {throw CP::EHit();}

double CP::THit::GetTimeUncertainty(void) const {throw CP::EHit();}

double CP::THit::GetTimeRMS(void) const {throw CP::EHit();}

bool CP::THit::HasValidTime() const {return !TestBit(kInvalidTime);}

void CP::THit::SetTimeValidity(bool valid) {SetBit(kInvalidTime, !valid);}

const TVector3& CP::THit::GetPosition(void) const {throw CP::EHit();}

const TVector3& CP::THit::GetUncertainty(void) const {throw CP::EHit();}

const TVector3& CP::THit::GetRMS(void) const {throw CP::EHit();}

const TMatrixD& CP::THit::GetRotation(void) const {throw CP::EHit();}

TVector3 CP::THit::GetXAxis(void) const {
    const TMatrixD& rot = GetRotation();
    return TVector3(rot(0,0),rot(1,0), rot(2,0));
}

TVector3 CP::THit::GetYAxis(void) const {
    const TMatrixD& rot = GetRotation();
    return TVector3(rot(0,1),rot(1,1), rot(2,1));
}

TVector3 CP::THit::GetZAxis(void) const {
    const TMatrixD& rot = GetRotation();
    return TVector3(rot(0,2),rot(1,2), rot(2,2));
}

const TMatrixD& CP::THit::GetCovariance(void) const {
    // Check if the covariance is already initialized.
    if (fCovariance.GetNcols() == 3 && fCovariance.GetNrows() == 3) {
        return fCovariance;
    }

    // Fill the covariance in the local coordinate system.
    fCovariance.ResizeTo(3,3);
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            fCovariance(r,c) = 0.0;
        }
    }
    fCovariance(0,0) = GetUncertainty().X();
    fCovariance(1,1) = GetUncertainty().Y();
    fCovariance(2,2) = GetUncertainty().Z();
    
    // Rotate to the global coordinate system.  I'm doing this "brute force"
    // since I don't think we need much efficiency here, and it makes the code
    // easier to read.
    TMatrixD rot(GetRotation());
    TMatrixD rotInv(rot);
    rotInv.InvertFast();

    fCovariance = fCovariance*rotInv;
    fCovariance = rot*fCovariance;

    return fCovariance;
}

const TMatrixD& CP::THit::GetError(void) const {
    // Check if the error matrix is already initialized.
    if (fError.GetNcols() == 3 && fError.GetNrows() == 3) {
        return fError;
    }

    // Fill the error matrix with the covariance, and then invert.
    fError.ResizeTo(3,3);
    fError = GetCovariance();
    fError.InvertFast();

    return fError;
}

CP::THandle<CP::THit> CP::THit::GetConstituent(int i) const {
    throw CP::EHitOutOfRange();
}

int CP::THit::GetConstituentCount() const {return 0;}

const CP::TDigitProxy& CP::THit::GetDigit(int i) const {
    throw CP::EHitOutOfRange();
}

int CP::THit::GetDigitCount() const {return 0;}

CP::TChannelId CP::THit::GetChannelId(int i) const {throw CP::EHitOutOfRange();}

int CP::THit::GetChannelIdCount() const {return 0;}

CP::TGeometryId CP::THit::GetGeomId(int i) const {throw CP::EHitOutOfRange();}

int CP::THit::GetGeomIdCount() const {return 0;}

// Print the hit information.
void CP::THit::ls(Option_t *opt) const {
    CP::ls_header(this,opt);
    int prec = std::cout.precision();
    std::cout << " Geom Id: " << GetGeomId().AsInt();
    if (GetChannelIdCount() > 0) {
        std::cout << " C:";
        for (int i=0; i<GetChannelIdCount(); ++i) {
            std::cout << " " << GetChannelId(i);
        }
    }
    std::cout << std::endl;
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "T: " << unit::AsString(GetTime(),GetTimeUncertainty(),"time")
              << " Q: " << unit::AsString(GetCharge(), 
                                          GetChargeUncertainty(),
                                          "charge")
              << std::endl;
    TROOT::DecreaseDirLevel();
    if (GetGeomIdCount() > 1) {
        TROOT::IncreaseDirLevel();
        TROOT::IndentLevel();
        std::cout << "P: (" 
                  << CP::TUnitsTable::Get().ConvertLength(GetPosition().X())
                  << ", " 
                  << CP::TUnitsTable::Get().ConvertLength(GetPosition().Y())
                  << ", "
                  << CP::TUnitsTable::Get().ConvertLength(GetPosition().Z()) 
                  << ")";
        std::cout << std::endl;
        TROOT::DecreaseDirLevel();
    }
    std::string option(opt);
    if (option.find("hits") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        TROOT::IndentLevel();
        std::cout << "RMS          " 
                  << std::setprecision(0)
                  << "(" << CP::TUnitsTable::Get().ConvertLength(GetRMS().X())
                  << ", " << CP::TUnitsTable::Get().ConvertLength(GetRMS().Y())
                  << ", " << CP::TUnitsTable::Get().ConvertLength(GetRMS().Z())
                  << ", " << CP::TUnitsTable::Get().ConvertTime(GetTimeRMS())
                  << ")"
                  << std::endl;
        TROOT::IndentLevel();
        std::cout << "Uncertainty: " 
                  << std::setprecision(2)
                  << "("
                  << CP::TUnitsTable::Get().ConvertLength(GetUncertainty().X())
                  << ", "
                  << CP::TUnitsTable::Get().ConvertLength(GetUncertainty().Y())
                  << ", "
                  << CP::TUnitsTable::Get().ConvertLength(GetUncertainty().Z())
                  << ", "
                  << CP::TUnitsTable::Get().ConvertTime(GetTimeUncertainty())
                  << ")"
                  << std::endl;
        TROOT::IndentLevel();
        std::cout << "Start Time: "
                  << (int) GetTimeStart() << " ns" 
                  << " Stop Time: " 
                  << (int) GetTimeStop() << " ns" 
                  << std::endl;
        TROOT::DecreaseDirLevel();
    }
    std::cout.precision(prec);
    if (option.find("samples") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        TROOT::IncreaseDirLevel();
        TROOT::IndentLevel();
        std::cout << "Samples: ";
        for (int i=0; i<GetTimeSamples(); ++i) {
            if (i>1) std::cout << ", ";
            std::cout << CP::TUnitsTable::Get().ConvertCharge(GetTimeSample(i));
        }
        std::cout << std::endl;
        TROOT::DecreaseDirLevel();
        TROOT::DecreaseDirLevel();
    }
    if (GetDigitCount() > 0) {
        for (int i=0; i<GetDigitCount(); ++i) {
            CP::TDigitProxy digit = GetDigit(i);
            if (!digit.IsValid()) {
                TROOT::IndentLevel();
                std::cout << "Digit: not-valid";
                continue;
            }
            (*digit)->ls(opt);
        }
    }
}
