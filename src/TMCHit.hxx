#ifndef TMCHit_hxx_seen
#define TMCHit_hxx_seen

#include <iostream>
#include <vector>

#include "TSingleHit.hxx"

namespace CP {
    class TG4VHit;
    class TMCHit;
    class TWritableMCHit;
}

/// A single calibrated hit detector element from the MC.  This corresponds to
/// a real measurement inside the detector, but after it has been fully
/// calibrated.
class CP::TMCHit : public TSingleHit {
public:
    typedef std::vector<TG4VHit*> TruthContainer;

    TMCHit();
    TMCHit(const TWritableMCHit& val);
    virtual ~TMCHit();

    /// Return the MC information for the raw MC hits that contributed to this
    /// hit.
    virtual const TruthContainer& GetTruth() const {
        return fTruth;
    }

    /// Print the hit information.
    virtual void ls(Option_t *opt = "") const;

protected:
    /// The G4HitBase objects that contributed to this hit.
    TruthContainer fTruth;

    ClassDef(TMCHit,1);
};

/// Provide a writable derived class used to create a TMCHit object.
class CP::TWritableMCHit : public TMCHit {
public:
    TWritableMCHit();
    TWritableMCHit(const TWritableMCHit& h);
    virtual ~TWritableMCHit();

    void SetGeomId(TGeometryId id);

    void SetCharge(double q);

    void SetTime(double t);

    void SetChargeValidity(bool valid);

    void SetTimeValidity(bool valid);

    /// Return non constant version of the MC information for the raw MC hits
    /// that contributed to this hit.
    virtual TruthContainer& GetTruth() {
        return fTruth;
    }

    /// Return the MC information for the raw MC hits that contributed to this
    /// hit.
    virtual const TruthContainer& GetTruth() const {
        return fTruth;
    }

    ClassDef(TWritableMCHit,1);
};
#endif
