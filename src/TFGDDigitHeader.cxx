#include "TFGDDigitHeader.hxx"

ClassImp(CP::TCMBDigitHeader);
ClassImp(CP::TFGDDigitHeader);

CP::TCMBDigitHeader::TCMBDigitHeader() : 
    fMinicrate(-1), fTriggerCounter(-1), fTimestamp(-1),
    fFecError(-1), fLosFlag(-1), fSynFail(-1)
    {}

CP::TCMBDigitHeader::TCMBDigitHeader(int Minicrate, unsigned int TriggerCounter, unsigned int Timestamp,
                                 unsigned int FecError, unsigned int LosFlag, unsigned int SynFail) : 
    fMinicrate(Minicrate), fTriggerCounter(TriggerCounter), fTimestamp(Timestamp),
    fFecError(FecError), fLosFlag(LosFlag), fSynFail(SynFail)
    {}

CP::TFGDDigitHeader::TFGDDigitHeader(): 
  TDigitHeader("fgd")  {}


CP::TFGDDigitHeader::~TFGDDigitHeader(){}
