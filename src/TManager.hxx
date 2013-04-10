#ifndef TManager_hxx_seen
#define TManager_hxx_seen

#include <iostream>
#include <map>
#include <set>

#include <TObject.h>

#include "ECore.hxx"
#include "TPackageSet.hxx"
#include "TSHAHashValue.hxx"
#include "TAlignmentId.hxx"

class TGeoManager;
class TDatabasePDG;
class TFile;
class TTree;
class TGeoMatrix;

namespace CP {
    class TEvent;
    class TManager;
    class TDigitManager;
    class TGeomIdManager;
    class TRootInput;
    class TGeometryId;

    /// An exception from the data base.
    EXCEPTION(EManager,ECore);

    /// Flag that no file was found.
    EXCEPTION(ENoInputFile,EManager);

    /// Flag that geometry data base was not found.
    EXCEPTION(ENoGeometry,EManager);

    /// Flag that something went wrong with the alignment.
    EXCEPTION(EBadAlignment, ENoGeometry);

    /// Flag that tpc pad information data base was not found.
    EXCEPTION(ENoTPCPads,EManager);
}

/// Static access to the various geometry and calibration databases.  The main
/// table provided by this class are the ROOT geometry using
/// CP::TManager::Get().Geometry().  It also provides several managers such
/// as Digits(), GeomId(), TPCPads, and the PDG particle table in Particles().
///
/// This class is mostly used to access the root geometry using the Geometry()
/// method.  This method takes an event pointer as an argument, or uses the
/// current event in memory, and verifies that the correct geometry is
/// available.  In addition, if the oaApplyAlign library is available, it will
/// apply the alignment constant to the current event.
///
/// If user code needs to fill tables with geometry information (e.g. the
/// internal RECPACK sensor geometry), a CP::TManager::GeometryChange call
/// back should be registered using RegisterGeometryCallback().  See the
/// CP::TManager::GeometryChange class documentation for more details.
///
class CP::TManager {
    friend class CP::TRootInput;
    friend class CP::TGeomIdManager;

public:

    /// A base class to be registered by users providing a callback whenever
    /// the geometry has changed.  The callback is registered using the
    /// CP::TManager::RegisterGeometryCallback() method.
    ///
    /// This class is most usefule whenever the a user has tables that need to
    /// be reinitialized after a geometry change.  For example, if a user
    /// builds a table of sub-detector masses.  To use this class, a user will
    /// declare a local class derived from GeometryChange that overrides the
    /// Callback method.
    ///
    /// \code
    /// class MyChangeClass: public CP::TManager::GeometryChange {
    /// public:
    ///    void Callback(const CP::TEvent* const event) {
    ///        // Do something with the current geometry
    ///        TGeoManager* newGeom = CP::TManager::Get().Geometry();
    ///    }
    /// };
    /// \endcode
    ///
    /// This callback must then be registered with TManager.
    ///
    /// \code
    /// MyChangeClass* myChangeClass = new MyChangeClass;
    /// CP::TManager::Get().RegisterGeometryCallback(myChangeClass);
    /// \endcode
    ///
    /// All of the callbacks registered with TManager will then be called
    /// when the geometry changes.  The order of callbacks is unspecified
    /// (meaning don't count on a particular call order).
    class GeometryChange {
    public:
        GeometryChange() {}
        virtual ~GeometryChange() {}

        // A method to be implemented by the user that will be called each
        // time the geometry has changed.  The call back is called after the
        // alignment has been applied.  This method should not modify the
        // event.  In addition, don't depend on the order in which the
        // registered callbacks will be executed.
        virtual void Callback(const CP::TEvent* const event) = 0;
    };

    /// A base class to be registered to find the geometry associated with an
    /// event.  In general, a derived class will be implemented in a separate
    /// package and use off-line calibration database to associate an event
    /// context with a particular geometry.  The derived class must implement
    /// the GetHash method.
    class GeometryLookup {
    public:
        GeometryLookup() {}
        virtual ~GeometryLookup() {}
        
        /// Return the hash value of the geometry that should be used with the
        /// event.  If the hash value cannot be determined, an invalid hash
        /// should be returned.  If an invalid hash is returned, then a
        /// default geometry file will be used.
        ///
        /// Note: The hash value is the last method of finding a geometry file
        /// and can be overridden by providing a geometry directly in the data
        /// file, setting the geometry file explicitly using
        /// CP::TManager::SetGeometryFile(), or setting the hash value
        /// explicitly using CP::TManager::SetGeometryHash().
        /// 
        /// This class is used in the FindEventGeometry method.
        virtual TSHAHashValue GetHash(const CP::TEvent* const event) {
            return TSHAHashValue();
        }
    };

