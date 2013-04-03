// $Id: TDataLink.cxx,v 1.1 2010/09/07 18:29:48 mcgrew Exp $
//
// Implement the TDataLink class.  This is a very simple class so almost
// all of the methods are implemented in line.  However the ClassImp
// macro must be in a separate file and the virtual distructor is also
// implemented here.

#include <TBrowser.h>

#include "TDataLink.hxx"

ClassImp(CP::TDataLink);

CP::TDataLink::~TDataLink() {
    // Null the fParent incase there are dangling pointers someplace.  
    fLink = NULL;
}

// This calls the function for the think that is being linked to.
Bool_t CP::TDataLink::IsFolder(void) const {
    Bool_t result =  GetThis()->IsFolder();
    return result;
}

// Called by the browser.  This calls the browser of the class that is being
// linked.  
void CP::TDataLink::Browse(TBrowser* b) {
    GetThis()->Browse(b);
}

