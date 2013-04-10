#ifndef TRootInput_hxx_seek
#define TRootInput_hxx_seek

#include <TFile.h>

#include "ECore.hxx"
#include "TVInputFile.hxx"

class TTree;

namespace CP {
    EXCEPTION(EInputFile,ECore);
    EXCEPTION(EInputFileMissing,EInputFile);
    EXCEPTION(ENoEvents,EInputFile);

    class TEvent;
    class TRootInput;
}

/// Attach to a file so that the events can be read.  This also checks
/// to see if the geometry is saved in the input file and reads it if
/// it can.  This will only read files that were written using the
/// TRootOutput class.  This will work with any file name, but the preferred
/// file extension is [name].root.
class CP::TRootInput : public TVInputFile {
public:
    /// Open an input file. 
    TRootInput(const char* fName, Option_t* option="", int compress = 1);

    /// Use a TFile that was opened someplace else.  The TRootInput object
    /// takes ownership of the file.  This is particularly useful if the file
    /// has been openned using the static TFile::Open method.  This method
    /// returns a pointer to a TFile derived class that is appropriate to the
    /// type of file (dist, http, &c).
    TRootInput(TFile* file);

    virtual ~TRootInput(void);
    
    /// Overload the base class filename.
    const char* GetFilename() const {return GetInputName();}

    /// Check that an input file is able to read an event.  If the
    /// file is not ready, then this will try to set up for reading.
    virtual bool IsAttached(void);

    /// Return the total number of events in this file 
    virtual Int_t GetEventsInFile(void);

    /// Get the number of events that have been read. 
    virtual Int_t GetEventsRead(void);

    /// Flag that the input file is open and ready to be read.
    virtual bool IsOpen(void);

    /// Flag that we are past an end of the file.  This is true if an attempt
    /// was made to read before the first event of the file, or after the last
    /// event in the file.
    virtual bool EndOfFile(void);

    /// Return the first event in the file.
    virtual TEvent* FirstEvent(void) {return ReadEvent(0);}

    /// Read the next event in the file.
    virtual TEvent* NextEvent(int skip = 0);

    /// Read the previous event in the file.
    virtual TEvent* PreviousEvent(int skip = 0);

    /// Return the first event in the file.
    virtual TEvent* LastEvent(void) {
        return ReadEvent(GetEventsInFile()-1);
    }

    /// Read the n'th event in the file.  The events are counted from zero.
    /// If the index is out-of-range, or no event can be read, this will
    /// return NULL.  For example, if event -1 is requested, this returns
    /// NULL.
    virtual TEvent* ReadEvent(Int_t n);

    /// Make sure that the file is closed.  This method is specific to
    /// TRootInput.
    virtual void Close(Option_t* opt = "");

    /// The generic way to close the file.  This is inherited from TVInputFile
    /// and must be provided for eventLoop (it's pure virtual).
    virtual void CloseFile() {Close();}

    /// Return the entry number of the event within the tree.
    virtual int GetPosition(void) const {
        if (fSequence<0) return -1;
        return fSequence;
    }

    /// Return the TFile that events are being read from.
    TFile* GetFilePointer(void) const {return fFile;}

    /// Return the file name to provide the base abstract input name class.
    virtual const char* GetInputName(void) const;

private:
    TFile* fFile;               // The file to get events from.
    Int_t fSequence;            // The sequence number of the last event read.

    TTree* fEventTree;          // the TTTree of event objects. 
    TEvent* fEventPointer; //! the last event read.
    
    Int_t fEventsRead;          //! count of events read from file
    bool fAttached;             //! are we prepared to read from the file?

#ifdef PRIVATE_COPY
private:
    TRootInput(const TRootInput& aFile);
#endif

    ClassDef(TRootInput,0);
};
#endif
