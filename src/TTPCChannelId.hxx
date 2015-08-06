#ifndef TTPCChannelId_hxx_seen
#define TTPCChannelId_hxx_seen

#include "method_deprecated.hxx"
#include "TChannelId.hxx"

namespace CP {
    class TTPCChannelId;
};

/// A TChannelId derived class for signals from the TPC wires.  The channel
/// identifier holds the information about how the DAQ views the electronics.
/// For the TPC, the channel is identified by the crate, the slot in the
/// crate, and the channel in the slot.  For the TPC, all of the slots hold
/// front end modules (FEMs), so the methods refer to FEMs.
class CP::TTPCChannelId : public CP::TChannelId {
public:
    /// Construct a TPC channel ID from a raw UInt_t
    explicit TTPCChannelId(UInt_t id = 0);

    /// Constructor to specialise a Channel ID as a TPC channel ID
    TTPCChannelId(const CP::TChannelId& src);

    /// Construct a TPC channel ID from its electronics-space co-ordinates.
    /// The sub-detector identifier is CP::TChannelId::kTPC(=2).
    TTPCChannelId(UInt_t crate, UInt_t fem, UInt_t channel);

    virtual ~TTPCChannelId();

    ///Format as a human readable string.
    virtual std::string AsString() const; 

    /// Get the crate number for this channel [0-2].
    virtual const UInt_t GetCrate() const;

    /// Get the FEM number in the crate [0-31].  The crates can hold up to 20
    /// cards.
    virtual const UInt_t GetFEM() const;
    
    /// Get the channel on the FEM [0-63].
    virtual const UInt_t GetChannel() const;

protected:
    /// @{ Define the sub-detector specific part of the channel id.  The
    /// sub-detector specific part of the channel id is a 25 bit field with
    /// bits number from 0 (lsb) to 24 (msb).  The sub-fields are defined
    ///
    /// R RRRR RRRR CCCC ffff fccc cccc
    ///
    ///    - R(4) Bits 16-24: Reserved bits.  These must be set to zero.
    ///    - C(4) Bits 12-15: The FEM index field (Nominally 2 Crates)
    ///    - f(5) Bits 7-11: The FEM index field (<18 FEM/Crate)
    ///    - c(7) Bits 0-6: The channel on the ASIC (64 channels/FEMC)
    /// 
    /// Note: The shift and the least significant bit (lsb) must be the same.
    enum BitDefinitions {
        kReserved1MSB=24, kReserved1LSB=16,
        kCrateMSB=15, kCrateLSB=12,
        kFEMMSB=11, kFEMLSB=7,
        kChanMSB=6, kChanLSB=0
    };
    /// @}

private:
    /// Get the crate number for this channel.
    virtual void SetCrate(int val);

    /// Set the FEM number for this channel.
    virtual void SetFEM(int val);

    /// Set the channel on the ASIC.
    virtual void SetChannel(int val);

    ClassDef(TTPCChannelId, 1)
};
#endif //TTPCChannelId_hxx_seen
