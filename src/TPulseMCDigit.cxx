#include "TPulseMCDigit.hxx"

ClassImp(CP::TPulseMCDigit);

CP::TPulseMCDigit::TPulseMCDigit() {}

CP::TPulseMCDigit::TPulseMCDigit(CP::TChannelId chan, 
                             int first, 
                             const CP::TPulseDigit::Vector& adcs,
                             const CP::TMCDigit::ContributorContainer& contrib)
    : TPulseDigit(chan,first,adcs), 
      TMCDigit(contrib) {}


CP::TPulseMCDigit::~TPulseMCDigit() {}
