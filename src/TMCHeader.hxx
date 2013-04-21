#ifndef TMCHeader_hxx_seen
#define TMCHeader_hxx_seen

#include "TDataVector.hxx"
#include "THandle.hxx"
#include "TSHAHashValue.hxx"

namespace CP {
    class TMCHeader;
};

/// Save information about how the MC was run.  This saves information that
/// might be thought of as "MC Global Slow Control".
class CP::TMCHeader: public CP::TDataVector {
public:
    TMCHeader();
    TMCHeader(const char* name, const char* title);
    virtual ~TMCHeader();

    /// Set "Intensity" for the event.  The meaning is defined by the usage,
    /// but for a beam spill MC, this should be the protons per pulse.
    void SetIntensity(double val) {fIntensity = val;}

    /// Get "Intensity" for the event.
    double GetIntensity() const {return fIntensity;}

    /// Get the SHA hash value for the geometry used to generate the event.
    const TSHAHashValue& GetGeometryHash() const {return fGeometryHash;}

    /// Set the hash value for the geometry.
    void SetGeometryHash(const TSHAHashValue& h) {fGeometryHash = h;}
    
    /// Print the object information.
    virtual void ls(Option_t *opt = "") const;

private:

    /// Do the class initialization once.  This is used in the constructors.
    void Init();

    /// The intensity used to generate this event.  The definition of the
    /// intensity can depend on the MC run type, but for a beam spill run, it
    /// will contain the number of protons per pulse. 
    float fIntensity;

    /// The hash value for the geometry that generated the event.
    TSHAHashValue fGeometryHash;

    ClassDef(TMCHeader,1);
};

#endif
