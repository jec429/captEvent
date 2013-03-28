#include "TGeoManager.h"
#include "TVector3.h"

#include "TOADatabase.hxx"
#include "TGeomIdManager.hxx"
#include "ND280GeomId.hxx"

#include <nd280EventLoop.hxx>

class TTestGeomId: public ND::TND280EventLoopFunction {
public:
    TTestGeomId() {}

    virtual ~TTestGeomId() {};

    void Usage(void) {
        std::cout << "Test the assignment of geometry identifiers."
                  << std::endl;
    }

    void BeginFile(ND::TVInputFile *const) {
        std::cout << "Get the geometry out of the file" << std::endl;
        ND::TOADatabase::Get().Geometry();

        ND::TGeomIdManager& geomId = ND::TOADatabase::Get().GeomId();

        std::cout << "Path to P0D" << std::endl;
        geomId.CdId(ND::GeomId::P0D::Detector());
        std::cout << gGeoManager->GetPath() << std::endl;
 
        std::cout << "Path to P0D y bar" << std::endl;
        geomId.CdId(ND::GeomId::P0D::Bar(20,1,50));
        std::cout << gGeoManager->GetPath() << std::endl;

        std::cout << "Path to TPC1" << std::endl;
        geomId.CdId(ND::GeomId::TPC::TPC1());
        std::cout << gGeoManager->GetPath() << std::endl;

        std::cout << "Path to TPC mm" << std::endl;
        geomId.CdId(ND::GeomId::TPC::MicroMega(1,0,5));
        std::cout << gGeoManager->GetPath() << std::endl;

        std::cout << "Path to TPC pad (same as mm)" << std::endl;
        if (geomId.CdId(ND::GeomId::TPC::Pad(1,0,5,123))) 
            std::cout << gGeoManager->GetPath() << std::endl;
        else 
            std::cout << "not found" << std::endl;

        TVector3 position;
        ND::TGeometryId id;

        std::cout << "Check the pad positions." << std::endl;

        ND::TGeometryId check = ND::GeomId::TPC::Pad(1,0,5,123);
        std::cout << check.GetName() << std::endl;
        std::cout << "ID: " << check << std::endl;
        geomId.GetPosition(check,position);
        std::cout << " (" << position.X() 
                  << ", " << position.Y() 
                  << ", " << position.Z() << ")" << std::endl;

        if (geomId.GetGeometryId(position.X(),position.Y(),position.Z(),id)) {
            if (id == check) {
            std::cout << id.GetName() << std::endl;
                std::cout << "Pad found at position " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
            else {
                std::cout << "Pad id mismatch " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
        }
        else {
            std::cout << "Pad Not Found " 
                      << check.AsInt() << std::endl;
        }

        check = ND::GeomId::TPC::Pad(1,0,5,124);
        std::cout << check.GetName() << std::endl;
        std::cout << "ID: " << check << std::endl;
        geomId.GetPosition(check,position);
        std::cout << " (" << position.X() 
                  << ", " << position.Y() 
                  << ", " << position.Z() << ")" << std::endl;

        if (geomId.GetGeometryId(position.X(),position.Y(),position.Z(),id)) {
            std::cout << id.GetName() << std::endl;
            if (id == check) {
                std::cout << "Pad found at position " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
            else {
                std::cout << "Pad id mismatch " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
        }
        else {
            std::cout << "Pad Not Found " 
                      << check.AsInt() << std::endl;
        }

        check = ND::GeomId::TPC::Pad(1,1,5,123);
        std::cout << check.GetName() << std::endl;
        std::cout << "ID: " << check << std::endl;
        geomId.GetPosition(check,position);
        std::cout << " (" << position.X() 
                  << ", " << position.Y() 
                  << ", " << position.Z() << ")" << std::endl;

        if (geomId.GetGeometryId(position.X(),position.Y(),position.Z(),id)) {
            std::cout << id.GetName() << std::endl;
            if (id == check) {
                std::cout << "Pad found at position " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
            else {
                std::cout << "Pad id mismatch " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
        }
        else {
            std::cout << "Pad Not Found " 
                      << check.AsInt() << std::endl;
        }

        check = ND::GeomId::TPC::Pad(1,1,5,124);
        std::cout << check.GetName() << std::endl;
        std::cout << "ID: " << check << std::endl;
        geomId.GetPosition(check,position);
        std::cout << " (" << position.X() 
                  << ", " << position.Y() 
                  << ", " << position.Z() << ")" << std::endl;

        if (geomId.GetGeometryId(position.X(),position.Y(),position.Z(),id)) {
            std::cout << id.GetName() << std::endl;
            if (id == check) {
                std::cout << "Pad found at position " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
            else {
                std::cout << "Pad id mismatch " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
        }
        else {
            std::cout << "Pad Not Found " 
                      << check.AsInt() << std::endl;
        }

        check = ND::GeomId::SMRD::Bar(0,1,1,3,2);
        std::cout << check.GetName() << std::endl;
        std::cout << "ID: " << check << std::endl;
        geomId.GetPosition(check,position);
        std::cout << " (" << position.X() 
                  << ", " << position.Y() 
                  << ", " << position.Z() << ")" << std::endl;

        if (geomId.GetGeometryId(position.X(),position.Y(),position.Z(),id)) {
            std::cout << id.GetName() << std::endl;
            if (id == check) {
                std::cout << "SMRD bar found at position " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
            else {
                std::cout << "Bar id mismatch " 
                          << id.AsInt() 
                          << " " << check.AsInt() << std::endl;
            }
        }
        else {
            std::cout << "Bar Not Found " 
                      << check.AsInt() << std::endl;
        }
    }

    bool operator () (ND::TND280Event& event) {
        return false;
    }
};

int main(int argc, char **argv) {
    TTestGeomId userCode;
    nd280EventLoop(argc,argv,userCode,1);
}

