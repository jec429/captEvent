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