    /// A base class to be registered to find the alignment parameters
    /// associated with a particular geometry.  In general, a derived class
    /// will be implemented in a separate package and use off-line calibration
    /// database to associate an event context with a particular alignment.
    ///
    /// The derived class must implement the Align method.  The StartAlignment
    /// methods can be optionally implemented and is used to notify the
    /// AlignmentLookup class that a new alignment is being started.
    /// 
    /// This class is used in the AlignGeometry method.
    class AlignmentLookup {
    public:
        AlignmentLookup() {}
        virtual ~AlignmentLookup() {}

        /// Called to find out if the alignment should be reapplied.  If the
        /// alignment should be reapplied this will return true.  If the
        /// geometry has been changed, then the alignment will be reapplied
        /// regardless of the CheckAlignment return value.
        virtual bool CheckAlignment(const CP::TEvent* const event) {
            return true; 
        }
        
        /// Called before the geometry alignment for an event is begun.  This
        /// is used to notify the implemented class that a new alignment is
        /// about to start.  It will generally trigger initialization.  If an
        /// alignment should not be applied, then this should return an empty
        /// alignment.
        virtual CP::TAlignmentId 
        StartAlignment(const CP::TEvent* const event) {
            return CP::TAlignmentId();
        }
        
        /// Return the alignment for a particular geometry id.  This will be
        /// called multiple times.  The alignment correction is returned as a
        /// TGeoMatrix pointer.  When there is no more alignment to be
        /// returned, this should return a NULL TGeoMatrix pointer.  The
        /// geomId argument is used to return the geometry identifier of the
        /// volume to be aligned.  The ownership of the TGeoMatrix is claimed
        /// by the calling routine, so it must be generated using "new".
        ///
        /// The returned alignment is relative to the nominal position and
        /// rotation of the volume, and is specified in the master coordinate
        /// system.  
        virtual TGeoMatrix* Align(const CP::TEvent* const event,
                                  TGeometryId& geomId) {return NULL;}
    };

    virtual ~TManager() {;};

    /// Get a pointer to the singleton instance of the database.
    static TManager& Get(void);

    /// Return a pointer to the correct detector geometry for the event.  If
    /// the argument is NULL, then this will try to get the geometry for the
    /// current event (as available from TEventFolder::GetCurrentEvent(), or
    /// the most recent available geometry.  This method leaves the geometry
    /// unchanged if the current geometry is valid, and the current event
    /// context has not changed.
    ///
    /// \note This method may change the state of the geometry.  This cannot
    /// be changed since the geometry may or may not needed to be reloaded
    /// depending on the state of the event.  Users should plan on either
    /// checking the state of the geometry after using this method, or setting
    /// the geometry state using CdNode, or FindNode.  In addition, users
    /// should assume that calling this method could invalidate any node
    /// identifiers they may be using. (e.g. the result of
    /// TGeoManager::GetCurrentNodeId()).
    ///
    /// The geometry is searched for in the following order: If the geometry
    /// file has been explicity set using CP::TManager::SetGeometryFile(),
    /// then use that geometry.  Otherwise, if an explicit geometry hash value
    /// has been set using CP::TManager::SetGeometryHash(), use that
    /// geometry.  Otherwise, if the event file contains a geometry, try to
    /// use that geometry.  Otherwise, use the geometry associated with a hash
    /// value returned by the registered CP::TManager::GeometryLookup class
    /// which generally looks up the hash value using the calibration
    /// database.
    /// 
    /// \note The new geometry will only be loaded if there is a current
    /// event.  If there isn't a current event, a previously loaded geometry
    /// will be returned.  If there isn't a current and there isn't a
    /// previously loaded geometry, this will throw a ENoGeometry exception.
    /// If you want to force a geometry to be loaded even though you don't
    /// currently have access to an event, you can use
    /// CP::TManager::Get().GeomId().ReadEvent(CP::TSHAHashValue()).  While
    /// this isn't convenient, loading a geometry without an event is also
    /// rather unusual.
    /// 
    /// If none of the attempts to find a geometry work this throws an
    /// ENoGeometry exception; however, since not having a geometry is a very
    /// serious error you will probably want your program to crash so you
    /// don't need to catch the ENoGeometry exception.
    ///
    /// After the geometry is loaded alignment parameters are applied.  The
    /// alignment is applied using the AlignGeometry() method which uses
    /// CP::TManager::AlignmentLookup class to lookup the coeficients.
    ///
    /// Users should register the CP::TManager::GeometryLookup and
    /// CP::TManager::AlignmentLookup classes using the
    /// RegisterGeometryLookup and RegisterAlignmentLookup methods.
    ///
    /// If you need to use tables that get initialized for each new geometry
    /// (for instance, you might want to cache the locations of various
    /// detectors for use in RECPACK), then you need to define and register a
    /// CP::TManager::GeometryChange class using
    /// CP::TManager::RegisterGeometryCallback.  See the
    /// CP::TManager::GeometryChange for details and an example.
    TGeoManager* Geometry(TEvent* event=NULL);

