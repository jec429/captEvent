#ifndef TCalibPulseDigit_hxx_seen
#define TCalibPulseDigit_hxx_seen

#include <vector>
#include <TROOT.h>

#include <TChannelId.hxx>
#include <TDigit.hxx>

namespace CP {
    class TCalibPulseDigit;
};

/// A calibrated "digit" for pulse based detectors.  This holds the calibrated
/// data for a channel where an ADC value is read with a fixed frequency.  It
/// can, but is not intended to, be saved to a file
class CP::TCalibPulseDigit : public TDigit {
public:
    typedef std::vector<double> Vector;

    TCalibPulseDigit ();
    virtual ~TCalibPulseDigit();

    /// Construct a digit for a particular channel.  The first time bin is
    /// specified, and then the vector of adcs values for the next set of adcs
    /// need to be provided.
    TCalibPulseDigit(CP::TChannelId chan, double first, const Vector& adcs);

    /// Get the time of the first sample.  This can be negative since some
    /// ADCs may give a delta relative to an index saved in the header.
    double GetFirstSample() const;

    // number of time bins in this digit
    std::size_t GetSampleCount() const;

    /// Get the sample value for a specific time bin
    double GetSample(int t) const;

    /// vector of samples.
    const Vector& GetSamples() const;

    /// Print the digit information.
    virtual void ls(Option_t* opt = "") const;
    
private: 

    /// The index of the first sample in the pulse.  This is signed since some
    /// digitizers may return a delta from a sample number saved in the
    /// header.
    double fFirstSample;    

    /// vector of samples.
    Vector fSamples;

    ClassDef(TCalibPulseDigit,1);
};
#endif
