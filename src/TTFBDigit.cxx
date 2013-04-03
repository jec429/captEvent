#include "TTFBDigit.hxx"

ClassImp(CP::TTFBDigit);

CP::TTFBDigit::TTFBDigit() {}

CP::TTFBDigit::TTFBDigit(CP::TChannelId chan, 
                         int highGainADC,
                         int lowGainADC, 
                         unsigned int timeCounter,
                         int timeQuad,
                         bool timeDiscriminator)
    : TDigit(chan), fHighGainADC(highGainADC), fLowGainADC(lowGainADC),
      fTimeCounter(timeCounter) {
    fBits = timeQuad & 0x03;
    if (timeDiscriminator) fBits |= 0x04;
    else fBits &= 0xFB; 
}

CP::TTFBDigit::~TTFBDigit() {}

void CP::TTFBDigit::ls(Option_t* opt) const {
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << GetChannelId()
              << " T: (" << CheckTimeDiscriminator()
              << "," << GetTimeQuad() << "," << GetTimeCounter() << ")"
              << " Q: (" << GetHighGainADC() << "," << GetLowGainADC() << ")"
              << std::endl;
    TROOT::DecreaseDirLevel();
}

