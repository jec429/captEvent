#ifndef TPulseDigit_hxx_seen
#define TPulseDigit_hxx_seen

#include <vector>
#include <TROOT.h>

#include <TChannelId.hxx>
#include <TDigit.hxx>

namespace CP {
    class TPulseDigit;
};

/// Digit for pulse based detectors.  This holds the actual digitization data
/// for a channel where an sample values are read with a fixed frequency.
class CP::TPulseDigit : public TDigit {
public:
    typedef std::vector<unsigned short> Vector;

    TPulseDigit ();
    virtual ~TPulseDigit();

    /// Construct a digit for a particular channel.  The first time bin is
    /// specified, and then the vector of sample values for the next set of adcs
    /// need to be provided.
    TPulseDigit(CP::TChannelId chan, int first, const Vector& adcs);

    /// Get the index of the first sample.  This can be negative since some
    /// Samples may give a delta relative to an index saved in the header.
    int GetFirstSample() const;

    // number of time bins in this digit
    std::size_t GetSampleCount() const;

    /// Get the sample value for a specific time bin
    int GetSample(int t) const;

    /// vector of sample counts
    const Vector& GetSamples() const;

    /// Print the digit information.
    virtual void ls(Option_t* opt = "") const;
    
private: 

    /// The index of the first sample in the pulse.  This is signed since some
    /// digitizers may return a delta from a sample number saved in the
    /// header.
    int fFirstSample;    

    /// vector of Samples 
    Vector fSamples;

    ClassDef(TPulseDigit,1);
};
#endif
