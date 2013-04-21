#include "TCalibPulseDigit.hxx"
#include "TPulseDigit.hxx"

ClassImp(CP::TCalibPulseDigit);

CP::TCalibPulseDigit::TCalibPulseDigit() {}

CP::TCalibPulseDigit::TCalibPulseDigit(CP::TPulseDigit* parent,
                                       double first,
                                       const Vector& samples) 
    : TDigit(fParent->GetChannelId()), fParent(parent),
      fFirstSample(first), fSamples(samples) {}


CP::TCalibPulseDigit::~TCalibPulseDigit() {}

double CP::TCalibPulseDigit::GetFirstSample() const {
    return fFirstSample;
}

std::size_t CP::TCalibPulseDigit::GetSampleCount() const {
    return fSamples.size();
}

double CP::TCalibPulseDigit::GetSample(int t) const {
    if (t < 0) return 0;
    if (fSamples.size() <= (std::size_t) t) return 0;
    return fSamples[t];
}

const CP::TCalibPulseDigit::Vector& CP::TCalibPulseDigit::GetSamples() const {
    return fSamples;
}

void CP::TCalibPulseDigit::ls(Option_t* opt) const {
    std::string option(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();

    std::cout << GetChannelId()
              << " T: " << GetFirstSample() 
              << " (" << GetSampleCount() << ")";

    TROOT::IncreaseDirLevel();

    if (option.find("digits") != std::string::npos) {
        int sample = 0;
        for (CP::TCalibPulseDigit::Vector::const_iterator d = fSamples.begin();
             d != fSamples.end(); ++d) {
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
