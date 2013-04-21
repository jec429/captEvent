#include "TMCHeader.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TMCHeader);

CP::TMCHeader::TMCHeader()
    : CP::TDataVector("mcHeader", "Monte Carlo Header") {}

CP::TMCHeader::TMCHeader(const char* name, const char* title)
    : CP::TDataVector(name,title) {}

CP::TMCHeader::~TMCHeader() {}

void CP::TMCHeader::Init() {
    // Put field initialization here instead of in the constructor.  In C++11,
    // this can be done by putting the base constructor into the other
    // constructors initializationn lists.  e.g.
    //     TMCHeader(): TMCHeader(NULL)
    //     TMCHeader(char* name): fName(name), fIntensity(0.0) {}

    fIntensity = 0.0;

}

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
