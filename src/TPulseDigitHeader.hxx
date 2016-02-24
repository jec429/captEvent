#ifndef TPulseDigitHeader_hxx_seen
#define TPulseDigitHeader_hxx_seen

#include "TDigitHeader.hxx"
#include "nanoStamp.hxx"

namespace CP {
    class TPulseDigitHeader;
};

class CP::TPulseDigitHeader : public CP::TDigitHeader {
public:
    TPulseDigitHeader();
    explicit TPulseDigitHeader(const std::string& name);
    virtual ~TPulseDigitHeader();

    // Set the time stamp for the digits.  This handles the case where the
    // digits may have different trigger times in an event. (e.g. they come
    // from different DAQs).
    void SetTimeStamp(CP::NanoStamp ns) {fTimeStamp = ns;}

    /// Get the time stamp for the digits.
    CP::NanoStamp GetTimeStamp() const {return fTimeStamp;}
    
    /// Print the header information.
    virtual void ls(Option_t* opt = "") const;

private:
    
    /// The time stamp for these digits.
    CP::NanoStamp fTimeStamp;
    
    ClassDef(TPulseDigitHeader, 2);
};
#endif
