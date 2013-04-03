#include <cstdio>
#include "TCaptLog.hxx"
#include "TFGDChannelId.hxx"

#define RAWFGD_CHAN_PER_ASIC 82
#define RAWFGD_CHAN_PER_FEB 64

// This is the map of which FEB channel each asic channel corresponds to.
static Int_t fgd_asic_feb_channel_map[2][RAWFGD_CHAN_PER_ASIC]= {
  // First ASIC 
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 1, 1, -1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, -1, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, -1, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, -1, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, -1, -1, -1},
    // Second ASIC
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, 32, 33, 33, -1, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, -1, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, -1, 52, 52, 53, 53, 54, 54, 55, 55, 56, 56, 57, 57, -1, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, -1, -1, -1}
};
    
static Int_t fgd_asic_att_channel_map[2][RAWFGD_CHAN_PER_ASIC]= {
  // First ASIC 
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, -1, -1},
    // Second ASIC
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, -1, -1, -1}
};

static UInt_t fgd_feb_asic_map[RAWFGD_CHAN_PER_FEB]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

static UInt_t fgd_feb_asic_channel_map[RAWFGD_CHAN_PER_FEB]= {11, 13, 16, 18, 20, 22, 24, 26, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 54, 56, 58, 60, 62, 64, 67, 69, 71, 73, 75, 77, 11, 13, 16, 18, 20, 22, 24, 26, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 54, 56, 58, 60, 62, 64, 67, 69, 71, 73, 75, 77};

ClassImp(CP::TFGDChannelId);

//----------------------------------------------------------------------
CP::TFGDChannelId::TFGDChannelId(UInt_t id): TChannelId(id) {}

//----------------------------------------------------------------------
CP::TFGDChannelId::TFGDChannelId(const CP::TChannelId& src)
  : TChannelId(src) {}

//----------------------------------------------------------------------
CP::TFGDChannelId::~TFGDChannelId() {}

//----------------------------------------------------------------------
CP::TFGDChannelId::TFGDChannelId(UInt_t subDet, UInt_t minicrate, UInt_t feb, 
                                 UInt_t afterChip, UInt_t afterChannel) {

    if (subDet != CP::TChannelId::kFGD) {
        CaptSevere("Invalid sub-detector field: " << subDet);
    }

    SetGuardBit();
    SetSubDetector(CP::TChannelId::kFGD);
    SetMinicrate(minicrate);
    SetFEB(feb);
    SetAfterChip(afterChip);
    SetChannel(afterChannel);
}

//----------------------------------------------------------------------
CP::TFGDChannelId::TFGDChannelId(UInt_t subDet, UInt_t minicrate, UInt_t feb, 
                                 UInt_t febCh) {

    UInt_t afterChip=0;
    UInt_t afterChannel=0;
    
    // Figure out which asic and asic channel this FEB channel is associated
    // with.
    if (febCh < RAWFGD_CHAN_PER_FEB) {
        afterChip = fgd_feb_asic_map[febCh];
        afterChannel = fgd_feb_asic_channel_map[febCh];
    }

    if (subDet != CP::TChannelId::kFGD) {
        CaptSevere("Invalid sub-detector field: " << subDet);
    }

    SetGuardBit();
    SetSubDetector(CP::TChannelId::kFGD);
    SetMinicrate(minicrate);
    SetFEB(feb);
    SetAfterChip(afterChip);
    SetChannel(afterChannel);
}

//----------------------------------------------------------------------
std::string CP::TFGDChannelId::AsString() const {
  const char* det = TChannelId::SubDetAsString().c_str();
  char buffer[50];
  if(GetFebChannel() < 0)
      std::snprintf(buffer,30,"%7s: FGD:%02d:%01d:%01d:%02d",
                    det, this->GetMinicrate(),this->GetFEB(), 
                    this->GetAfterChip(), this->GetChannel());
  else
      std::snprintf(buffer,50,"%7s: FGD:%02d:%01d:%01d:%02d (FEB CH=%02i, ATT=%01i)",
                    det, this->GetMinicrate(),this->GetFEB(), 
                    this->GetAfterChip(), this->GetChannel(),
                    this->GetFebChannel(), this->GetFebChAttenuation());      

  return std::string(buffer);
}

//----------------------------------------------------------------------
const UInt_t CP::TFGDChannelId::GetMinicrate() const {
    return GetField(kMinicrateMSB, kMinicrateLSB);
}

//----------------------------------------------------------------------
void CP::TFGDChannelId::SetMinicrate(int val) {
    SetField(val,kMinicrateMSB, kMinicrateLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TFGDChannelId::GetFEB() const {
    return GetField(kFEBMSB, kFEBLSB);
}

//----------------------------------------------------------------------
void CP::TFGDChannelId::SetFEB(int val) {
    SetField(val, kFEBMSB, kFEBLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TFGDChannelId::GetAfterChip() const {
    return GetField(kAfterMSB, kAfterLSB);
}

//----------------------------------------------------------------------
void CP::TFGDChannelId::SetAfterChip(int val) {
    SetField(val, kAfterMSB, kAfterLSB);
}

//----------------------------------------------------------------------
const UInt_t CP::TFGDChannelId::GetChannel() const {
    return GetField(kAfterChanMSB, kAfterChanLSB);
}

//----------------------------------------------------------------------
void CP::TFGDChannelId::SetChannel(int val) {
    SetField(val, kAfterChanMSB, kAfterChanLSB);
}

//----------------------------------------------------------------------
const Int_t CP::TFGDChannelId::GetFebChannel() const {

    UInt_t asic = GetAfterChip();
    UInt_t asicCh = GetChannel();

    if((asic==0 || asic==1) &&
       asicCh >= 0 && asicCh < RAWFGD_CHAN_PER_ASIC)
        return fgd_asic_feb_channel_map[asic][asicCh];

    return -1;
}

//----------------------------------------------------------------------
const Int_t CP::TFGDChannelId::GetFebChAttenuation() const {

    UInt_t asic = GetAfterChip();
    UInt_t asicCh = GetChannel();

    if((asic==0 || asic==1) &&
       asicCh >= 0 && asicCh < RAWFGD_CHAN_PER_ASIC)
        return fgd_asic_att_channel_map[asic][asicCh];

    return -1;
}
