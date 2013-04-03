#include "TTPCmcDigit.hxx"

ClassImp(CP::TTPCmcDigit);

CP::TTPCmcDigit::TTPCmcDigit() {}

CP::TTPCmcDigit::TTPCmcDigit(CP::TChannelId chan, 
                             short startTime, 
                             std::vector<short> adcs,
                             const CP::TMCDigit::ContributorContainer& contrib)
    : TTPCDigit(chan,startTime,adcs), 
      TMCDigit(contrib) {}


CP::TTPCmcDigit::~TTPCmcDigit() {}
