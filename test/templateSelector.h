#ifndef templateSelector_hxx_seen
#define templateSelector_hxx_seen

#include "TND280EventsSelector.hxx"

/// An template of a user selector that is suitable to be
/// copied and renamed as part of analysis code.  To use
/// this in your code, copy "templateSelector.h" and
/// "templateSelector.C" to new files and replace "template"
/// with some other name.  After that all you need to do is
/// to replace Process in your copy of "templateSelector.C"
/// with your own code.  To run your code, copy
/// "runTemplate.C" to a new file and replace "template"
/// with some other name.
class templateSelector : public TND280EventsSelector {
 public:
     templateSelector() {}
     ~templateSelector() {}
     
     Bool_t Process(Int_t entry);
     
     ClassDef(templateSelector,0);
};
#endif
