#ifndef TND280Event_hxx_seen
#define TND280Event_hxx_seen

#include <string>

#include "TObject.h"

#include "TND280Log.hxx"

#include "TDataVector.hxx"
#include "THandle.hxx"
#include "THitSelection.hxx"
#include "TAlgorithmResult.hxx"
#include "TND280Context.hxx"
#include "TDigitContainer.hxx"
#include "TSHAHashValue.hxx"
#include "TAlignmentId.hxx"

/// The standard namespace for nd280 offline software (see \ref namespaces).  
namespace ND {
    class TND280Event;
}

/// The event container (get the current event using
/// TEventFolder::GetCurrentEvent()).
///
/// Provide the highest level event container for ND280 off-axis events.  This
/// is a TDataVector and provides an effective "root directory" for all of the
/// information saved with an event.  This also contains the infrastructure to
/// interface with TEventFolder class that couples events to the ROOT browser
/// and will appear as a root folder in //root/Events/ named by the event
/// number.  For example, an event that is read will appear as a folder,
/// "//root/Events/event.0.00023.00001324".
///
/// The TND280Event class provides the following event structure.
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
/// TND280Event objects are usually read from an input file using a
/// TND280Input object (output is with a TND280Output).  The last event read
/// will be available from TEventFolder::GetCurrentEvent() which is a static
/// method.  Interactive programs which want to provide a method to keep
/// multiple events in memory (e.g. the event display), should get the current
/// event using the TEventFolder::GetCurrentEvent() method.  Events can then
/// be saved automatically into a root folder by calling the
/// TEventFolder::GetEventFolder() static method.
///
class ND::TND280Event: public TDataVector {
public:
    TND280Event();

    /// Create a new TND280Event with the context, but not the header, filled.
    TND280Event(const ND::TND280Context& context);

    virtual ~TND280Event();

    /// Get the context for this event.
    const ND::TND280Context& GetContext() const {return fContext;}

    /// Get the hash value of the geometry associated with this event.  If the
    /// event doesn't have a particular geometry associated with it, then
    /// the hash value will be zero (This happens for files processed with
    /// early versions of the calibration).  The hash code is used to
    /// associate the proper geometry with the event when determining the
    /// positions of the hits (the hit positions are not saved with the
    /// event).
    const ND::TSHAHashValue& GetGeometryHash() const {return fGeometryHash;}

    /// Get the alignment id of the alignment constants associated with this
    /// event.  The alignment identifier is a SHA1 hash code of the alignment
    /// constats plus a documentation string.  If the event doesn't have a
    /// particular alignment associated with it, then the hash code will be
    /// zero (this happens for files processed with early versions of the
    /// calibration).  The hash code is used to associate the proper alignment
    /// constants with the event when determining the positions of the hits
    /// (the hit positions are not saved with the event).
    const ND::TAlignmentId& GetAlignmentId() const {return fAlignmentId;}

    /// Set the context for this event.
    void SetContext(ND::TND280Context context) {
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
    void SetRunId(int i) {
        fContext.SetRun(i);
        Build();
    }
    
    /// Get the event number.
    int GetRunId(void) const {return fContext.GetRun();}

    /// Set the event number.
    void SetEventId(int i) {
        fContext.SetEvent(i);
        Build();
    }
    
    /// Get the event number.
    int GetEventId(void) const {return fContext.GetEvent();}

    /// Get the digit container by name (may not exist).  This will get the
    /// digits for a sub-detector.  They will be available for if oaUnpack can
    /// be loaded, or if digits were saved by the elecSim.  
    /// 
    /// \note (For advanced users) If the event contains the raw data, this
    /// will try to unpack the digits from MIDAS using oaUnpack.  By default,
    /// the cached digits are saved as temporary datum, but can be made
    /// persistent using TOADatabase::Get().Digits().PersistentDigits().  If
    /// the digits are made to be persistent, then they will continue to be
    /// available even if the TND280RawEvent datum is deleted.
    THandle<ND::TDigitContainer> GetDigits(const char *name);

    /// Get the hit selection by name.
    THandle<ND::THitSelection> GetHitSelection(const char *name) const;

    /// Get a TAlgorithmResult by name.
    THandle<ND::TAlgorithmResult> GetFit(const char* name) const;

    /// Add a TAlgorithmResult object.
    void AddFit(TAlgorithmResult* fit, const char* name=NULL);
    void AddFit(THandle<ND::TAlgorithmResult> fit, const char* name=NULL);

    /// Print the object information.
    virtual void ls(Option_t *opt = "") const;

    void Register();

private:

    /// The context for this event.
    TND280Context fContext;

    /// The hash value of the geometry that should be associated with this
    /// event.  This must be set during calibration.
    TSHAHashValue fGeometryHash;

    /// The alignment identifier of the constants that should be applied to
    /// the geometry when handling this event.
    TAlignmentId fAlignmentId;

    /// Build internal structure of the event.
    void Build();

    ClassDef(TND280Event,8);
};
#endif
