#include "TTPCDigitHeader.hxx"

ClassImp(CP::TTPCDigitHeader);

//***********************************
CP::TTPCDigitHeader::TTPCDigitHeader():
  fCompressed(-1), fLosFlag(-1), fSyncFail(-1) {
//************************************************************

}

//***********************************
CP::TTPCDigitHeader::TTPCDigitHeader(bool c, bool l, bool s): 
  TDigitHeader("tpc"), fCompressed(c), fLosFlag(l), fSyncFail(s)  {
//************************************************************

}

//***********************************
CP::TTPCDigitHeader::~TTPCDigitHeader() {
//***********************************

}

