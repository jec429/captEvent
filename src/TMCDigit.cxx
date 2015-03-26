#include "TMCDigit.hxx"

ClassImp(CP::TMCDigit);

CP::TMCDigit::TMCDigit() {}

CP::TMCDigit::TMCDigit(const CP::TMCDigit::ContributorContainer& contrib) 
    : fContributors(contrib) {}

CP::TMCDigit::TMCDigit(const CP::TMCDigit::ContributorContainer& contrib,
                       const CP::TMCDigit::InfoContainer& info) 
    : fContributors(contrib), fInformation(info) {}

CP::TMCDigit::~TMCDigit() {}
