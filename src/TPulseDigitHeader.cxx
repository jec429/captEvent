#include "TPulseDigitHeader.hxx"
#include "nanoStamp.hxx"
#include "TUnitsTable.hxx"

ClassImp(CP::TPulseDigitHeader);

CP::TPulseDigitHeader::TPulseDigitHeader()
    : TDigitHeader("pulse"), fTimeStamp(0) { }

CP::TPulseDigitHeader::TPulseDigitHeader(const std::string& name)
    : TDigitHeader(name), fTimeStamp(0) {}

CP::TPulseDigitHeader::~TPulseDigitHeader() { }

void CP::TPulseDigitHeader::ls(Option_t* opt) const {
    TROOT::IndentLevel();
    std::cout<< "CP::TPulseDigitHeader("<<this<<"):: "
             << GetName() 
             << " valid for " << GetBeginValid()
             << " -- " << GetEndValid() << std::endl;
    TROOT::IncreaseDirLevel();
    std::time_t t = CP::NanoStampToTime(GetTimeStamp());
    if (t > 0) {
        TROOT::IndentLevel();
        char buf[256];
        std::strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",std::gmtime(&t));
        double ns = CP::NanoStampFromLastSecond(GetTimeStamp());
        std::cout << "Time Stamp: " << buf << " "
                  << CP::TUnitsTable::Get().ConvertTime(ns) << std::endl;
    }
    else {
        TROOT::IndentLevel();
        std::cout << "Time Stamp: none" << std::endl;
    }
    TROOT::DecreaseDirLevel();
}
