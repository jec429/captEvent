#include "TPulseDigit.hxx"

ClassImp(CP::TPulseDigit);

CP::TPulseDigit::TPulseDigit() {}

CP::TPulseDigit::TPulseDigit(CP::TChannelId chan, int first, const Vector& adc) 
    : TDigit(chan), fFirstSample(first), fADCs(adc) {}


CP::TPulseDigit::~TPulseDigit() {}

int CP::TPulseDigit::GetFirstSample() const {
    return fFirstSample;
}

std::size_t CP::TPulseDigit::GetNumberOfSamples() const {
    return fADCs.size();
}

int CP::TPulseDigit::GetADC(int t) const {
    if (t < 0) return 0;
    if (fADCs.size() <= (std::size_t) t) return 0;
    return fADCs[t];
}


const CP::TPulseDigit::Vector& CP::TPulseDigit::GetADCs() const {
    return fADCs;
}

void CP::TPulseDigit::ls(Option_t* opt) const {
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << GetChannelId()
              << " T: (" << GetFirstSample() 
              << " (" << GetNumberOfSamples() << ")" 
              << std::endl;
    TROOT::DecreaseDirLevel();
}
