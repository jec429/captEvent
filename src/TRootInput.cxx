//
// Implement a class to read an event from a file.
// 

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TFolder.h>

#include "TRootInput.hxx"

#include "TEvent.hxx"
#include "TManager.hxx"
#include "TInputManager.hxx"
#include "TCaptLog.hxx"

namespace {
    class TRootInputBuilder : public CP::TVInputBuilder {
    public:
        TRootInputBuilder() 
            : CP::TVInputBuilder("root", "Read a captEvent ROOT file") {}
        CP::TVInputFile* Open(const char* file) const {
            return new CP::TRootInput(file,"OLD");
        }
    };

    class TRootInputRegistration {
    public:
        TRootInputRegistration() {
            CP::TManager::Get().Input().Register(new TRootInputBuilder());
        }
    };
    TRootInputRegistration registrationObject;
}

CP::TRootInput::TRootInput(const char* name, Option_t* option, Int_t compress) 
    : fFile(NULL), fSequence(0), fEventTree(NULL), fEventPointer(0),
      fEventsRead(0), fAttached(false) {
    fFile = new TFile(name, option, "ROOT Input File", compress);
    if (!fFile || !fFile->IsOpen()) {
        throw CP::EInputFileMissing();
    }
    CaptError("ROOT may produce warning messages about missing ShowMembers" 
               << " methods which may be ignored");
    IsAttached();
    CP::TManager::Get().SetCurrentInputFile(fFile);
    CaptVerbose("Input file " << fFile->GetName() << " is open");
}

CP::TRootInput::TRootInput(TFile* file) 
    : fFile(file), fSequence(0), fEventTree(NULL), fEventPointer(0),
      fEventsRead(0), fAttached(false) {
    if (!fFile || !fFile->IsOpen()) {
        throw CP::ENoInputFile();
    }
    CaptError("ROOT may produce warning messages about missing ShowMembers" 
               << " methods which may be ignored");
    IsAttached();
    CP::TManager::Get().SetCurrentInputFile(fFile);
    CaptVerbose("Input file " << fFile->GetName() << " is open");
}

CP::TRootInput::~TRootInput(void) {
    Close();
    if (fFile) delete fFile;
}

#ifdef PRIVATE_COPY
CP::TRootInput::TRootInput(const CP::TRootInput& aFile) {
    // Copy constructor not implemented
    MayNotUse("Copy Constructor");
    return;
}
#endif

const char* CP::TRootInput::GetInputName() const {
    if (fFile) return fFile->GetName();
    return NULL;
}

bool CP::TRootInput::IsAttached(void) {
    if (!IsOpen()) return false;

    // Make sure the **** global file pointer is pointing to this file.
    if (gFile != fFile) {
        if (gFile) {
            CaptDebug("Changing current file from " << gFile->GetName()
                       << " to " << fFile->GetName() << " to read.");
        }
        fFile->cd();
    }

    // Make sure that we have the event tree.
    if (!fEventTree) {
        fEventTree = dynamic_cast<TTree*>(fFile->Get("captainEventTree"));
        if (!fEventTree) throw ENoEvents();
    }

    return true;
}

Int_t CP::TRootInput::GetEventsInFile(void) {
    // Returns number of events in this file that can be read.
    if (!IsAttached()) return 0;
    return static_cast<Int_t>(fEventTree->GetEntries());
}

Int_t CP::TRootInput::GetEventsRead(void) {
    // Returns number of events read from this file
    return fEventsRead;
}

bool CP::TRootInput::IsOpen(void) {
    return fFile->IsOpen();
}

bool CP::TRootInput::EndOfFile(void) {
    // Flag that we are past an end of the file.  This is true if the next
    // event to be read is before the first event, or after the last event.  
    if (fSequence<0) {
        fSequence = -1;
        return true;
    }
    if (GetEventsInFile()<=fSequence) {
        fSequence = GetEventsInFile();
        return true;
    }
    return false;
}

CP::TEvent* CP::TRootInput::NextEvent(int skip) {
    if (skip>0) fSequence += skip;
    return ReadEvent(++fSequence);
}

CP::TEvent* CP::TRootInput::PreviousEvent(int skip) {
    if (skip>0) fSequence -= skip;
    return ReadEvent(--fSequence);
}

CP::TEvent* CP::TRootInput::ReadEvent(Int_t n) {
    // Read the n'th event (starting from 0) in the file
    fSequence = n;
    if (fSequence<0) {
        fSequence = -1;
        return NULL;
    }

    if (!IsAttached()) return NULL;

    // Set up for a new event structure to be allocated.
    fEventPointer = new CP::TEvent;
    fEventTree->SetBranchAddress("Event",&fEventPointer);

    // Read the new event.
    int nBytes = fEventTree->GetEntry(fSequence);
    if (nBytes > 0) {
        fEventsRead++;
        fEventPointer->Register();
    } else {
        fSequence = GetEventsInFile();
        delete fEventPointer;
        fEventPointer = NULL;
    }

    CP::TManager::Get().SetCurrentInputFile(fFile);
    return fEventPointer;
}

void CP::TRootInput::Close(Option_t* opt) {
    TFile* current = CP::TManager::Get().CurrentInputFile();
    if (fFile == current) CP::TManager::Get().SetCurrentInputFile(NULL);
    fFile->Close(opt);
}

ClassImp(CP::TRootInput)

    