    /// Return a reference to the ROOT TDatabasePDG.
    TDatabasePDG& ParticleData(void);

    /// Return a reference to the set of packages linked to the current
    /// executable.
    TPackageSet& PackageSet(void);

    /// Return a reference to the TDigitManager.
    TDigitManager& Digits(void);

    /// Return a reference to the TGeomIdManager.  The TGeomIdManager class
    /// manages the geometries and alignments that are loaded to use with an
    /// event.  It provides the implementation required to implement
    /// TManager::Geometry().  All book keeping information about the
    /// current geometry (such as it's hashcode or the alignment id) should be
    /// accessed through TGeomIdManager.
    TGeomIdManager& GeomId(void);

    /// @{ Provide a file to override the standard geometry.  Setting the
    /// geometry file to any value other than "" will cause TManager to
    /// override the standard (default) geometry with the one found in the
    /// geometry file.  This resets the required geometry if called without an
    /// argument.
    void SetGeometryOverride(const std::string& geoName ="");
    void SetGeometryOverride(const CP::TSHAHashValue& hc);
    /// @}

    /// Register a user callback that will be called whenever the geometry
    /// changes (see the CP::TManager::GeometryChange class).
    void RegisterGeometryCallback(CP::TManager::GeometryChange* callback);

    /// Remove a user callback.
    void RemoveGeometryCallback(CP::TManager::GeometryChange* callback);

    /// Remove all user callbacks.
    void ClearGeometryCallbacks();

    /// Register a class implementing the search for the correct geometry to
    /// be used for an event.  If called with "NULL", the default geometry
    /// lookup is installed.
    CP::TManager::GeometryLookup* RegisterGeometryLookup(
        CP::TManager::GeometryLookup* lookup);

    /// Register a class implementing the search for the correct geometry to
    /// be used for an event.  A pointer to the previous alignment lookup
    /// class is returned.
    CP::TManager::AlignmentLookup* RegisterAlignmentLookup(
        CP::TManager::AlignmentLookup* lookup);

private:
    TManager();
    TManager(const TManager&);
    TManager& operator=(const TManager&);

    /// Find the hash code for a particular event using a registered
    /// GeometryLookup object.  This returns the hash code of the geometry to
    /// use for the event.  If the event hash code is not found, then this
    /// returns an invalid hash code.  This is used by TGeomIdManager to
    /// access the GeometryLookup class.
    TSHAHashValue LookupGeometry(const TEvent *const event) const;

    /// Call the registered geometry callbacks.  This is used by
    /// TGeomIdManager to notify the user routines when the geometry changes.
    void ApplyGeometryCallbacks(const CP::TEvent* event);

    /// Check if there is a valid alignment.
    bool HaveAlignment() {return (fAlignmentLookup != NULL);}

    /// Use the AlignmentLookup method to check if a new alignment should be
    /// applied.  If this returns true, the we need a new alignment.  This is
    /// used by TGeomIdManager to short circuit the alignment application.
    bool CheckAlignment(const CP::TEvent* const event);

    /// Use the AlignmentLookup class to start applying the alignment.  This
    /// is used by the TGeomIdManager to start applying alignment.
    CP::TAlignmentId StartAlignment(const CP::TEvent* const event);

    /// Use the AlignmentLookup class to return an alignment matrix.
    std::pair<CP::TGeometryId,TGeoMatrix*> Alignment(const CP::TEvent* event);
    
    /// Set the current input file for the data base.  This must be set for
    /// many of the methods to work correctly.  This is used by TRootInput to
    /// notify TManager when the input file changes.
    void SetCurrentInputFile(TFile* input);
    
    /// Get the current input event file.  This is used by TGeomIdManager to
    /// find the file with the geometry.
    TFile* CurrentInputFile(void) {return fCurrentInputFile;}

    static TManager *fManager;
    
    /// The current event file.  This is only valid for a ROOT input file.
    TFile* fCurrentInputFile;

    /// A pointer to a class which will find the correct geometry to be used
    /// with an event.
    GeometryLookup* fGeometryLookup;

    /// A pointer to a class which will find the alignment associated with the
    /// current geometry used for this event.
    AlignmentLookup* fAlignmentLookup;

    /// A connection to the ROOT particle data book data base.
    TDatabasePDG* fParticleData;

    /// The set of packages linked with the executable.
    TPackageSet fPackageSet;
    
    /// Manage the connection to the oaRawEvent digitized data.
    TDigitManager* fDigits;

    /// Manage the mapping of the root TGeoManager node identifiers to the
    /// geometry identifiers.
    TGeomIdManager* fGeomId;

    /// A vector of call backs that will be called whenever the geometry
    /// changes.
    std::set<CP::TManager::GeometryChange*> fGeometryCallbacks;

    ClassDef(TManager,0);
};
#endif
