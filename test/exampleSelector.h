#ifndef exampleSelector_hxx_seen
#define exampleSelector_hxx_seen

#include "TND280EventsSelector.hxx"

/// An example of a user selector that is suitable to be copied and renamed as
/// part of analysis code.  To use this in your code, copy "exampleSelector.h"
/// and "exampleSelector.C" to new files and replace "example" with some other
/// name.  After that all you need to do is to replace Process in your copy of
/// "exampleSelector.C" with your own code.  To run your code, copy
/// "runExample.C" to a new file and replace "example" with some other name.

class exampleSelector : public TND280EventsSelector {
 public:
     exampleSelector() {}
     ~exampleSelector() {}
     
     Bool_t Process(Int_t entry);
     
     ClassDef(exampleSelector,0);
};
#endif
