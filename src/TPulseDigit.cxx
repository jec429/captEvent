#include "TPulseDigit.hxx"

ClassImp(CP::TPulseDigit);

CP::TPulseDigit::TPulseDigit() {}

CP::TPulseDigit::TPulseDigit(CP::TChannelId chan, int first, const Vector& adc) 
    : TDigit(chan), fFirstSample(first), fSamples(adc) {}

CP::TPulseDigit::~TPulseDigit() {}

int CP::TPulseDigit::GetFirstSample() const {
    return fFirstSample;
}

std::size_t CP::TPulseDigit::GetSampleCount() const {
    return fSamples.size();
}

int CP::TPulseDigit::GetSample(int t) const {
    if (t < 0) return 0;
    if (fSamples.size() <= (std::size_t) t) return 0;
    return fSamples[t];
}

const CP::TPulseDigit::Vector& CP::TPulseDigit::GetSamples() const {
    return fSamples;
}

void CP::TPulseDigit::ls(Option_t* opt) const {
    std::string option(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();

    std::cout << GetChannelId() << " opt: " << opt
              << " T: " << GetFirstSample() 
              << " (" << GetSampleCount() << ")";

    TROOT::IncreaseDirLevel();

    if (option.find("digits") != std::string::npos) {
        int sample = 0;
        for (CP::TPulseDigit::Vector::const_iterator d = GetSamples().begin();
             d != GetSamples().end(); ++d) {
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
