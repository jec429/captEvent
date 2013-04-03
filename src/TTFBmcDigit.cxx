#include "TTFBmcDigit.hxx"

ClassImp(CP::TTFBmcDigit);

CP::TTFBmcDigit::TTFBmcDigit() {}

CP::TTFBmcDigit::TTFBmcDigit(CP::TChannelId chan, 
			     int highGainADC, 
			     int lowGainADC,
			     unsigned int timeCounter,
			     int timeQuad,
			     bool timeDiscriminator,
                             const CP::TMCDigit::ContributorContainer& contrib) 
  : TTFBDigit(chan,highGainADC,lowGainADC, timeCounter, timeQuad, timeDiscriminator), 
    TMCDigit(contrib) {}

CP::TTFBmcDigit::~TTFBmcDigit() {}
