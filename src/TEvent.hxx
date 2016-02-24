#ifndef TEvent_hxx_seen
#define TEvent_hxx_seen

#include <string>

#include "TObject.h"

#include "TCaptLog.hxx"

#include "TDataVector.hxx"
#include "THandle.hxx"
#include "THitSelection.hxx"
#include "TAlgorithmResult.hxx"
#include "TEventContext.hxx"
#include "TDigitContainer.hxx"
#include "TSHAHashValue.hxx"
#include "TAlignmentId.hxx"
#include "nanoStamp.hxx"

/// The standard namespace for captain offline software (see \ref namespaces).  
namespace CP {
    class TEvent;
}

/// The event container (get the current event using
/// TEventFolder::GetCurrentEvent()).
///
/// Provide the highest level event container for events.  This is a
/// TDataVector and provides an effective "root directory" for all of the
/// information saved with an event.  This also contains the infrastructure to
/// interface with TEventFolder class that couples events to the ROOT browser
/// and will appear as a root folder in //root/Events/ named by the event
/// number.  For example, an event that is read will appear as a folder,
/// "//root/Events/event.0.00023.00001324".
///
/// The TEvent class provides the following event structure.
///
/// - event/
///   - digits/ Raw data for the sensor hits.  This is a copy of the data 
///             available from the MIDAS DAQ data files.
///   - hits/   Data about the sensor hits.  This contains calibrated hit
///             data. 
///   - truth/  Exists only for MC events.  This is all of the information 
///             saved from the MC.   
///   - fits/   Results from fitters.  This is a data vector full of results 
///             saved from any fitter run on this event.
///
/// TEvent objects are usually read from an input file using a
/// TRootInput object (output is with a TRootOutput).  The last event read
/// will be available from TEventFolder::GetCurrentEvent() which is a static
/// method.  Interactive programs which want to provide a method to keep
/// multiple events in memory (e.g. the event display), should get the current
/// event using the TEventFolder::GetCurrentEvent() method.  Events can then
/// be saved automatically into a root folder by calling the
/// TEventFolder::GetEventFolder() static method.
///
class CP::TEvent: public TDataVector {
public:
    TEvent();

    /// Create a new TEvent with the context, but not the header, filled.
    TEvent(const CP::TEventContext& context);

    virtual ~TEvent();

    /// Get the context for this event.
    const CP::TEventContext& GetContext() const {return fContext;}

    /// Get the hash value of the geometry associated with this event.  If the
    /// event doesn't have a particular geometry associated with it, then
    /// the hash value will be zero (This happens for files processed with
    /// early versions of the calibration).  The hash code is used to
    /// associate the proper geometry with the event when determining the
    /// positions of the hits (the hit positions are not saved with the
    /// event).
    const CP::TSHAHashValue& GetGeometryHash() const {return fGeometryHash;}

    /// Get the alignment id of the alignment constants associated with this
    /// event.  The alignment identifier is a SHA1 hash code of the alignment
    /// constats plus a documentation string.  If the event doesn't have a
    /// particular alignment associated with it, then the hash code will be
    /// zero (this happens for files processed with early versions of the
    /// calibration).  The hash code is used to associate the proper alignment
    /// constants with the event when determining the positions of the hits
    /// (the hit positions are not saved with the event).
    const CP::TAlignmentId& GetAlignmentId() const {return fAlignmentId;}

    /// Set the context for this event.
    void SetContext(CP::TEventContext context) {
        fContext = context;
        Build();
    }

    /// Set the hash value for the geometry.  This should be used by the
    /// calibrations as the hits are created.
    void SetGeometryHash(const TSHAHashValue& h) {fGeometryHash = h;}
    
    /// Set the alignment identifier for the event.  This should be used by the
    /// calibrations as the hits are created.
    void SetAlignmentId(const TAlignmentId& h) {fAlignmentId = h;}
    
    /// Set the event number.
    void SetRunId(UInt_t i) {
        fContext.SetRun(i);
        Build();
    }
    
    /// Get the event number.
    UInt_t GetRunId(void) const {return fContext.GetRun();}

    /// Set the event number.
    void SetEventId(UInt_t i) {
        fContext.SetEvent(i);
        Build();
    }

    /// Set the high precision time stamp for the event.
    /// @{
    void SetTimeStamp(int s, int ns) {fTimeStamp = CP::TimeToNanoStamp(s,ns);}
    void SetTimeStamp(CP::NanoStamp ns) {fTimeStamp = ns;}
    /// @}

    /// Get the high precision time stamp for the event.  This should be the
    /// highest precision time available for the event, but it might just be
    /// the computer time.  The NanoStamp is a uint64_t, which is defined in
    /// nanoStamp.hxx along with some utility routines.
    CP::NanoStamp GetTimeStamp() const {return fTimeStamp;}
    
    /// Get the event number.
    UInt_t GetEventId(void) const {return fContext.GetEvent();}

    /// Get the digit container by name (may not exist).  This will get the
    /// digits for a sub-detector.  They may not be saved since we often need
    /// to save space.
    THandle<CP::TDigitContainer> GetDigits(const char *name);

    /// Get the hit selection by name.
    THandle<CP::THitSelection> GetHits(const char *name) const;

    /// Get a TAlgorithmResult by name.
    THandle<CP::TAlgorithmResult> GetFit(const char* name) const;

    /// Add a TAlgorithmResult object.
    void AddFit(TAlgorithmResult* fit, const char* name=NULL);
    void AddFit(THandle<CP::TAlgorithmResult> fit, const char* name=NULL);

    /// Print the object information.
    virtual void ls(Option_t *opt = "") const;

    void Register();

private:

    /// The context for this event.
    TEventContext fContext;

    /// The hash value of the geometry that should be associated with this
    /// event.  This must be set during calibration.
    TSHAHashValue fGeometryHash;

    /// The alignment identifier of the constants that should be applied to
    /// the geometry when handling this event.
    TAlignmentId fAlignmentId;

    /// The timestamp for the event as nanoseconds since Jan 1, 1970.
    NanoStamp fTimeStamp;
    
    /// Build internal structure of the event.
    void Build();

    ClassDef(TEvent,9);
};
#endif
