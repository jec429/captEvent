#include <string>
#include <iomanip>
#include <sstream>

#include "TChannelId.hxx"
#include "TTFBChannelId.hxx"
#include "TFGDChannelId.hxx"
#include "TTPCChannelId.hxx"
#include "TCaptLog.hxx"

ClassImp(CP::TChannelId);

namespace {
};

//----------------------------------------------------------------------
CP::TChannelId::TChannelId(UInt_t id): fChannelId(id) {}

//----------------------------------------------------------------------
CP::TChannelId::TChannelId(const TChannelId& src): fChannelId(src.fChannelId) {}

//----------------------------------------------------------------------
CP::TChannelId::~TChannelId() {}

//----------------------------------------------------------------------
unsigned int CP::TChannelId::GetField(int msb, int lsb) const {
    if (lsb>msb || lsb<0 || 31<msb){ 
        ND280Error("Requesting TChannelId Field with invalid bit range"
                    << " LSB: " << lsb << " MSB: " << msb);
        throw EChannelId();
    }
    unsigned int field = fChannelId >> lsb;
    unsigned int mask = (1<<(msb-lsb+1))-1;
    return (field & mask);
}

//----------------------------------------------------------------------
void CP::TChannelId::SetField(int val, int msb, int lsb) {
    // Make sure the bit range is valid.
    if (lsb>msb || lsb<0 || 31<msb) {
        ND280Error("Trying to set channel id field with invalid bit range"
                    << " MSB: " << msb
                    << " LSB: " << lsb);
        throw EChannelId();
    }
    // Make sure val is in range.
    int maxValue = (1 << (msb-lsb+1));
    if (val >= maxValue) {
        ND280Severe("Channel id value out of range " << val 
                    << " Bits in field " << msb-lsb+1 
                    << " Maximum value " << maxValue);
        return;
    }
    if (val < 0) {
        ND280Severe("Channel id value out of range " << val 
                   << " -- Negative values are not allowed");
        return;
    }
    // Build a mask for this field
    long mask = ((1<<(msb-lsb+1))-1) << lsb;
    // Clear the bit field.
    fChannelId &= ~mask;
    // Shift the value and set the field.
    fChannelId |= ((val << lsb)&mask);
}

//----------------------------------------------------------------------
void CP::TChannelId::SetGuardBit() {
    SetField(1,kGuardBitMSB, kGuardBitLSB);
}

//----------------------------------------------------------------------
const CP::TChannelId::SubDetId CP::TChannelId::GetSubDetector() const {
    return CP::TChannelId::SubDetId(GetField(kSubDetMSB,kSubDetLSB));
}

//----------------------------------------------------------------------
void CP::TChannelId::SetSubDetector(int det) {
    if ( det < 1 || kMaxDetector <= det) {
        ND280Severe("Setting invalid sub-detector value: " << det);
    }
    SetField(det,kSubDetMSB,kSubDetLSB);
}

//----------------------------------------------------------------------
const Bool_t CP::TChannelId::IsValid() const {
    if (!(fChannelId & kGuard_Mask)) {
        ND280Severe("Channel identifier has invalid guard bit");
        return false;
    }
    int det = GetSubDetector();
    if (det < 1 || kMaxDetector <= det) {
        ND280Severe("Channel identifier has invalid sub-detector: " << det);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------
const Bool_t CP::TChannelId::IsFGDChannel() const {
    return (kFGD == GetSubDetector());
}

//----------------------------------------------------------------------
const Bool_t CP::TChannelId::IsTFBChannel() const {
    UInt_t det = GetSubDetector();
    return((det == kP0D) 
           || (det == kECal)
#ifdef  USE_DEPRECATED_SubDetId
           || (det == kDSECal) || (det == kTECal) || (det == kPECal) 
#endif
           || (det == kSMRD) || (det == kINGRID));
}

//----------------------------------------------------------------------
const Bool_t CP::TChannelId::IsTPCChannel() const {
    return (kTPC == GetSubDetector());
}

//----------------------------------------------------------------------
std::string CP::TChannelId::SubDetAsString() const {
    std::string det = "Unset";
  
    switch (GetSubDetector()) {
    case kP0D:     det = "P0D";     break;
    case kTPC:     det = "TPC";     break;
    case kFGD:     det = "FGD";     break;
#ifdef  USE_DEPRECATED_SubDetId
    case kDSECal:  det = "DS-ECal"; break;
    case kTECal:   det = "T-ECal";  break;
    case kPECal:   det = "P0DECal"; break;
#else
    case kECal:    det = "ECal";    break;
#endif
    case kSMRD:    det = "SMRD";    break;
    case kINGRID:  det = "INGRID";  break;
    default:       det = "-------";     break; 
    }

    return det;
}

//----------------------------------------------------------------------
std::string CP::TChannelId::AsString() const {
    if (IsTFBChannel()) {
        TTFBChannelId tfb(*this);
        return tfb.AsString();
    }

    if (IsFGDChannel()) {
        TFGDChannelId fgd(*this);
        return fgd.AsString();
    }

    if (IsTPCChannel()) {
        TTPCChannelId tpc(*this);
        return tpc.AsString();
    }

    std::ostringstream buffer;
    buffer << std::setw(7) << SubDetAsString()
           << std::setw(8) << std::hex << std::setfill('0') 
           << "0x" << fChannelId;
    return buffer.str();
}
