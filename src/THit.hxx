#ifndef THit_hxx_seen
#define THit_hxx_seen

#include "TDatum.hxx"
#include "THandle.hxx"
#include "TGeometryId.hxx"
#include "TChannelId.hxx"
#include "TDigitProxy.hxx"

#include "method_deprecated.hxx"

#include <TObject.h>
#include <TVector3.h>
#include <TMatrixD.h>

#include <iostream>
#include <vector>

namespace CP {
    class THit;

    /// Base class for all exceptions associated with the THit classes.
    EXCEPTION(EHit,ECore);

    /// The index of the requested contributor, digit, or channel was out of
    /// range.
    EXCEPTION(EHitOutOfRange,ECore);

}

/// The base class for a hit detector element.  This corresponds to a real
/// measurement inside the detector after it has been fully calibrated.  Hits
/// correspond to a particular geometric element inside of the detector.  In
/// many parts of the detector, a geometric element also corresponds to a
/// particular electronics channel, however, the match to the detector
/// geometry, and not the electroncis channel.
class CP::THit : public TObject {
public:
    THit();
    virtual ~THit();

    /// Return the calibrated "charge" for the hit.
    virtual double GetCharge(void) const = 0;

    /// Return the uncertainty in the charge for the hit.
    virtual double GetChargeUncertainty() const = 0;

    /// Return true if the calibrated charge is valid. 
    virtual bool HasValidCharge(void) const;

    /// Return the calibrated "time" for the hit representing the mean (or
    /// central) time of the hit.
    virtual double GetTime(void) const = 0;

    /// Return the uncertainty on the hit time.
    virtual double GetTimeUncertainty(void) const = 0;

    /// Return the rms of the calibrated time for the hit.  This represents
    /// the extent (or spread) of the time distribution.
    virtual double GetTimeRMS(void) const = 0; 

    /// Return true if the calibrated time is valid.
    virtual bool HasValidTime(void) const;

    /// Return the starting time of the charge integration.  This is the lower
    /// bound of the full width of the hit.  This must be greater than or
    /// equal to GetTimeStart().
    virtual double GetTimeLowerBound(void) const {
        // The default is usually overridden by the daughter class.
        return GetTimeStart();
    }

    /// Return the ending time of the charge integration.  This is the upper
    /// bound of the full width of the hit.  This must be less than or
    /// equal to GetTimeStop().
    virtual double GetTimeUpperBound(void) const {
        // The default is usually overridden by the daughter class.
        return GetTimeStop();
    }
    
    /// Return the time of the start of the first sample contributing to this
    /// hit.
    virtual double GetTimeStart() const {
        // The default is based on the RMS and a factor assuming an almost
        // uniform charge distribution.  This is usually overridden by the
        // daughter class.
        return GetTime()-1.73*GetTimeRMS();
    }

    /// Return the time of the end of the last sample contributing to this
    /// hit.
    virtual double GetTimeStop() const {
        // The default is based on the RMS and a factor assuming an almost
        // uniform charge distribution.  This is usually overridden by the
        // daughter class.
        return GetTime()+1.73*GetTimeRMS();
    }
    
    /// Get the number of samples saved with this hit.
    virtual int GetTimeSamples() const {return 1;}

    /// Get the value of a sample saved with this hit.
    virtual double GetTimeSample(int i) const {return GetCharge();}
    
    /// The center of the volume associated with this hit.
    virtual const TVector3& GetPosition(void) const = 0;

    /// Return the uncertainty of the hit position in the local coordinate
    /// system of the hit (diagonal by definition).
    virtual const TVector3& GetUncertainty(void) const = 0;

    /// Return the rms of the hit position in the local coordinate system of
    /// the hit.  For objects like scintillator bars, or wires, this is the
    /// spacing/sqrt(12).
    virtual const TVector3& GetRMS(void) const = 0;

    /// Get a rotation to transform from the local rotation to the global
    /// coordinates.
    virtual const TMatrixD& GetRotation(void) const = 0;

    /// Get the local X axis.
    virtual TVector3 GetXAxis(void) const;

    /// Get the local Y axis.
    virtual TVector3 GetYAxis(void) const;

    /// Get the local Z axis.
    virtual TVector3 GetZAxis(void) const;

    /// Get the covariance in the global coordinate system.  This is the local
    /// uncertainty (which is diagonal) rotated into the global coordinate
    /// system.  It's provided so that the result can be conveniently cached.
    /// It's calculated on a just-in-time basis.
    virtual const TMatrixD& GetCovariance(void) const;

    /// Get the error matrix in the global coordinate system.  This is the
    /// inverse of the covariance and is provided so that the result can be
    /// conveniently cached.  It's calculated on a just-in-time basis.
    virtual const TMatrixD& GetError(void) const;

    /// Return a hit that has been combined into the current hit.  If the
    /// index is out of range, this will throw an EHitOutOfRange exception.
    /// By default this will throw an EHitOutOfRange, but it may be
    /// over-ridden in a derived class.
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

    /// @{Override methods in the base TObject class.
    virtual void ls(Option_t* opt = "") const; 
    virtual void Print(Option_t* opt = "") const {ls(opt);}
    /// @}

    /// Define the status bits used by the THit object.  These can't collide
    /// with any status bits defined in TObject (the parent class for THit),
    /// and none of the THit children can define a status bit that collides
    /// with these definitions.  Bits 14 to 23 are available for use.
    enum EStatusBits {
      kInvalidTime        = BIT(18),   // if time is not valid.
      kInvalidCharge      = BIT(19),   // if charge is not valid.
    };

protected:
    
    /// Set the validity of the calibrated time.
    virtual void SetTimeValidity(bool valid);

    /// Set the validity of the calibrated charge.
    virtual void SetChargeValidity(bool valid);
    
private:
    /// A cache for the covariance matrix
    mutable TMatrixD fCovariance;  //! Don't Save.

    /// A cache for the error matrix.
    mutable TMatrixD fError;       //! Don't Save.

    ClassDef(THit,1);
};
#endif
