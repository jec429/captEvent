#include <cstdio>

// This allows deprecated methods to be used.
#define METHOD_DEPRECATED
#include "TTPCChannelId.hxx"

ClassImp(CP::TTPCChannelId);

//----------------------------------------------------------------------
CP::TTPCChannelId::TTPCChannelId(UInt_t id): CP::TChannelId(id) {}

//----------------------------------------------------------------------
CP::TTPCChannelId::TTPCChannelId(const CP::TChannelId& src)
  : CP::TChannelId(src) {}

//----------------------------------------------------------------------
CP::TTPCChannelId::~TTPCChannelId() {}

//----------------------------------------------------------------------
CP::TTPCChannelId::TTPCChannelId(UInt_t dcc, UInt_t fem, UInt_t fec, 
			     UInt_t asic, UInt_t channel) {
    SetGuardBit();
    SetSubDetector(CP::TChannelId::kTPC);
    SetDCC(dcc);
    SetFEM(fem);
    SetFEC(fec);
    SetAsic(asic);
    SetChannel(channel);
}

#ifdef USE_DEPRECATED_TTPCChannelId_Constructor
//----------------------------------------------------------------------
CP::TTPCChannelId::TTPCChannelId(UInt_t tpc, UInt_t mm, UInt_t channel) {
    SetGuardBit();
    SetSubDetector(CP::TChannelId::kTPC);
    SetTPC(tpc);
    SetMM(mm);
    SetMMChannel(channel);
}
#endif

//----------------------------------------------------------------------
std::string CP::TTPCChannelId::AsString() const {
    const char* det = TChannelId::SubDetAsString().c_str();
    char buffer[30];

    /// \todo Code needs to be translated into C++ and use sstream.  
    std::snprintf(buffer,30,"%7s: TPC:%02d:%02d:%01d:%01d:%02d",
                  det, this->GetDCC(), this->GetFEM(),this->GetFEC(), 
                  this->GetAsic(), this->GetChannel());

    return std::string(buffer);
}


//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetDCC() const {
    return GetField(kDCCMSB, kDCCLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetDCC(int val) {
    SetField(val, kDCCMSB, kDCCLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetFEM() const {
    return GetField(kFEMMSB, kFEMLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetFEM(int val) {
    SetField(val, kFEMMSB, kFEMLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetFEC() const {
    return GetField(kFECMSB, kFECLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetFEC(int val) {
    SetField(val, kFECMSB, kFECLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetAsic() const {
    return GetField(kAsicMSB,kAsicLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetAsic(int val) {
    SetField(val, kAsicMSB,kAsicLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetChannel() const {
    return GetField(kChanMSB, kChanLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetChannel(int val) {
    SetField(val, kChanMSB, kChanLSB);
}

#ifdef USE_DEPRECATED_TPCCode
//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetTPC() const {
    return GetField(kTPCMSB,kTPCLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetTPC(int val) {
    SetField(val,kTPCMSB,kTPCLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetMM() const {
    return GetField(kMMegaMSB, kMMegaLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetMM(int val) {
    SetField(val,kMMegaMSB, kMMegaLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTPCChannelId::GetMMChannel() const {
    return GetField(kMMChanMSB, kMMChanLSB);
}

//----------------------------------------------------------------------
void CP::TTPCChannelId::SetMMChannel(int val) {
    SetField(val, kMMChanMSB, kMMChanLSB);
}
#endif

