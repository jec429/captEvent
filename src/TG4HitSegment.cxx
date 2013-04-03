#include <TClass.h>
#include <TStreamerInfo.h>

#include "HEPUnits.hxx"
#include "TG4HitSegment.hxx"

ClassImp(CP::TG4HitSegment);

CP::TG4HitSegment::TG4HitSegment()
    : fPrimaryId(0), fEnergyDeposit(0), fTrackLength(0), 
      fStartX(0), fStartY(0), fStartZ(0), fStartT(0),
      fStopX(0), fStopY(0), fStopZ(0), fStopT(0) {}

CP::TG4HitSegment::TG4HitSegment(const CP::TG4HitSegment& rhs)
    : TG4VHit(rhs), 
      fContributors(rhs.fContributors), fPrimaryId(rhs.fPrimaryId),
      fEnergyDeposit(rhs.fEnergyDeposit), fTrackLength(rhs.fTrackLength), 
      fStartX(rhs.fStartX), fStartY(rhs.fStartY),
      fStartZ(rhs.fStartZ), fStartT(rhs.fStartT),
      fStopX(rhs.fStopX), fStopY(rhs.fStopY),
      fStopZ(rhs.fStopZ), fStopT(rhs.fStopT) {}
 

CP::TG4HitSegment::~TG4HitSegment() {}

void CP::TG4HitSegment::ls(Option_t *opt) const {
    CP::ls_header(this,opt);
    std::cout << " Primary Id: " << GetPrimaryId();
    double energy = GetEnergyDeposit();
    int prec = std::cout.precision();
    std::cout.precision(1);
    if (energy > 1*unit::GeV) {
        std::cout << " E: " << GetEnergyDeposit()/unit::GeV << " GeV";
    }
    else if (energy > 1*unit::MeV) {
        std::cout << " E: " << GetEnergyDeposit()/unit::MeV << " MeV";
    }
    else {
        std::cout << " E: " << GetEnergyDeposit()/unit::keV << " keV";
    }
    std::cout << " L: " << GetTrackLength()/unit::mm << " mm" << std::endl;
    std::string option(opt);
    if (option.find("dump") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        TROOT::IndentLevel();
        std::cout << std::setprecision(1)
                  << "Start: (" << fStartX << " cm"
                  << ", " << fStartY << " cm"
                  << ", " << fStartZ << " cm"
                  << std::setprecision(0)
                  << ", " << fStartT << " ns)"
                  << std::endl;
        TROOT::IndentLevel();
        std::cout << std::setprecision(1)
                  << "Stop:  (" << fStopX << " cm"
                  << ", " << fStopY << " cm"
                  << ", " << fStopZ << " cm"
                  << std::setprecision(0)
                  << ", " << fStopT << " ns)"
                  << std::endl;
        TROOT::IndentLevel();
        std::cout << "Traj. Ids:";
        for (std::vector<int>::const_iterator i = GetContributors().begin();
             i != GetContributors().end();
             ++i) std::cout << " " << *i;
        std::cout << std::endl;
        TROOT::DecreaseDirLevel();
    }
    std::cout.precision(prec);
}
