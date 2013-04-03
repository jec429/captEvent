#include "TFGDDigit.hxx"

ClassImp(CP::TFGDDigit);

CP::TFGDDigit::TFGDDigit() {}

CP::TFGDDigit::TFGDDigit(CP::TChannelId chan, 
                         short startTime, short pulseTime, short pulseCharge, 
                         std::vector<unsigned short> adc, short dataType)
    : TDigit(chan), fFirstTimeSample(startTime), fPulseTime(pulseTime),
      fPulseCharge(pulseCharge), 
      fADCs(adc), fDataType(dataType) {}

CP::TFGDDigit::~TFGDDigit() {}

