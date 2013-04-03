////////////////////////////////////////////////////////////
// $Id: TG4TrajectoryPoint.cxx,v 1.9 2011/08/25 21:22:49 mcgrew Exp $
//

#include <TGeoManager.h>
#include <TClass.h>
#include <TStreamerInfo.h>

#include "TOADatabase.hxx"

#include "TG4TrajectoryPoint.hxx"

ClassImp(CP::TG4TrajectoryPoint);

CP::TG4TrajectoryPoint::TG4TrajectoryPoint() 
    : fPositionX(0), fPositionY(0), fPositionZ(0), fPositionT(0),
      fMomentumX(0), fMomentumY(0), fMomentumZ(0), fVolumeNode(0) {}

CP::TG4TrajectoryPoint::~TG4TrajectoryPoint() {}

void CP::TG4TrajectoryPoint::ls(Option_t *opt) const {
    CP::ls_header(this,opt);
    std::cout << " at: (" << fPositionX
              << "," << fPositionY
              << "," << fPositionZ
              << "," << fPositionT
              << ")--> "
              << "(" << fMomentumX
              << "," << fMomentumY
              << "," << fMomentumZ
              << ")"
              << ", node: " << fVolumeNode
              << std::endl;
}

std::string CP::TG4TrajectoryPoint::GetVolumeName(void) const {
    std::string path("none");
    try {
        TGeoManager* geom = CP::TOADatabase::Get().Geometry();
        geom->PushPath();
        geom->CdNode(fVolumeNode);
        path = gGeoManager->GetPath();
        gGeoManager->PopPath();
    }
    catch (...) {
        CaptTrace("Geometry not found");
    }
    return path;
}
