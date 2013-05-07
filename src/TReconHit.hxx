#ifndef TReconHit_hxx_seen
#define TReconHit_hxx_seen

#include <iostream>
#include <vector>

#include "THit.hxx"
#include "TGeometryId.hxx"

namespace CP {
    class TReconHit;
    class TWritableReconHit;

    EXCEPTION(EReconHitIllegal,ECore);
}

/// A calibrated hit detector element where the hit information has been
/// reconstructed from the raw hits.  A TReconHit object represents a hit that
/// has been generated from one or more hits where the position and other
/// properties are calculated from the constituient hits.  A common usage is to
/// represent a hit that has been constructed from two or more hits on a wires
/// that cross at a single X/Y positon.  This is a lighter object that
/// TReconCluster and is primarily used for building up 2D hits into 3D
/// objects.  Complex combinations of hits should be placed into a
/// TReconCluster. 
///
/// The TReconHit class can't be directly instantiated.  It is created
/// using the TWritableReconHit class and is accessed as a THit class.
class CP::TReconHit : public THit {
public:
    TReconHit();
    TReconHit(const TWritableReconHit& val);
    virtual ~TReconHit();

    /// Return the calibrated "charge" for the hit.
    virtual double GetCharge(void) const;

    /// Return the uncertainty in the charge for the hit.
    virtual double GetChargeUncertainty() const;

    /// Return the calibrated "time" for the hit representing the mean (or
    /// central) time of the hit.
    virtual double GetTime(void) const;

    /// Return the uncertainty on the hit time.
    virtual double GetTimeUncertainty(void) const;

    /// Return the rms of the calibrated time for the hit.  This represents
    /// the extent (or spread) of the time distribution.
    virtual double GetTimeRMS(void) const; 

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

    /// Return a contributing hit.  If the index is out of range, this will
    /// throw an EHitOutOfRange exception.  By default this will throw an
    /// EHitOutOfRange, but it may be over-ridden in a derived class.
    virtual CP::THandle<CP::THit> GetConstituent(int i=0) const;

    /// Return the number of hits that contribute to this hit.
    virtual int GetConstituentCount() const;

    /// Return a proxy to the digit that generated this hit.  The TDigitProxy
    /// will be valid if a TDigitContainer is available, and if the digit
    /// being accessed is in the first 131,000 hits (2^17).  Accessing hits
    /// with an invalid index, missing TDigitContainer, or outside of that
    /// range will cause a EHitOutOfRange exception.  For specialized studies,
    /// digits after the first 131,000 can be accessed by using GetChannelId()
    /// and then searching for the appropriate hit in the TDigitContainer.
    /// This search requires knowledge of how the digits are translated into
    /// hits.
    virtual const CP::TDigitProxy& GetDigit(int i=0) const;

    /// Return the number of digits that contribute to this hit.
    virtual int GetDigitCount() const;

    /// Return the channel identifier associated with this hit.  If the hit
    /// has an associated channel identifier, then this will return a valid
    /// TChannelId.  If there isn't an associated hit, this will return an
    /// invalid value (check with TChannelId::IsValid).  If the index is out
    /// of range, then this will throw an EHitOutOfRange exception.  This
    /// information is also available through the digit, and it is an error
    /// condition if the two values disagree.
    virtual TChannelId GetChannelId(int i=0) const;

    /// Return the number of channel identifiers associated with the hit.
    virtual int GetChannelIdCount() const;

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

    /// Return the number of geometry identifiers associated with this hit.
    virtual int GetGeomIdCount() const;

    /// Print the hit information.
    virtual void ls(Option_t *opt = "") const;

private:
  
    /// Fill all of the geometry related fields from the single hits and 
    /// apply corrections.
    void Initialize();

protected:

    /// The measured "charge" for this hit.
    Float_t fCharge;

    Float_t fChargeUncertainty;

    /// The measured "time" for this hit.
    Float_t fTime;

    /// The reconstructed time uncertainty
    Float_t fTimeUncertainty; 
  
    /// The RMS of the timing.
    Float_t fTimeRMS;

    /// The reconstructed position of the hit in global coordinates.
    TVector3 fPosition;

    /// The uncertainty of the hit position in global coordinates.  The
    /// uncertainty of the hit is in the global coordinates, and is by
    /// definition diagonal.
    TVector3 fUncertainty;

    /// The RMS of the hit position in global coordinates.
    TVector3 fRMS;

    /// The THits that make up this reconstructed hit.
    std::vector< CP::THandle < CP::THit > > fConstituents;

    /// A static constant TMatrixD to hold the rotation matrix for all
    /// TReconHits.  This is by definition the identity matrix.
    static TMatrixD* fRotation;

    ClassDef(TReconHit,1);
};



/// Provide a writable interface to a TReconHit that can be used to fill the
/// object.  All of the THit objects added to a single TReconHit object must
/// be in the same geometry object (e.g. all in the save bar).  The geometry
/// information (i.e. the geometry id, spread, &c) is taken from the first
/// hit.  The other hits are not used by this class, but are checked to make
/// sure that they are in the same geometry volume.  If you need to combine
/// hits from different geometry volumes, use a TReconCluster.
class CP::TWritableReconHit : public TReconHit {
public:
    TWritableReconHit(const TWritableReconHit& h);

    /// A "convenience" constructor for the case where there is one
    /// constituent.  This sets the constituent fields, but the position,
    /// time, &c are not initialized.  These fields and their uncertainties
    /// must be set using SetCharge, SetTime, &c.
    explicit TWritableReconHit(CP::THandle<CP::THit> hit);

    /// A "convenience" constructor for the case where there are two
    /// constituents.  This sets the constituent fields, but the position,
    /// time, &c are not initialized.  These fields and their uncertainties
    /// must be set using SetCharge, SetTime, &c.
    explicit TWritableReconHit(CP::THandle<CP::THit> hit1,
                               CP::THandle<CP::THit> hit2);

    virtual ~TWritableReconHit();
    
    /// Add one more hit to an existing TWritableReconHit.
    void AddHit(CP::THandle<CP::THit> hit);

    /// Set the charge for the hit.
    void SetCharge(double q);

    void SetChargeUncertainty(double unc);

    /// Set the time for the hit.
    void SetTime(double t);

    /// Set the time uncertainty for the hit.
    void SetTimeUncertainty(double tunc);

    /// Set the time RMS for the hit.
    void SetTimeRMS(double rms);

    /// Set the position for the hit in *** GLOBAL *** coordinates.
    /// Internally, this is translated to local coordinates.
    void SetPosition(const TVector3& pos);
  
    /// Set the position uncertainty for the hit in global coordinates.  The
    /// position covariance is diagonal by definition.  For a more complete
    /// representation of the covariance, use a TReconCluster.
    void SetUncertainty(const TVector3& unc);

    /// Set the RMS of the hit in GLOBAL coordinates.  The RMS elements are
    /// the diagonal elements of the charge moments, so this the moments are,
    /// by definition, diagonal.  For a more complete representation of the
    /// charge moments, use a TReconCluster.
    void SetRMS(const TVector3& rms);

    ClassDef(TWritableReconHit,4);
};
#endif
