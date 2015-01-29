#ifndef TG4HitSegment_hxx_seen
#define TG4HitSegment_hxx_seen

#include <vector> 

#include "TG4VHit.hxx"

namespace CP {
    class TG4HitSegment;
}

/// DO NOT USE THIS CLASS IN ANALYSIS CODE.
///
/// This is a base class used to save G4Hit objects into a root output file
/// when the detector simulation expects the electronics to be smulated off
/// line.  It contains the global position of the starting point and stopping
/// point of the track segment that created the hit, plus the fields provided
/// by TG4VHit.  The detector simulation expects the electronics response
/// simulation to assign the sensor identifiers to each hit.
///
/// In the simulation, this class is multiply inherited with a G4VHit derived
/// class that can be used in a G4HitCollection.  The behavior is designed so
/// that TG4HitSegment contains all of the payload for the hit, and detector
/// simulation class contains all of the interface related to the G4
/// infrastructure.
class CP::TG4HitSegment: public TG4VHit {
public:
    TG4HitSegment();
    TG4HitSegment(const TG4HitSegment& rhs);
    virtual ~TG4HitSegment();
    
    /// Return a list of track identifiers that contributed to this hit.
    /// These track ids can be used as indicies to find trajectories in the
    /// TG4TrajectoryContainer object associated with an MC event.
    const int GetContributor(int i) const {
        return fContributors[i];
    }

    /// Return a list of track identifiers that contributed to this hit.
    /// These track ids can be used as indicies to find trajectories in the
    /// TG4TrajectoryContainer object associated with an MC event.
    const int GetContributorCount() const {
        return fContributors.size();
    }

    /// Get the TrackId of the "primary" particle that is associated with this
    /// hit.  This is slightly complicated since the "interesting" primary
    /// particle is saved.  For instance, if the primary particle is a pizero,
    /// The TrackId of the gamma-rays from the decay is saved.  Likewise, the
    /// electron from a muon decay is used as the primary particle.  You can
    /// find the "really truly" primary particle by finding the trajectory
    /// associated with this PrimaryId, and then working backwards to the
    /// associated G4PrimaryParticle (You can tell that a trajectory comes
    /// from a primary particle by checking if it's ParentID is zero.  If it
    /// is zero, the trajectory came from a primary).
    int GetPrimaryTrajectoryId(void) const {return fPrimaryId;}

    /// Get the total energy deposited in this hit.
    double GetEnergyDeposit(void) const {return (double) fEnergyDeposit;}
    
    /// Get the secondary energy deposited in this hit (see the field
    /// documentation).
    double GetSecondaryDeposit(void) const {return (double) fSecondaryDeposit;}
    
    /// Get the total charged track length in this hit.  This includes all of
    /// the contributions from secondary particles that got lumped into this
    /// hit (e.g. the contributions from delta-rays).
    double GetTrackLength(void) const {return (double) fTrackLength;}

    /// The X position of the hit starting point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit. 
    double GetStartX(void) const {return fStartX;}

    /// The Y position of the hit starting point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit.
    double GetStartY(void) const {return fStartY;}

    /// The Z position of the hit starting point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit.
    double GetStartZ(void) const {return fStartZ;}

    /// The time of the hit starting point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit.
    double GetStartT(void) const {return fStartT;}

    /// The X position of the hit stoping point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit.
    double GetStopX(void) const {return fStopX;}

    /// The Y position of the hit stoping point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit.
    double GetStopY(void) const {return fStopY;}

    /// The Z position of the hit stoping point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit.
    double GetStopZ(void) const {return fStopZ;}

    /// The time of the hit stopping point.  Note that a hit by
    /// definition is in a single volume.  If the hit is spread over two
    /// volumes, it's a result of round-off error (and is almost a bug).  The
    /// GeoNodeId should be defined by the average position of the hit.
    double GetStopT(void) const {return fStopT;}

    /// Print the hit information.
    virtual void ls(Option_t * = "") const;

protected:
    /// The TrackID for each trajectory that contributed to this hit.  This
    /// could contain the TrackID of the primary particle, but not
    /// necessarily.  
    std::vector<Int_t> fContributors;

    /// The track id of the primary particle.
    Int_t fPrimaryId;

    /// The total energy deposit in this hit.  
    Float_t fEnergyDeposit;

    /// The "secondary" energy deposit in this hit.  This is used to help
    /// simulate the recombination of electrons, and is part of the total
    /// energy deposit.
    ///
    /// ND280MC is expected to use this field to save the amount of energy
    /// deposited as ionization.  The remaining energy will be deposited as
    /// scintillation light.  In this model, the number of electrons created
    /// will be about N_e = fSecondaryDeposit/(ionization energy) while N_ph =
    /// (fEnergyDeposit-fSecondaryDeposit)/(scint energy).
    Float_t fSecondaryDeposit;
    
    /// The total charged track length in this hit.  This includes the
    /// contribution from all of the secondary particles (e.g. delta-rays)
    /// that are included in this hit.
    Float_t fTrackLength;

    /// The X position of the hit starting point.
    Float_t fStartX;

    /// The Y position of the hit starting point.
    Float_t fStartY;

    /// The Z position of the hit starting point.
    Float_t fStartZ;

    /// The time of the hit starting point.
    Float_t fStartT;

    /// The X position of the hit stopping point.
    Float_t fStopX;

    /// The Y position of the hit stopping point.
    Float_t fStopY;

    /// The Z position of the hit stopping point.
    Float_t fStopZ;

    /// The time of the hit stopping point.
    Float_t fStopT;

    ClassDef(TG4HitSegment,1);
};
#endif
