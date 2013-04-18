#include "TTPCmcDigit.hxx"

ClassImp(CP::TTPCmcDigit);

CP::TTPCmcDigit::TTPCmcDigit() {}

CP::TTPCmcDigit::TTPCmcDigit(CP::TChannelId chan, 
                             int first, 
                             const CP::TTPCDigit::Vector& adcs,
                             const CP::TMCDigit::ContributorContainer& contrib)
    : TTPCDigit(chan,first,adcs), 
      TMCDigit(contrib) {}


CP::TTPCmcDigit::~TTPCmcDigit() {}
