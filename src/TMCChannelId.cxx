#include "TMCChannelId.hxx"

#include <string>
#include <sstream>

ClassImp(CP::TMCChannelId);

CP::TMCChannelId::TMCChannelId(UInt_t id): CP::TChannelId(id) {}

CP::TMCChannelId::TMCChannelId(const CP::TChannelId& src)
    : CP::TChannelId(src) {}

CP::TMCChannelId::TMCChannelId(UInt_t type,  UInt_t plane,  UInt_t wire) {
    SetGuardBit();
    SetSubDetector(kMC);
    SetType(type);
    SetPlane(plane);
    SetWire(wire);
}

CP::TMCChannelId::~TMCChannelId() {}

std::string CP::TMCChannelId::AsString() const {
    std::ostringstream str;

    str << CP::TChannelId::SubDetAsString().c_str();

    switch (GetType()) {
    case 0:     
        str << "-W-";
        switch (GetPlane()) {
        case 0:
            str << "X";
            break;
        case 1:
            str << "U";
            break;
        case 2:
            str << "V";
            break;
        default:
            str << "?";
        }
        str << "-" << GetWire();
        break;
    case 1:
        str << "-P-" << GetWire();
        break;
    default:
        str << "-?-" << GetPlane() << "-" << GetWire();
        break;
    }

    return str.str();
}

UInt_t CP::TMCChannelId::GetType() const {
    return GetField(kTypeMSB,kTypeLSB);
}

void CP::TMCChannelId::SetType(int val) {
    SetField(val,kTypeMSB,kTypeLSB);
}

UInt_t CP::TMCChannelId::GetPlane() const {
    return GetField(kPlaneMSB,kPlaneLSB);
}

void CP::TMCChannelId::SetPlane(int val) {
    SetField(val,kPlaneMSB,kPlaneLSB);
}

UInt_t CP::TMCChannelId::GetWire() const {
    return GetField(kWireMSB,kWireLSB);
}

void CP::TMCChannelId::SetWire(int val) {
    SetField(val,kWireMSB,kWireLSB);
}
