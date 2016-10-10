#ifndef TG4TrajectoryPoint_hxx_seen
#define TG4TrajectoryPoint_hxx_seen

#include <string>
#include <vector>

#include <TObject.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include "TDatum.hxx"

namespace CP {
    class TG4TrajectoryPoint;
}

/// A class to save a G4 trajectory point into a root output file without
/// linking to geant.  The trajectory point is saved in a TG4Trajectory as a
/// way to record the path of a particle through the detector.  This is the
/// truth information about the particles which were tracked.
class CP::TG4TrajectoryPoint: public TObject {
public:
    TG4TrajectoryPoint();
    virtual ~TG4TrajectoryPoint();

    /// Get the position of the trajectory point.
    const TLorentzVector GetPosition(void) const {
        return TLorentzVector(fPositionX, fPositionY, fPositionZ, fPositionT);
    }

    /// Set the position of the trajectory point.
    void SetPosition(double x, double y, double z, double t) {
        fPositionX = x;
        fPositionY = y;
        fPositionZ = z;
        fPositionT = t;
    }
    
    /// Get the momentum of the particle at the trajectory point.
    const TVector3 GetMomentum(void) const {
        return TVector3(fMomentumX, fMomentumY, fMomentumZ);
    }
    
    /// Set the momentum of the particle at this trajectory point.
    void SetMomentum(double px, double py, double pz) {
        fMomentumX = px;
        fMomentumY = py;
        fMomentumZ = pz;
    }

    /// Get the name of the physical volume for the trajectory point.
    std::string GetVolumeName(void) const;

    /// Set the ROOT node id for this trajectory point
    void SetVolumeNode(int id) {fVolumeNode = id;}

    /// Get the ROOT geometry node id for this track point.  This can be used
    /// to identify which volume is being hit using the
    /// TGeomInfo::Get().VolumeName() method.
    int GetVolumeNode(void) const {
        return fVolumeNode;
    }

    /// Set the G4 process type and and subtype.
    void SetProcessType(int type, int subtype) {
        fProcessType = type + 1000*subtype;
    }

    /// Get the G4ProcessType (as an integer).
    int GetProcessType() const {return fProcessType % 1000;}

    /// Get the G4 process subtype (as an integer).
    int GetProcessSubtype() const {return fProcessType / 1000;}

    /// Print the hit information.
    virtual void ls(Option_t * = "") const;

private:
    /// @{ The position of this trajectory point.
    float fPositionX;
    float fPositionY;
    float fPositionZ;
    float fPositionT;
    /// @}

    /// @{ The momentum of the particle at this trajectory point.
    float fMomentumX;
    float fMomentumY;
    float fMomentumZ;
    /// @}

    /// The ROOT geometry node id for this track step.  This can be used to
    /// identify which volume is being hit using the
    /// TGeomInfo::Get().VolumeName() method from the oaUtility library.
    Int_t fVolumeNode;

    /// The interaction process type associated with this trajectory point.
    /// The value is a combination of the process type and the process
    /// subtype [the type is (fProcessType%1000), the subtype is
    /// (fProcessType/1000)].  It's filled with G4ProcessType +
    /// 1000*G4ProcessSubType.
    UInt_t fProcessType;

    ClassDef(TG4TrajectoryPoint,6);
};
#endif
