#include <memory>
#include <iostream>
#include <TTree.h>
#include <eventLoop.hxx>

/// User code for the event loop.  This can be copied and used as a skeleton
/// for generic user loop programs.
class TUserLoop: public TEventLoopFunction {
public:
    /// Initialize any class specific variables, but most of the work can be
    /// done in Initialize.  Don't create histograms here!
    TUserLoop() {}
    virtual ~TUserLoop() {};

    /// Print a usage message.  This is generally called when there is a
    /// command line input error.
    void Usage(void) {
    }

    /// Set an option and return true if it is valid.  This is called by the
    /// event loop command line argument processing code for each "-O
    /// [name]=[value]" option found on the command line.  If the command line
    /// has "-O [name]" without a value, then the value string will be equal
    /// to "".  This must return false if the option was not correctly
    /// processed.
    virtual bool SetOption(std::string option,std::string value="") {
        return false;
    }

    /// Called for each event inside the event loop, and returns true if the
    /// event should be saved to the output file.  If the remainder of the
    /// current file should be skipped, this should through the
    /// ENextEventLoopFile exception.
    bool operator () (TEvent& event) {
        // Get the  hits
	THandle<THitSelection> hits(event.GetHits("drift"));
	// Update Tree values
	if (hits) {
	  fNHits = hits->size();
        }
	else {
	  fNHits = 0;
        }
        fOutputTree->Fill();
        return false; // Don't save events...
    }

    /// Called after the arguments are processed but before reading the first
    /// event.  The output file is open so any histograms will be added to the
    /// output file.  This example creates a simple tree with one branch.
    virtual void Initialize(void) {
        fOutputTree = new TTree("userLoopTree","A simple summary tree");
        fOutputTree->Branch("NHits", &fNHits, "NHits/I");
    }

    /// Called after reading the last event.  The output file is still open,
    /// so you can add extra information.  Because of an ideosyncracy in the
    /// way root handles histograms, objects created in Initialize() will
    /// already be stored in the output file.
    virtual void Finalize(TRootOutput* output) {
        fOutputTree->Write();
    }

private:
    /// A pointer to the output tree that is being created.
    TTree* fOutputTree;

    /// Number of hits.
    int    fNHits;
};

int main(int argc, char **argv) {
    TUserLoop userLoop;
    CP::eventLoop(argc,argv,userLoop);
}
