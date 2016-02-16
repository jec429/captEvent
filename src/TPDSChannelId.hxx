#ifndef TPDSChannelId_hxx_seen
#define TPDSChannelId_hxx_seen

#include "method_deprecated.hxx"
#include "TChannelId.hxx"

namespace CP {
    class TPDSChannelId;
};

/// A TChannelId derived class for signals from the photon detection system
/// (PDS).  The channel identifier holds the information about how the DAQ
/// views the electronics.  For the PDS, the channel is identified by the
/// crate, the digitizer in the crate, and the channel in the slot.
class CP::TPDSChannelId : public CP::TChannelId {
public:
    /// Construct a PDS channel id from a raw UInt_t
    explicit TPDSChannelId(UInt_t id = 0);

    /// Constructor to specialise a channel id as a PDS channel id
    TPDSChannelId(const CP::TChannelId& src);

    /// Construct a PDS channel id from its electronics-space co-ordinates.
    /// The sub-detector identifier is CP::TChannelId::kPDS(=3).
    TPDSChannelId(UInt_t crate, UInt_t digitizer, UInt_t channel);

    virtual ~TPDSChannelId();

    /// Format as a human readable string.
    virtual std::string AsString() const; 

    /// Get the crate number for this channel [0-15].
    virtual const UInt_t GetCrate() const;

    /// Get the digitizer number in the crate [0-31].  The crates can hold up
    /// to 20 cards.
    virtual const UInt_t GetDigitizer() const;
    
    /// Get the channel on the digitizer [0-63].
    virtual const UInt_t GetChannel() const;

protected:
    /// @{ Define the sub-detector specific part of the channel id.  The
    /// sub-detector specific part of the channel id is a 25 bit field with
    /// bits number from 0 (lsb) to 24 (msb).  The sub-fields are defined
    ///
    /// R RRRR RRRR CCCC ffff fccc cccc
    ///
    ///    - R(4) Bits 16-24: Reserved bits.  These must be set to zero.
    ///    - C(4) Bits 12-15: The crate index field (Nominally 1 Crates)
    ///    - f(5) Bits 7-11: The digitizer index field (<18 digitzers/Crate)
    ///    - c(7) Bits 0-6: The channel on the digitizer (<64 chan/digitizer)
    /// 
    /// Note: The shift and the least significant bit (lsb) must be the same.
    enum BitDefinitions {
        kReserved1MSB=24, kReserved1LSB=16,
        kCrateMSB=15, kCrateLSB=12,
        kDigitizerMSB=11, kDigitizerLSB=7,
        kChanMSB=6, kChanLSB=0
    };
    /// @}

private:
    /// Get the crate number for this channel.
    virtual void SetCrate(int val);

    /// Set the digitizer number for this channel.
    virtual void SetDigitizer(int val);

    /// Set the channel on the ASIC.
    virtual void SetChannel(int val);

    ClassDef(TPDSChannelId, 1)
};
#endif //TPDSChannelId_hxx_seen
