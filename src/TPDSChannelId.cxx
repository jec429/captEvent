#include <cstdio>

#include "TPDSChannelId.hxx"

ClassImp(CP::TPDSChannelId);

CP::TPDSChannelId::TPDSChannelId(UInt_t id): CP::TChannelId(id) {}

CP::TPDSChannelId::TPDSChannelId(const CP::TChannelId& src)
  : CP::TChannelId(src) {}

CP::TPDSChannelId::~TPDSChannelId() {}

CP::TPDSChannelId::TPDSChannelId(UInt_t crate, UInt_t digitizer, UInt_t chan) {
    SetGuardBit();
    SetSubDetector(CP::TChannelId::kPDS);
    SetCrate(crate);
    SetDigitizer(digitizer);
    SetChannel(chan);
}

std::string CP::TPDSChannelId::AsString() const {
    const char* det = TChannelId::SubDetAsString().c_str();
    char buffer[30];
    /// Crude, but effective...
    std::snprintf(buffer,30,"%3s:%02d:%02d:%02d",
                  det, GetCrate(), GetDigitizer(), GetChannel());
    return std::string(buffer);
}

const UInt_t CP::TPDSChannelId::GetCrate() const {
    return GetField(kCrateMSB, kCrateLSB);
}

void CP::TPDSChannelId::SetCrate(int val) {
    SetField(val, kCrateMSB, kCrateLSB);
}

const UInt_t CP::TPDSChannelId::GetDigitizer() const {
    return GetField(kDigitizerMSB, kDigitizerLSB);
}

void CP::TPDSChannelId::SetDigitizer(int val) {
    SetField(val, kDigitizerMSB, kDigitizerLSB);
}

const UInt_t CP::TPDSChannelId::GetChannel() const {
    return GetField(kChanMSB, kChanLSB);
}

void CP::TPDSChannelId::SetChannel(int val) {
    SetField(val, kChanMSB, kChanLSB);
}
