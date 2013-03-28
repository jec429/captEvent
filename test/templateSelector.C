#include <vector>

#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TParticlePDG.h>
#include <TMath.h>

#include "templateSelector.h"

Bool_t templateSelector::Process(Int_t entry) {
    std::cout << "######################################################" 
              << std::endl;
    fChain->GetTree()->GetEntry(entry);
    
    // This is an template of how to get at various pieces of information in an
    // ND280MC event file.  You should probably delete most of this from your
    // analysis. 
    std::cout << "Entry " << entry 
              << " is for event ID " << ND280Event->GetEventID() 
              << " with reaction code " << ND280Event->GetReactionCode()
              << std::endl;
    
    return kTRUE;
}
