
#include <TClass.h>
#include <TStreamerInfo.h>

#include "TG4VHit.hxx"

ClassImp(CP::TG4VHit);
ClassImp(CP::TG4HitContainer);

CP::TG4VHit::TG4VHit() {}

CP::TG4VHit::~TG4VHit() {}

void CP::TG4VHit::ls(Option_t *opt) const {
    CP::ls_header(this,opt);
    std::cout << "Abstract Base Class"
              << std::endl;
}

CP::TG4HitContainer::~TG4HitContainer() {
    for (const_iterator v = begin();
         v != end(); 
         ++v) {
        delete *v;
    };
}

void CP::TG4HitContainer::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    std::string option(opt);
    if (option.find("dump") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        for (const_iterator v = begin();
             v != end(); 
             ++v) {
            (*v)->ls(opt);
        };
        TROOT::DecreaseDirLevel();
    }
}

