#include "TND280MCHeader.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TND280MCHeader);

CP::TND280MCHeader::TND280MCHeader()
    : CP::TDataVector("mcHeader", "Monte Carlo Header"),
      fOffAxisField(0.0), fIntensity(0.0) {}

CP::TND280MCHeader::TND280MCHeader(const char* name, const char* title)
    : CP::TDataVector(name,title), fOffAxisField(0.0), fIntensity(0.0) {}

CP::TND280MCHeader::~TND280MCHeader() {}

void CP::TND280MCHeader::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Geometry Hash: " << GetGeometryHash() 
              << std::endl;
    TROOT::IndentLevel();
    std::cout << "Off Axis Magnetic Field: " << GetOffAxisField()/unit::tesla
              << " T"
              << std::endl;
    TROOT::IndentLevel();
    std::cout << "Intensity:               " << GetIntensity()
              << std::endl;
    TROOT::DecreaseDirLevel();
}
