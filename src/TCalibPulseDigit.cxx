#include "TCalibPulseDigit.hxx"
#include "TPulseDigit.hxx"

ClassImp(CP::TCalibPulseDigit);

CP::TCalibPulseDigit::TCalibPulseDigit() {}

CP::TCalibPulseDigit::TCalibPulseDigit(const CP::TDigitProxy& parent,
                                       double first, double last,
                                       const Vector& samples) 
    : TDigit((*parent)->GetChannelId()), fParent(parent),
      fFirstSample(first), fLastSample(last), fSamples(samples) {}

CP::TCalibPulseDigit::TCalibPulseDigit(const CP::TCalibPulseDigit& cpd) 
    : TDigit(cpd.GetChannelId()), fParent(cpd.GetParent()),
      fFirstSample(cpd.GetFirstSample()), fLastSample(cpd.GetLastSample()),
      fSamples(cpd.GetSamples()) { }
                                        


CP::TCalibPulseDigit::~TCalibPulseDigit() {}

double CP::TCalibPulseDigit::GetFirstSample() const {
    return fFirstSample;
}

double CP::TCalibPulseDigit::GetLastSample() const {
    return fLastSample;
}

std::size_t CP::TCalibPulseDigit::GetSampleCount() const {
    return fSamples.size();
}

double CP::TCalibPulseDigit::GetSample(int t) const {
    if (t < 0) return 0;
    if (fSamples.size() <= (std::size_t) t) return 0;
    return fSamples[t];
}

void CP::TCalibPulseDigit::SetSample(int t, double v) {
    fSamples[t] = v;
}

const CP::TCalibPulseDigit::Vector& CP::TCalibPulseDigit::GetSamples() const {
    return fSamples;
}

void CP::TCalibPulseDigit::ls(Option_t* opt) const {
    std::string option(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();

    int width = std::cout.width();
    std::cout << GetChannelId()
              << " " << std::setw(20) << fGeometryId << std::setw(width)
              << " T: " << GetFirstSample() << " to " << GetLastSample()
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
