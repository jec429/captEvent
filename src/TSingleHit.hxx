#ifndef TSingleHit_hxx_seen
#define TSingleHit_hxx_seen

#include "THit.hxx"

class TGeoManager;

namespace CP {
    class TSingleHit;
    class TMCHit;
    class TDataHit;
    class TWritableMCHit;
    class TWritableDataHit;
}

/// A single hit.  This cannot be directly instantiated.  
class CP::TSingleHit: public THit {
private:
    friend class TMCHit;
    friend class TDataHit;
    friend class TWritableMCHit;
    friend class TWritableDataHit;

    // The TSingleHit class can't be directly instantiated.  It is created
    // using the TWritableMCHit, or TWritableDataHit classes and is accessed
    // as a THit class.
    TSingleHit();
    TSingleHit(const TSingleHit&);

public:
    virtual ~TSingleHit();

    /// Return the calibrated "charge" for the hit.
    virtual double GetCharge(void) const;

    /// Return the uncertainty in the charge of the hit.
    virtual double GetChargeUncertainty(void) const;
    
    /// Return the calibrated "time" for the hit.
    virtual double GetTime(void) const;

    /// Return the uncertainty on the hit time.
    virtual double GetTimeUncertainty(void) const;

    /// Return the rms of the calibrated time for the hit.  This represents
    /// the extent (or spread) of the time distribution.
    virtual double GetTimeRMS(void) const; 

    /// Return the time of the start of the first sample contributing to this
    /// hit.
    virtual double GetTimeStart() const;

    /// Return the time of the end of the last sample contributing to this
    /// hit.
    virtual double GetTimeStop() const;

    /// Get the number of samples saved with this hit.
    virtual int GetTimeSamples() const;

    /// Get the value of a sample saved with this hit.
    virtual double GetTimeSample(int i) const;
    
    /// The center of the volume associated with this hit.
    virtual const TVector3& GetPosition(void) const;

    /// Return the uncertainty of the hit position in the local coordinate
    /// system of the hit (diagonal by definition).
    virtual const TVector3& GetUncertainty(void) const;

    /// Return the rms of the hit position in the local coordinate system of
    /// the hit.  For objects like scintillator bars, or wires, this is the
    /// spacing/sqrt(12).
    virtual const TVector3& GetRMS(void) const;

    /// Get a rotation to transform from the local rotation to the global
    /// coordinates.
    virtual const TMatrixD& GetRotation(void) const;

    /// Return the geometry identifier of the volume associated with this hit.
    /// This uniquely identifies the volume containing the hit (e.g. the
    /// scintillator bar, or the TPC micromega pad).  Notice that it is
    /// possible for there to be more than one hit in a single volume.  For
    /// instance, if a scintillator has double ended readout, it will have two
    /// associated channels that will both generate a hit for the same volume.
    /// This sort of double hit will generally be handled using a TReconHit.
    ///
    /// For most users, the geometry identifiers replace the functionality of
    /// the TGeoManager node identifiers.  The most common usage of the node
    /// identifier is to set the current node using TGeoManager::CdNode().
    /// The same operation can be done with a TGeometryId using
    /// TGeomIdManager::CdId(TGeometryId).  The CdId method makes the volume
    /// identified by the TGeometryId into the current TGeoManager volume, and
    /// then the user can use the various TGeoManager methods to query
    /// information.  For instance, the node identifier associated with a
    /// TGeometryId can be found using
    ///
    /// \code
    /// THandle<THit> myHit = ... Must have a value ...
    /// TManager::Get().GeomId().CdId(myHit->GetGeomId());
    /// int node = gGeoManager->GetCurrentNodeId();
    /// \endcode
    ///
    /// Notice that using TGeomIdManager::CdId() changes the state of the
    /// TGeoManager in exactly the same way as TGeoManager::CdNode().
    ///
    /// The name of the volume is accessible using TGeometryId::GetName(), and
    /// the position is available either through THit::GetPosition(), or
    /// TGeometryId::GetPosition().
    virtual TGeometryId GetGeomId(int i=0) const;

    virtual int GetGeomIdCount(void) const {return 1;}

private: 
    /// Fill all of the geometry related fields from the geometry data base.
    void Initialize();

    /// A helper routine to handle the generic geometry data base (for now,
    /// this is everything).
    bool InitializeGeneric();

protected:
    /// The geometry node that was hit
    Int_t fGeomId;

    /// The measured charge for this hit.
    Float_t fCharge;

    /// The uncertainty in the measured charge.
    Float_t fChargeUncertainty;

    /// The measured "time" for this hit.
    Float_t fTime;

    /// The uncertainty for the timing.
    Float_t fTimeUncertainty;

    /// The RMS of the timing.
    Float_t fTimeRMS;

    /// The time of the first sample in the hit.
    Float_t fTimeStart;

    /// The time of the last sample in the hit.
    Float_t fTimeStop;

    /// The charge samples in the hit.
    std::vector<Float_t> fTimeSamples;
    
    /// This is set to true if the fast access fields below have been
    /// initialized.
    bool fInitialized; //! Don't Save

    /// The central position of the hit.  This is the center of the wire in
    /// the global coordinate system.
    TVector3 fPosition; //! Don't Save

    /// The uncertainty of the hit position in local coordinates
    TVector3 fUncertainty; //! Don't Save

    /// The spread of the hit position in local coordinates
    TVector3 fRMS; //! Don't Save

    /// The rotation of the hit.
    TMatrixD fRotation; //! Don't Save

    ClassDef(TSingleHit,4);
};
#endif
