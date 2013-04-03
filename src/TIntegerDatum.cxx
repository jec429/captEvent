#include "TIntegerDatum.hxx"

ClassImp(CP::TIntegerDatum);

CP::TIntegerDatum::TIntegerDatum() 
    : CP::TDatum("unnamed","Integer Value") {
    fValue.push_back(0);
}

CP::TIntegerDatum::TIntegerDatum(const char *name, int value) 
    : CP::TDatum(name,"Integer Value") {
    fValue.push_back(value);
}

CP::TIntegerDatum::TIntegerDatum(const char *name, const char* title, int value) 
    : CP::TDatum(name,title) {
    fValue.push_back(value);
}

CP::TIntegerDatum::TIntegerDatum(const TIntegerDatum& rhs)
    : CP::TDatum(rhs.GetName(),rhs.GetTitle()), fValue(rhs.fValue) {}

CP::TIntegerDatum::~TIntegerDatum() {}

int CP::TIntegerDatum::GetValue(void) const {return fValue.front();}

void CP::TIntegerDatum::SetValue(int v) {fValue.front() = v;}

std::vector<int>& CP::TIntegerDatum::GetVector(void) {return fValue;}

const std::vector<int>& CP::TIntegerDatum::GetVector(void) const {return fValue;}

void CP::TIntegerDatum::ls(Option_t*opt) const {
    ls_header(this, opt);
    for (std::vector<int>::const_iterator v = fValue.begin();
         v != fValue.end();
         ++v) {
        std::cout << " " << *v;
    }
    std::cout << std::endl;
}
