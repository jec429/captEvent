#include <string>
#include <iomanip>
#include <sstream>

#include "TChannelId.hxx"
#include "TMCChannelId.hxx"

#include "TCaptLog.hxx"

ClassImp(CP::TChannelId);

CP::TChannelId::TChannelId(UInt_t id): fChannelId(id) {}

CP::TChannelId::TChannelId(const TChannelId& src): fChannelId(src.fChannelId) {}

CP::TChannelId::~TChannelId() {}

std::ostream& CP::operator<<(std::ostream& s, const CP::TChannelId& id) {
    try {
        s << id.AsString();
    }
    catch (...) {
        s << id.AsInt();
    };

    return s;
}

unsigned int CP::TChannelId::GetField(int msb, int lsb) const {
    if (lsb>msb || lsb<0 || 31<msb){ 
        CaptSevere("Requesting TChannelId Field with invalid bit range"
                    << " LSB: " << lsb << " MSB: " << msb);
        throw EChannelId();
    }
    unsigned int field = fChannelId >> lsb;
    unsigned int mask = (1<<(msb-lsb+1))-1;
    return (field & mask);
}

void CP::TChannelId::SetField(int val, int msb, int lsb) {
    // Make sure the bit range is valid.
    if (lsb>msb || lsb<0 || 31<msb) {
        CaptSevere("Trying to set channel id field with invalid bit range"
                   << " MSB: " << msb
                   << " LSB: " << lsb);
        throw EChannelId();
    }
    // Make sure val is in range.
    int maxValue = (1 << (msb-lsb+1));
    if (val >= maxValue) {
        CaptSevere("Channel id value out of range " << val 
                   << " Bits in field " << msb-lsb+1 
                   << " Maximum value " << maxValue);
        return;
    }
    if (val < 0) {
        CaptSevere("Channel id value out of range " << val 
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

void CP::TChannelId::SetGuardBit() {
    SetField(1,kGuardBitMSB, kGuardBitLSB);
}

const CP::TChannelId::SubDetId CP::TChannelId::GetSubDetector() const {
    return CP::TChannelId::SubDetId(GetField(kSubDetMSB,kSubDetLSB));
}

void CP::TChannelId::SetSubDetector(int det) {
    if ( det < 1 || kMaxDetector <= det) {
        CaptSevere("Setting invalid sub-detector value: " << det);
    }
    SetField(det,kSubDetMSB,kSubDetLSB);
}

const Bool_t CP::TChannelId::IsValid() const {
    if (!(fChannelId & kGuard_Mask)) {
        CaptSevere("Channel identifier has invalid guard bit");
        return false;
    }
    int det = GetSubDetector();
    if (det < 1 || kMaxDetector <= det) {
        CaptSevere("Channel identifier has invalid sub-detector: " << det);
        return false;
    }
    return true;
}

std::string CP::TChannelId::SubDetAsString() const {
    std::string det = "Unset";
  
    switch (GetSubDetector()) {
    case kMC:      det = "MC";     break;
    default:       det = "NV";     break; 
    }

    return det;
}

const Bool_t CP::TChannelId::IsMCChannel() const {
    return (kMC == GetSubDetector());
}

std::string CP::TChannelId::AsString() const {

    if (IsMCChannel()) {
        TMCChannelId id(*this);
        return id.AsString();
    }

    std::ostringstream buffer;
    buffer << std::setw(7) << SubDetAsString()
           << "-0x" << std::setw(8) << std::hex << std::setfill('0') 
           << fChannelId;
    return buffer.str();
}
