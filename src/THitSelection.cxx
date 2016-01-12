////////////////////////////////////////////////////////////
// $Id: THitSelection.cxx,v 1.9 2010/08/03 21:58:34 mcgrew Exp $
//

#include <algorithm>
#include "THitSelection.hxx"

ClassImp(CP::THitSelection);

CP::THitSelection::THitSelection(const char* name, 
                             const char* title)
    : CP::TDatum(name,title) { }

CP::THitSelection::~THitSelection() { }

void CP::THitSelection::push_back(const CP::THandle<CP::THit>& hit) {
    if (!hit) {
        CaptSevere("Attempting to add a NULL hit");
        throw CP::EInvalidHit();
    }
    std::vector< CP::THandle<CP::THit> >::push_back(hit);
}

void CP::THitSelection::AddHit(const CP::THandle<CP::THit>& hit) {
    CP::THitSelection::iterator location
        = std::find(begin(), end(), hit);
    if (location == end()) push_back(hit);
}

void CP::THitSelection::RemoveHit(const CP::THandle<CP::THit>& hit) {
    CP::THitSelection::iterator location
        = std::find(begin(), end(), hit);
    if (location != end()) erase(location);
}

void CP::THitSelection::ls(Option_t* opt) const {
    ls_header(this,opt);
    std::cout << " (size: " << size() << ")" << std::endl;
    std::string option(opt);
    if (option.find("dump") != std::string::npos
        || option.find("hits") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        for (const_iterator v = begin();
             v != end(); 
             ++v) {
            v->ls(opt);
        }
        TROOT::DecreaseDirLevel();
    }
}
