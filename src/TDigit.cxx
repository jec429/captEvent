#include "TDigit.hxx"

ClassImp(CP::TDigit);

CP::TDigit::TDigit() {}

CP::TDigit::TDigit(const CP::TChannelId& channel) 
  : fChannelId(channel) {}

CP::TDigit::~TDigit() {}

const CP::TChannelId& CP::TDigit::GetChannelId() const {
    return fChannelId;
}

void CP::TDigit::ls(Option_t* opt) const {
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << GetChannelId() << std::endl;
    TROOT::DecreaseDirLevel();
}

