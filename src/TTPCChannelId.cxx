#include <cstdio>

#include "TTPCChannelId.hxx"

ClassImp(CP::TTPCChannelId);

CP::TTPCChannelId::TTPCChannelId(UInt_t id): CP::TChannelId(id) {}

CP::TTPCChannelId::TTPCChannelId(const CP::TChannelId& src)
  : CP::TChannelId(src) {}

CP::TTPCChannelId::~TTPCChannelId() {}

CP::TTPCChannelId::TTPCChannelId(UInt_t crate, UInt_t fem, UInt_t channel) {
    SetGuardBit();
    SetSubDetector(CP::TChannelId::kTPC);
    SetCrate(crate);
    SetFEM(fem);
    SetChannel(channel);
}

std::string CP::TTPCChannelId::AsString() const {
    const char* det = TChannelId::SubDetAsString().c_str();
    char buffer[30];
    /// Crude, but effective...
    std::snprintf(buffer,30,"%7s:%02d:%02d:%02d",
                  det, GetCrate(), GetFEM(), GetChannel());
    return std::string(buffer);
}

const UInt_t CP::TTPCChannelId::GetCrate() const {
    return GetField(kCrateMSB, kCrateLSB);
}

void CP::TTPCChannelId::SetCrate(int val) {
    SetField(val, kCrateMSB, kCrateLSB);
}

const UInt_t CP::TTPCChannelId::GetFEM() const {
    return GetField(kFEMMSB, kFEMLSB);
}

void CP::TTPCChannelId::SetFEM(int val) {
    SetField(val, kFEMMSB, kFEMLSB);
}

const UInt_t CP::TTPCChannelId::GetChannel() const {
    return GetField(kChanMSB, kChanLSB);
}

void CP::TTPCChannelId::SetChannel(int val) {
    SetField(val, kChanMSB, kChanLSB);
}


