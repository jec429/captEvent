#ifndef TCalibPulseDigit_hxx_seen
#define TCalibPulseDigit_hxx_seen

#include <vector>
#include <TROOT.h>

#include <TChannelId.hxx>
#include <TGeometryId.hxx>
#include <TDigit.hxx>
#include <TDigitProxy.hxx>

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
/// intended to, be saved to a file (e.g. to debug the calibration and for
/// event display).  Notice that TCalibPulseDigit objects are mutable.  This
/// is so they can be modified in the course of the calibration and reduce the
/// amount of copying.
class CP::TCalibPulseDigit : public TDigit {
public:
    typedef std::vector<float> Vector;

    TCalibPulseDigit ();
    virtual ~TCalibPulseDigit();

    /// Construct a digit for a particular channel.  The first time bin is
    /// specified, and then the vector of adcs values for the next set of adcs
    /// need to be provided.
    TCalibPulseDigit(const CP::TDigitProxy& parent, double first, double last,
                     const Vector& samples);

    /// Construct a digit from an existing calibrated pulse digit.  This is
    /// used to allow a progressive calibration chain with the intermediate
    /// results being saved.
    TCalibPulseDigit(const CP::TCalibPulseDigit& cpd);

    /// Return a pointer to the parent digit.
    const CP::TDigitProxy& GetParent() const {return fParent;}

    /// Get the geometry id of the wire for this calibrated digit.  This field
    /// may not always be filled, and should be checked using
    /// TGeometryId::IsValid() before it is used.
    CP::TGeometryId GetGeomId() {return fGeometryId;}

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

    /// The calibrated pulse digit is actually mutable, so this allows the
    /// value to be set.  This helps reduce the amount of copying required in
    /// the calibration.
    void SetSample(int i, double v);

    /// Set the geometry id of the wire.
    void SetGeomId(CP::TGeometryId id) {fGeometryId = id;}

    /// Get the vector of samples.
    const Vector& GetSamples() const;

    /// Print the digit information.
    virtual void ls(Option_t* opt = "") const;
    
private: 

    /// The digit that the calibrated digit came from.  This must be present
    /// since the calibrated digit will lose information otherwise.
    CP::TDigitProxy fParent;

    /// Save the geometry id for reference.
    CP::TGeometryId fGeometryId;

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

    ClassDef(TCalibPulseDigit,2);
};
#endif
