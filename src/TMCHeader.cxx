#include "TMCHeader.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TMCHeader);

CP::TMCHeader::TMCHeader()
    : CP::TDataVector("mcHeader", "Monte Carlo Header"),
      fIntensity(0.0) {}

CP::TMCHeader::TMCHeader(const char* name, const char* title)
    : CP::TDataVector(name,title), fIntensity(0.0) {}

CP::TMCHeader::~TMCHeader() {}

void CP::TMCHeader::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Geometry Hash: " << GetGeometryHash() 
              << std::endl;
    TROOT::IndentLevel();
    std::cout << "Intensity: " << GetIntensity()
              << std::endl;
    TROOT::DecreaseDirLevel();
}
