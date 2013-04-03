// $Id: TDataSymLink.cxx,v 1.4 2007/11/28 20:08:05 mcgrew Exp $
//
// Implement the CP::TDataSymLink class.  This is a very simple class so almost
// all of the methods are implemented in line.  However the ClassImp
// macro must be in a separate file and the virtual distructor is also
// implemented here.

#include <TBrowser.h>

#include "TDataSymLink.hxx"

ClassImp(CP::TDataSymLink);

CP::TDataSymLink::~TDataSymLink() {
    // Null the fParent incase there are dangling pointers someplace.  
    fLink = "";
}

// This calls the function for the think that is being linked to.
Bool_t CP::TDataSymLink::IsFolder(void) const {
    Bool_t result =  GetThis()->IsFolder();
    return result;
}

// Called by the browser.  This calls the browser of the class that is being
// linked.  
void CP::TDataSymLink::Browse(TBrowser* b) {
    GetThis()->Browse(b);
}

void CP::TDataSymLink::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "-> " << fLink;
    const CP::TDatum *link = GetThis();
    if (link) {
        std::cout << " <" <<
            link->ClassName() << "(" << link << "):: " 
                  << link->GetFullName()
                  << ">";
    }
    else {
        std::cout << " <does not exist>";
    }
    std::cout << std::endl;
    TROOT::DecreaseDirLevel();
}
