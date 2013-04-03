////////////////////////////////////////////////////////////
// $Id: TG4Trajectory.cxx,v 1.17 2011/09/12 13:43:38 mcgrew Exp $
//
#include <TClass.h>
#include <TStreamerInfo.h>
#include <TDatabasePDG.h>

#include "TG4Trajectory.hxx"

ClassImp(CP::TG4Trajectory);
ClassImp(CP::TG4TrajectoryContainer);

CP::TG4Trajectory::TG4Trajectory() 
    : fTrackId(0), fParentId(0), 
      fParticleName("none"), fPDGEncoding(0),
      fInitialMomentumX(0), fInitialMomentumY(0), 
      fInitialMomentumZ(0), fInitialMomentumT(0) {}

CP::TG4Trajectory::~TG4Trajectory() { }

const TParticlePDG* CP::TG4Trajectory::GetParticle() const {
    return TDatabasePDG::Instance()->GetParticle(fPDGEncoding);
}

void CP::TG4Trajectory::ls(Option_t* opt) const {
    CP::ls_header(this,opt);
    int prec = std::cout.precision();
    std::cout.precision(1);
    TLorentzVector mom = GetInitialMomentum();
    std::cout << " Id: " << fTrackId
              << " " << fPDGEncoding << "(" << fParticleName << ")"
              << " Points: " << fPositions.size()
              << " E: " << mom.E()
              << " P: (" << mom.Px()
              << ", " << mom.Py()
              << ", " << mom.Pz() << ")"
              << " from: " << fParentId
              << std::endl;
    std::string option(opt);
    if (option.find("dump") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        for (Points::const_iterator v 
                 = fPositions.begin();
             v != fPositions.end(); 
             ++v) {
            std::cout.precision(prec);
            v->ls(opt);
        };
        TROOT::DecreaseDirLevel();
    }
    std::cout.precision(prec);
}

int CP::TG4TrajectoryContainer::GetPrimaryId(int id) const {
    TG4TrajectoryContainer::const_iterator particle = find(id);

    // The particle doesn't exist in the trajectory container.
    if (particle == end()) return -1;
    
    for (;;) {
        int parentId = particle->second.GetParentId();
        if (parentId == 0) return particle->first;
        particle = find(parentId);
        if (particle == end()) return -1;
    }

    return -1;
}

CP::THandle<CP::TG4Trajectory>
CP::TG4TrajectoryContainer::GetTrajectory(int id) const {
    TG4TrajectoryContainer::const_iterator particle = find(id);
    
    // The particle doesn't exist in the trajectory container.
    if (particle == end()) return THandle<CP::TG4Trajectory>(NULL);
    
    CP::TG4Trajectory* val = const_cast<CP::TG4Trajectory*>(&(particle->second));
    return THandle<CP::TG4Trajectory>(val,false);
}

void CP::TG4TrajectoryContainer::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    TROOT::IncreaseDirLevel();
    for (const_iterator v = begin();
         v != end(); 
         ++v) {
        v->second.ls(opt);
    };
    TROOT::DecreaseDirLevel();
}
