#ifndef TCalibPulseDigit_hxx_seen
#define TCalibPulseDigit_hxx_seen

#include <vector>
#include <TROOT.h>

#include <TChannelId.hxx>
#include <TDigit.hxx>

namespace CP {
    class TCalibPulseDigit;
    class TPulseDigit;
};

/// A calibrated "digit" for pulse based detectors.  This holds the calibrated
/// data for a channel where a sample value is read with a fixed frequency.
/// Unlike most TDigit derived classes, this holds the data in "floating
/// point" variables.  The samples are held in a vector of floats.  The start
/// and stop times are held in doubles.  It also holds a pointer to the
/// original (uncalibrated) CP::TPulseDigit object.  While this class is
/// intended to hold intermediate, temporary results, it can, but is not
/// intended to, be saved to a file.
class CP::TCalibPulseDigit : public TDigit {
public:
    typedef std::vector<float> Vector;

    TCalibPulseDigit ();
    virtual ~TCalibPulseDigit();

    /// Construct a digit for a particular channel.  The first time bin is
    /// specified, and then the vector of adcs values for the next set of adcs
    /// need to be provided.
    TCalibPulseDigit(const CP::TPulseDigit* parent, double first, double last,
                     const Vector& samples);

    /// Return a pointer to the uncalibrated parent TPulseDigit.
    const CP::TPulseDigit* GetParent() const {return fParent;}

    /// Get the time of the first sample.  This can be negative since some
    /// electronics may give a delta relative to an index saved in the header.
    /// It is the time of the start of the first sample, and is saved in
    /// HEPUnits (i.e. nanoseconds).
    double GetFirstSample() const;
    
    /// Get the time of the last sample.  It is the time of the end of the
    /// last sample and is saved in HEPUnits (i.e. nanoseconds)
    double GetLastSample() const;

    /// Get the number of samples in this digit.  The time step for this digit
    /// is (GetLastSample() - GetFirstSample())/GetSampleCount()
    std::size_t GetSampleCount() const;

    /// Get the sample value for a specific time bin.  The calibrated value
    /// depends on the details of the calibrator, but conceptually, this
    /// returns the value of the sample in HEPUnits (i.e. number of positron
    /// charges).
    double GetSample(int t) const;

    /// Get the vector of samples.
    const Vector& GetSamples() const;

    /// Print the digit information.
    virtual void ls(Option_t* opt = "") const;
    
private: 

    /// The digit that the calibrated digit came from.  This must be present
    /// since the calibrated digit will lose information otherwise.
    const CP::TPulseDigit* fParent;

    /// The time of the first sample in the pulse.  This is signed since some
    /// digitizers may return a delta from a sample number saved in the
    /// header.
    double fFirstSample;    

    /// The time of the first sample in the pulse.  This is signed since some
    /// digitizers may return a delta from a sample number saved in the
    /// header.
    double fLastSample;    

    /// The vector of samples.
    Vector fSamples;

    ClassDef(TCalibPulseDigit,1);
};
#endif
