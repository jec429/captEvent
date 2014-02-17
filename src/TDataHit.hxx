#ifndef TDataHit_hxx_seen
#define TDataHit_hxx_seen

#include <iostream>
#include <vector>

#include "TSingleHit.hxx"
#include "TDigitProxy.hxx"

namespace CP {
    class TDataHit;
    class TWritableDataHit;
}

/// A single calibrated hit detector element.  This corresponds to a real
/// measurement inside the detector, after it has been fully calibrated.  The
/// hit is associated with a particular geometric element inside of the
/// detector.  
///
/// If the MC has written digitized data (derived from TMCDigit), then a
/// TDataHit will be used to hold the calibrated MC THit objects.  You can get
/// associated MC information using the TDigitProxy saved in the hit.
class CP::TDataHit : public TSingleHit {
public:
    TDataHit();
    TDataHit(const TWritableDataHit& val);
    virtual ~TDataHit();

    /// Return a proxy to the digit that generated this hit.  If the index is
    /// out of range, this will throw an EHitOutOfRange exception.
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

protected:
    /// The digit that generated this hit.
    CP::TDigitProxy fProxy;

    /// The electronics channel for this hit.
    UInt_t fChannelId;

    ClassDef(TDataHit,1);
};

/// Provide a writable interface to a TDataHit that can be used to fill the
/// object.
class CP::TWritableDataHit : public TDataHit {
public:
    TWritableDataHit();
    TWritableDataHit(const TWritableDataHit& h);
    virtual ~TWritableDataHit();

    void SetGeomId(TGeometryId id);

    void SetCharge(double q);

    void SetChargeUncertainty(double q);

    void SetTime(double t);

    void SetTimeUncertainty(double t);

    void SetTimeRMS(double t); 

    void SetChargeValidity(bool valid);

    void SetTimeValidity(bool valid);

    /// Set the digit and channel id for this hit.  If the proxy can access a
    /// valid digit, then this also sets the channel id.
    void SetDigit(CP::TDigitProxy proxy);

    /// Explicitly set the channel id (shouldn't be required since also done
    /// by SetDigit).
    void SetChannelId(CP::TChannelId id);

    ClassDef(TWritableDataHit,4);
};
#endif
