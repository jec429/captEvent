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
    SetSequence(plane);
    SetNumber(wire);
}

CP::TMCChannelId::~TMCChannelId() {}

std::string CP::TMCChannelId::AsString() const {
    std::ostringstream str;

    str << CP::TChannelId::SubDetAsString().c_str();

    switch (GetType()) {
    case 0:     
        str << "-W-";
        switch (GetSequence()) {
        case 0:
            str << "X";
            break;
        case 1:
            str << "V";
            break;
        case 2:
            str << "U";
            break;
        default:
            str << "?";
        }
        str << "-" << GetNumber();
        break;
    case 1:
        str << "-P-" << GetNumber();
        break;
    default:
        str << "-?-" << GetSequence() << "-" << GetNumber();
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

UInt_t CP::TMCChannelId::GetSequence() const {
    return GetField(kSequenceMSB,kSequenceLSB);
}

void CP::TMCChannelId::SetSequence(int val) {
    SetField(val,kSequenceMSB,kSequenceLSB);
}

UInt_t CP::TMCChannelId::GetNumber() const {
    return GetField(kNumberMSB,kNumberLSB);
}

void CP::TMCChannelId::SetNumber(int val) {
    SetField(val,kNumberMSB,kNumberLSB);
}
