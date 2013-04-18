#include "TTPCDigit.hxx"

ClassImp(CP::TTPCDigit);

CP::TTPCDigit::TTPCDigit() {}

CP::TTPCDigit::TTPCDigit(CP::TChannelId chan, int first, const Vector& adc) 
    : TDigit(chan), fFirstSample(first), fADCs(adc) {}


CP::TTPCDigit::~TTPCDigit() {}

int CP::TTPCDigit::GetFirstSample() const {
    return fFirstSample;
}

std::size_t CP::TTPCDigit::GetNumberOfSamples() const {
    return fADCs.size();
}

int CP::TTPCDigit::GetADC(int t) const {
    if (t < 0) return 0;
    if (fADCs.size() <= (std::size_t) t) return 0;
    return fADCs[t];
}


const CP::TTPCDigit::Vector& CP::TTPCDigit::GetADCs() const {
    return fADCs;
}

void CP::TTPCDigit::ls(Option_t* opt) const {
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << GetChannelId()
              << " T: (" << GetFirstSample() 
              << " (" << GetNumberOfSamples() << ")" 
              << std::endl;
    TROOT::DecreaseDirLevel();
}
