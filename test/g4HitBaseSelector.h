#ifndef g4HitBaseSelector_hxx_seen
#define g4HitBaseSelector_hxx_seen

#include <TH1F.h>

#include "TND280EventsSelector.hxx"

/// An g4HitBase of a user selector that is suitable to be
/// copied and renamed as part of analysis code.  To use
/// this in your code, copy "g4HitBaseSelector.h" and
/// "g4HitBaseSelector.C" to new files and replace "g4HitBase"
/// with some other name.  After that all you need to do is
/// to replace Process in your copy of "g4HitBaseSelector.C"
/// with your own code.  To run your code, copy
/// "runG4HitBase.C" to a new file and replace "g4HitBase"
/// with some other name.
class g4HitBaseSelector : public TND280EventsSelector {
 public:
     g4HitBaseSelector() {}
     ~g4HitBaseSelector() {}
     
     void Begin(TTree* tree);

     Bool_t Process(Int_t entry);

     void Terminate(void);
     
 private:
     TH1F* fTPCz;
     TH1F* fP0Dz;
     TH1F* fFGDz;
     TH1F* fECALz;

     ClassDef(g4HitBaseSelector,0);
};
#endif
