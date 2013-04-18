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
    std::string option(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();

    std::cout << GetChannelId()
              << " T: " << GetFirstSample() 
              << " (" << GetNumberOfSamples() << ")";

    TROOT::IncreaseDirLevel();

    if (option.find("digits") != std::string::npos) {
        int sample = 0;
        for (CP::TPulseDigit::Vector::const_iterator d = fADCs.begin();
             d != fADCs.end(); ++d) {
            if ((0<sample) && 0 == (sample%10)) {
                std::cout << std::endl;
                TROOT::IndentLevel();
                std::cout << sample << " --";
            }
            std::cout << " " << *d;
            ++sample;
        }
    }

    TROOT::DecreaseDirLevel();

    std::cout << std::endl;

    TROOT::DecreaseDirLevel();
}
