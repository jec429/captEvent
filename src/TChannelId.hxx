#ifndef TChannelId_hxx_seen
#define TChannelId_hxx_seen

#include <Rtypes.h>
#include <string>
#include <ostream>

#include "ECore.hxx"
#include "method_deprecated.hxx"

namespace CP {
    /// Base class for all exceptions associated with the TGeometryId classe.
    EXCEPTION(EChannelId,ECore);

    class TChannelId;

    std::ostream& operator<<(std::ostream& s, const CP::TChannelId& id);
};

/// Base class for channel identifiers.  This can answer simple interrogations
/// about channel, and can preform all of the basic conversions.  The channel
/// identifiers are designed so that the data fit into a single 32 bit field
/// (the same as a TGeometryId.  Channel identifiers and geometry identifiers
/// can be distinguished by the guard bit which is 1 for a TChannelId and 0
/// for a TGeometryId.
class CP::TChannelId {
public:
    /// Enumeration defining the sub-detector identifiers.  These tell you
    /// which sub-detector a channel is associated with.
    enum SubDetId {
        /// This isn't really a subdetector, but flags that the channel id is
        /// from the MC.  The resulting identifier should be cast as a
        /// TMCChannelId, which can then be easily translated to (from) a
        /// TGeometryId.  This is needed since the electronics configuration
        /// hasn't been specified for the detector.
        kMC     = 1,  
        /// Flags that the channel id is from the TPC wire planes.
        kTPC    = 2, 
        kMaxDetector,
    }; 

    /// Construct a channel id from a raw UInt_t.
    explicit TChannelId(UInt_t id = 0);

    /// Copy constructor
    TChannelId(const CP::TChannelId& src);

    virtual ~TChannelId();

    /// Get the sub-detector code for this channel
    virtual const SubDetId GetSubDetector() const;

    /// Check that this is a valid channel id, i.e. that the MSB is 1 and the
    /// sub-detector field is in range.
    virtual const Bool_t IsValid() const;

    /// Get the internal UInt_t representation.
    virtual const UInt_t AsInt() const {return fChannelId;} 

    /// Get the internal UInt_t representation
    virtual const UInt_t AsUInt() const {return fChannelId;} 
  
    /// Format (as much as is possible) as a human readable string
    virtual std::string AsString() const; 

    ///Check if this channel is from the MC.
    virtual const bool IsMCChannel() const; 

    ///Check if this channel is from the TPC.
    virtual const bool IsTPCChannel() const; 

    ///Check if a UInt_t is an MC channel id, without the user having to
    ///construct a channel id.
    static const bool IsMCChannel(UInt_t code) {
        return TChannelId(code).IsMCChannel();
    }

    ///Check if a UInt_t is a TPC channel id, without the user having to
    ///construct a channel id.
    static const bool IsTPCChannel(UInt_t code) {
        return TChannelId(code).IsTPCChannel();
    }

    /// Automatic conversion to char pointer (use with care).  This simplifies
    /// output to an i/o stream, but should be used with *great* caution.  The
    /// pointer to char actually points to a buffer generated by
    /// std::string::c_str() and is only valid until the next call to a
    /// non-constant string method (that includes a string distructor).
    operator const char * () const {return AsString().c_str();} 

    /// Print the detector this channel is associated with as a human readable
    /// string
    virtual std::string SubDetAsString() const;

    /// Equal operator
    virtual bool operator== (const TChannelId& rhs) const {
        return AsUInt() == rhs.AsUInt();
    }

    /// Not equal operator
    virtual bool operator!= (const TChannelId& rhs) const {
        return AsUInt() != rhs.AsUInt();
    }

    /// Less-than operator
    virtual bool operator< (const TChannelId& rhs) const {
        return AsUInt() < rhs.AsUInt();
    }

    /// Less-than-or-equal operator
    virtual bool operator<= (const TChannelId& rhs) const {
        return AsUInt() <= rhs.AsUInt();
    }

    /// Greater-than operator
    virtual bool operator> (const TChannelId& rhs) const {
        return AsUInt() > rhs.AsUInt();
    }

    /// Greater-than-or-equal operator
    virtual bool operator>= (const TChannelId& rhs) const {
        return AsUInt() >= rhs.AsUInt();
    }

protected:
    /// Get a bit field out of the channel identifier.
    unsigned int GetField(int msb, int lsb) const;

    /// Set a bit field in the channel identifier.
    void SetField(int val, int msb, int lsb);

    /// Set the guard bit for this id.  This must be done explicitly in a
    /// derived class.
    void SetGuardBit();

    /// Set the sub-detector code for this channel
    virtual void SetSubDetector(int det);

    /// Define the bit fields for the channel id guard bit and sub-detector
    /// fields.  The channel id is a 32 bit field with bits number from 0
    /// (lsb) to 31 (msb).  The sub-fields are defined
    ///
    /// g dddddd xxxxxxxxxxxxxxxxxxxxxxxx
    ///
    ///    - g(1) Bit 31:  A "guard bit" for the channel identifier.  This
    ///            must be 1. 
    ///    - d(6) Bits 25-30: A six bit sub-detector identifier.
    ///    - x(25) Bits 0-24: A 25 bit field reserved to the TChannelId
    ///            sub-classes. 
    ///
    /// Note: The shift and the least significant bit (lsb) must be the same.
    enum kBitDefinitions {
        kGuardBitMSB = 31,    kGuardBitLSB = 31,
        kSubDetMSB   = 30,    kSubDetLSB   = 25,
        kSubClassMSB = 24,    kSubClassLSB = 0
    };

private:
    UInt_t fChannelId;

#define MAKE_MASK(msb,lsb) (((1<<(((msb)-(lsb))+1))-1)<<(lsb))
    enum BitMask {kGuard_Mask=MAKE_MASK(kGuardBitMSB, kGuardBitLSB)};
#undef MAKE_MASK

    ClassDef(TChannelId, 1)
};

#endif
