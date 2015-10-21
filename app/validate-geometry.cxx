/// Copy the geometry found in a ROOT file into a second root file.

#include <memory>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <TFile.h>
#include <TGeoManager.h>
#include <TGeoOverlap.h>

#include <TManager.hxx>
#include <TGeomIdManager.hxx>
#include <TSHAHashValue.hxx>

void usage(int argc, char **argv) {
    std::cout << std::endl
              << argv[0] << " [options] <input-file-name>" 
              << std::endl
              << std::endl
              << "   Test a geometry to make sure all geometry objects can"
              << std::endl
              << "   be found."
              << std::endl;
}

int main(int argc, char** argv) {
    std::string outputName;
    int trials = 1;
    for (;;) {
        int c = getopt(argc, argv, "t:h");
        switch (c) {
        case 't': {
            std::istringstream in(optarg);
            in >> trials;
        }
        case 'h':
            usage(argc,argv);
            return 0;
        }
        if (c<0) break;
    }

    if (argc<optind+1) { 
        std::cerr << "ERROR: Missing input file" << std::endl;
        usage(argc,argv);
        return 1;
    }
    
    CP::TManager::Get().SetGeometryOverride(argv[optind++]);

    CP::TManager::Get().Geometry()->CheckOverlaps();
    TIter next(gGeoManager->GetListOfOverlaps());
    int count = 0;
    TGeoOverlap* overlap;
    while ((overlap=(TGeoOverlap*)next())) {
        ++count;
        overlap->PrintInfo();
    }
    if (count > 0) {
        std::cout << "Geometry with overlaps" << std::endl;
        std::cout << "FAIL" << std::endl;
        return 1;
    }

    // Build a vector of all geometry identifiers in the TGeoManager along
    // with the position of the volume associated with the TGeometryId.  Also
    // build a map matching root node id to the geometry id.
    std::vector< std::pair<CP::TGeometryId, TVector3> > allId;
    std::map<int, CP::TGeometryId> rootMap;
    for (CP::TGeomIdManager::GeomIdMap::const_iterator g
             = CP::TManager::Get().GeomId().GetGeomIdMap().begin();
         g != CP::TManager::Get().GeomId().GetGeomIdMap().end();
         ++g) {
        CP::TGeometryId geomId(g->first);
        if (!geomId.IsValid()) {
            std::cout << "Invalid geometry id" << std::endl;
            std::cout << "FAIL" << std::endl;
            return 1;
        }
        if (geomId.GetSubsystemName() == "node") {
            std::cout << "Node geometry id " 
                      << g->first << " " << g->second
                      << std::endl;
            std::cout << "FAIL" << std::endl;
            return 1;
        }
        if (geomId.GetSubsystemName() == "unknown") {
            std::cout << "Unknown geometry id "
                      << g->first << " " << g->second
                      << std::endl;
            std::cout << "FAIL" << std::endl;
            return 1;
        }
        TVector3 pos;
        if (!CP::TManager::Get().GeomId().GetPosition(geomId, pos)) {
            std::cout << "missing geometry id " 
                      << geomId.AsInt()
                      << " " << geomId.GetSubsystemName()
                      << " <" << geomId << ">" << std::endl;
            std::cout << "FAIL" << std::endl;
            return 1;
        }

        allId.push_back(std::pair<CP::TGeometryId,TVector3>(geomId,pos));
        rootMap[g->second] = geomId;
    }

    for (int i = 0; i<trials; ++i) {
        std::random_shuffle(allId.begin(), allId.end());
        for (std::vector< std::pair<CP::TGeometryId, TVector3> >::iterator g
                 = allId.begin();
             g != allId.end();
             ++g) {
            TVector3 pos(g->second);
            CP::TGeometryId target = g->first;

            CP::TGeometryId geomId;
            if (!CP::TManager::Get().GeomId().GetGeometryId(
                    pos.X(), pos.Y(), pos.Z(), geomId)) {
                std::cout << "couldn't find geometry id " 
                          << target.AsInt() << " " << target
                          << std::endl; 
                std::cout << "FAIL" << std::endl;
                return 1;
            }
        }
    }
    
    std::cout << "SUCCESS" << std::endl;
    return 0;
}
