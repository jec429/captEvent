#include "TRealDatum.hxx"

ClassImp(CP::TRealDatum);

CP::TRealDatum::TRealDatum() 
    : CP::TDatum("unnamed","Real Value") {
    fValue.push_back(0);
}

CP::TRealDatum::TRealDatum(const char *name, double value) 
    : CP::TDatum(name,"Real Value") {
    fValue.push_back(value);
}

CP::TRealDatum::TRealDatum(const char *name, const char* title, double value) 
    : CP::TDatum(name,title) {
    fValue.push_back(value);
}

CP::TRealDatum::TRealDatum(const TRealDatum& rhs)
    : CP::TDatum(rhs.GetName(),rhs.GetTitle()), fValue(rhs.fValue) {}

CP::TRealDatum::~TRealDatum() {}

double CP::TRealDatum::GetValue(void) const {return fValue.front();}

void CP::TRealDatum::SetValue(double v) {fValue.front() = v;}

void CP::TRealDatum::ls(Option_t* opt) const {
    ls_header(this, opt);
    for (std::vector<double>::const_iterator v = fValue.begin();
         v != fValue.end();
         ++v) {
        std::cout << " " << *v;
    }
    std::cout << std::endl;
}
