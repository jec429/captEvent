#include "TDigitHeader.hxx"
#include "TDataVector.hxx"

ClassImp(CP::TDigitHeader);

CP::TDigitHeader::TDigitHeader()
    : fName(""), fBeginValid(0), fEndValid(0), fDataVector(0) {}

CP::TDigitHeader::TDigitHeader(const std::string& name)
    : fName(name), fBeginValid(0), fEndValid(0), fDataVector(0) {};

CP::TDigitHeader::~TDigitHeader() {}

void CP::TDigitHeader::AddDatum(CP::TDatum* val, const char* name) {
    if (!fDataVector) {
        fDataVector = new CP::TDataVector("info",
                                          "Digit Header Information");
    }
    fDataVector->AddDatum(val,name);
}

void CP::TDigitHeader::AddDatum(CP::THandle<CP::TDatum> val, const char* name) {
    if (!fDataVector) {
        fDataVector = new CP::TDataVector("DigitHeader",
                                          "Digit Header Information");
    }
    fDataVector->AddDatum(val,name);
}

void CP::TDigitHeader::ls(Option_t* opt) const {
    TROOT::IndentLevel();
    std::cout<<"CP::TDigitHeader("<<this<<"):: ";
    std::cout<<GetName()<<std::endl;
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Valid for digits: " << GetBeginValid()
              << " -- " << GetEndValid() << std::endl;
    if (fDataVector) fDataVector->ls(opt);
    TROOT::DecreaseDirLevel();
}
