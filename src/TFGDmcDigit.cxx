#include "TFGDmcDigit.hxx"

ClassImp(CP::TFGDmcDigit);

CP::TFGDmcDigit::TFGDmcDigit() {}

CP::TFGDmcDigit::TFGDmcDigit(CP::TChannelId chan, 
                             short startTime, short pulseTime, short pulseCharge, 
                             std::vector<unsigned short> adcs,
                             short dataType,
                             const CP::TMCDigit::ContributorContainer& contrib)
    : TFGDDigit(chan,startTime,pulseTime,pulseCharge,adcs,dataType), 
      TMCDigit(contrib) {}


CP::TFGDmcDigit::~TFGDmcDigit() {}
