#include "TPulseMCDigit.hxx"

ClassImp(CP::TPulseMCDigit);

CP::TPulseMCDigit::TPulseMCDigit() {}

CP::TPulseMCDigit::TPulseMCDigit(
    CP::TChannelId chan, 
    int first, 
    const CP::TPulseDigit::Vector& adcs,
    const CP::TMCDigit::ContributorContainer& contrib)
    : TPulseDigit(chan,first,adcs), 
      TMCDigit(contrib) {}

CP::TPulseMCDigit::TPulseMCDigit(
    CP::TChannelId chan, 
    int first, 
    const CP::TPulseDigit::Vector& adcs,
    const CP::TMCDigit::ContributorContainer& contrib,
    const CP::TMCDigit::InfoContainer& info)
    : TPulseDigit(chan,first,adcs), 
      TMCDigit(contrib,info) {}


CP::TPulseMCDigit::~TPulseMCDigit() {}

void CP::TPulseMCDigit::ls(Option_t* opt) const {
    std::string option(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();

    std::cout << GetChannelId()
              << " T: " << GetFirstSample() 
              << " (" << GetSampleCount() << ")";

    if (GetInformation().empty()) {
        std::cout << " I: missing";
    }
    else {
        std::cout << " I:";
        for (CP::TMCDigit::InfoContainer::const_iterator i
                 = GetInformation().begin();
             i != GetInformation().end(); ++i) {
            std::cout << " " << *i;
        }
    }
            
    if (GetContributors().empty()) {
        std::cout << " C: missing";
    }
    else {
        std::cout << " C:";
        for (CP::TMCDigit::ContributorContainer::const_iterator i
                 = GetContributors().begin();
             i != GetContributors().end(); ++i) {
            std::cout << " " << *i;
        }
    }
            
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
