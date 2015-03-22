#include <TClass.h>
#include <TStreamerInfo.h>

#include "HEPUnits.hxx"
#include "TG4HitSegment.hxx"
#include "TUnitsTable.hxx"

ClassImp(CP::TG4HitSegment);

CP::TG4HitSegment::TG4HitSegment()
    : fPrimaryId(0), fEnergyDeposit(0), fSecondaryDeposit(0), fTrackLength(0), 
      fStartX(0), fStartY(0), fStartZ(0), fStartT(0),
      fStopX(0), fStopY(0), fStopZ(0), fStopT(0) {}

CP::TG4HitSegment::TG4HitSegment(const CP::TG4HitSegment& rhs)
    : TG4VHit(rhs), 
      fContributors(rhs.fContributors), fPrimaryId(rhs.fPrimaryId),
      fEnergyDeposit(rhs.fEnergyDeposit),
      fSecondaryDeposit(rhs.fSecondaryDeposit),
      fTrackLength(rhs.fTrackLength), 
      fStartX(rhs.fStartX), fStartY(rhs.fStartY),
      fStartZ(rhs.fStartZ), fStartT(rhs.fStartT),
      fStopX(rhs.fStopX), fStopY(rhs.fStopY),
      fStopZ(rhs.fStopZ), fStopT(rhs.fStopT) {}
 

CP::TG4HitSegment::~TG4HitSegment() {}

void CP::TG4HitSegment::ls(Option_t *opt) const {
    CP::ls_header(this,opt);
    std::cout << " Primary Id: " << GetPrimaryTrajectoryId() << std::endl;
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << " Energy Deposit: "
              << unit::AsString(GetEnergyDeposit(),"energy");
    if (GetSecondaryDeposit() > 0) {
        std::cout << " ("
                  << unit::AsString(GetSecondaryDeposit(),"energy")
                  << ")";
    }
    std::cout << " Length: " << unit::AsString(GetTrackLength(),"length") 
              << std::endl;
    std::string option(opt);
    if (option.find("dump") != std::string::npos) {
        TROOT::IncreaseDirLevel();
        TROOT::IndentLevel();
        std::cout << "Start: (" << unit::AsString(fStartX,"length")
                  << ", " << unit::AsString(fStartY,"length")
                  << ", " << unit::AsString(fStartZ,"length")
                  << ", " << unit::AsString(fStartT,"time") << ")"
                  << std::endl;
        TROOT::IndentLevel();
        std::cout << "Stop: (" << unit::AsString(fStopX,"length")
                  << ", " << unit::AsString(fStopY,"length")
                  << ", " << unit::AsString(fStopZ,"length")
                  << ", " << unit::AsString(fStopT,"time") << ")"
                  << std::endl;
        TROOT::IndentLevel();
        std::cout << "Traj. Ids:";
        for (std::vector<int>::const_iterator i = fContributors.begin();
             i != fContributors.end();
             ++i) std::cout << " " << *i;
        std::cout << std::endl;
        TROOT::DecreaseDirLevel();
    }
    TROOT::DecreaseDirLevel();
}
