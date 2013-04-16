#include "TTFBChannelId.hxx"
#include <cstdio>

ClassImp(CP::TTFBChannelId);

//----------------------------------------------------------------------
CP::TTFBChannelId::TTFBChannelId(UInt_t id): CP::TChannelId(id) {}

//----------------------------------------------------------------------
CP::TTFBChannelId::TTFBChannelId(const CP::TChannelId& src)
  : CP::TChannelId(src) {}

//----------------------------------------------------------------------
CP::TTFBChannelId::TTFBChannelId(UInt_t subDet,  UInt_t rmm,  UInt_t tfb,
                                 UInt_t tripChip, UInt_t tripChannel, 
                                 UInt_t capacitor) {
    SetGuardBit();
    SetSubDetector(subDet);
    SetRMM(rmm);
    SetTFB(tfb);
    SetTripChip(tripChip);
    SetChannel(tripChannel);
    SetCapacitor(capacitor);
}

//----------------------------------------------------------------------
CP::TTFBChannelId::~TTFBChannelId() {}

//----------------------------------------------------------------------
std::string CP::TTFBChannelId::AsString() const {
  const char* det = CP::TChannelId::SubDetAsString().c_str();
  
  char trip = 'U';
  switch (GetTripChip()) {
  case 0:     trip = 'A';     break;
  case 1:     trip = 'B';     break;
  case 2:     trip = 'C';     break;
  case 3:     trip = 'D';     break;
  default:    trip = 'X';     break;
  }

  char cap[3];
  if (GetCapacitor() != 24) {
    std::snprintf(cap,3,"%02d",GetCapacitor());
  }
  else{
    std::sprintf(cap,"--");
  }
  char buffer[30];
  std::snprintf(buffer,30,"%7s: TFB:%02d:%02d:%2s:%c:%02d",
		det, GetRMM(), GetTFB(), cap,
		trip, GetChannel());
  
  return std::string(buffer);
}

//----------------------------------------------------------------------
CP::TTFBChannelId CP::TTFBChannelId::IgnoreCapacitor() const{
    TTFBChannelId id24(*this);
    id24.SetCapacitor(24);
    return id24;
}

//----------------------------------------------------------------------
CP::TTFBChannelId CP::TTFBChannelId::IgnoreChannel() const{
    TTFBChannelId id(this->IgnoreCapacitor());
    id.SetChannel(18);
    return id;
}

//----------------------------------------------------------------------
CP::TTFBChannelId CP::TTFBChannelId::IgnoreTripChip() const{
    TTFBChannelId id(this->IgnoreChannel());
    id.SetTripChip(0);
    return id;
}

//----------------------------------------------------------------------
CP::TTFBChannelId CP::TTFBChannelId::IgnoreTFB() const{
    TTFBChannelId id(this->IgnoreTripChip());
    id.SetTFB(0);
    return id;
}

//----------------------------------------------------------------------
const UInt_t CP::TTFBChannelId::GetRMM() const {
    return GetField(kRMMBoardMSB,kRMMBoardLSB);
}

//----------------------------------------------------------------------
void CP::TTFBChannelId::SetRMM(int val) {
    SetField(val,kRMMBoardMSB,kRMMBoardLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTFBChannelId::GetTFB() const {
    return GetField(kTFBPortMSB, kTFBPortLSB);
}

//----------------------------------------------------------------------
void CP::TTFBChannelId::SetTFB(int val) {
    SetField(val, kTFBPortMSB, kTFBPortLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTFBChannelId::GetCapacitor() const {
    return GetField(kCapacitorMSB, kCapacitorLSB);
}

//----------------------------------------------------------------------
void CP::TTFBChannelId::SetCapacitor(int val) {
    SetField(val, kCapacitorMSB, kCapacitorLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTFBChannelId::GetTripChip() const {
    return GetField(kTripTMSB, kTripTLSB);
}

//----------------------------------------------------------------------
void CP::TTFBChannelId::SetTripChip(int val) {
    SetField(val, kTripTMSB, kTripTLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTFBChannelId::GetChannel() const {
    return GetField(kTripTChannelMSB, kTripTChannelLSB);
}

//----------------------------------------------------------------------
void CP::TTFBChannelId::SetChannel(int val) {
    SetField(val, kTripTChannelMSB, kTripTChannelLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TTFBChannelId::GetCableId() const {
    return IgnoreCapacitor().AsUInt();
}

//----------------------------------------------------------------------
const UInt_t CP::TTFBChannelId::GetTripTId() const {
    TTFBChannelId cableId(*this);
    cableId.SetChannel(0);
    cableId.SetCapacitor(0);
    return cableId.AsUInt();
}
