#include "TTPCDigit.hxx"

ClassImp(CP::TTPCDigit);

//**********************************************
CP::TTPCDigit::TTPCDigit() {}
//**********************************************

//**********************************************
CP::TTPCDigit::TTPCDigit(CP::TChannelId chan, short time, std::vector<short> adc) 
    : TDigit(chan), fFirstTimeSample(time), fADCs(adc) {}
//**********************************************

//**********************************************
CP::TTPCDigit::~TTPCDigit() {}
//**********************************************

//**********************************************
int CP::TTPCDigit::GetFirstTimeSample() const {
//**********************************************
  return fFirstTimeSample;
}

//**********************************************
int CP::TTPCDigit::GetNumberOfTimeSamples() const {
//**********************************************
return fADCs.size();
}

//**********************************************
int CP::TTPCDigit::GetADC(unsigned int t) const {
//**********************************************
  if (t>= 0 && t<fADCs.size())  
    return fADCs[t];
  else return 0;  
}


//**********************************************
std::vector<short> CP::TTPCDigit::GetADCs() const {
//**********************************************
return fADCs;
}

void CP::TTPCDigit::ls(Option_t* opt) const {
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << GetChannelId()
              << " T: (" << GetFirstTimeSample() 
              << " (" << GetNumberOfTimeSamples() << ")" 
              << std::endl;
    TROOT::DecreaseDirLevel();
}
