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

#include <TOADatabase.hxx>
#include <TGeomIdManager.hxx>
#include <TSHAHashValue.hxx>

void usage(int argc, char **argv) {
    std::cout << std::endl
              << argv[0] << " [options] <intput-file-name>" 
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
    
    CP::TOADatabase::Get().SetGeometryOverride(argv[optind++]);

    CP::TOADatabase::Get().Geometry()->CheckOverlaps();
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
             = CP::TOADatabase::Get().GeomId().GetGeomIdMap().begin();
         g != CP::TOADatabase::Get().GeomId().GetGeomIdMap().end();
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
        if (!CP::TOADatabase::Get().GeomId().GetPosition(geomId, pos)) {
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

            // Limit the test to the "Bar_" volumes.  This excludes the
            // composite volumes like "P0D_0".
            if (target.GetName().find("/Bar_") == std::string::npos) continue;

            CP::TGeometryId geomId;
            if (!CP::TOADatabase::Get().GeomId().GetGeometryId(
                    pos.X(), pos.Y(), pos.Z(), geomId)) {
                std::cout << "couldn't find geometry id " 
                          << target.AsInt() << " " << target
                          << std::endl; 
                std::cout << "FAIL" << std::endl;
                return 1;
            }
            if (geomId != target) {
                std::cout << "expected " << target.AsInt() 
                          <<  " " << target << std::endl;
                std::cout << "got      " << geomId.AsInt()
                          << " " << geomId << std::endl;
                std::cout << "FAIL" << std::endl;
                return 1;
            }
        }
    }
    
    int ngood = 0;
    int nbad = 0;
    
    // Loop over a bunch of bars in Top Left Barrel ECAL.
    for(int k = 0; k < 2; ++k){        
        for(int i = 0; i < 16; ++i){          
            for(int j = 0; j < 36; ++j){
                
                char place[200];
                if(k == 0) {
                    unsigned int len = snprintf(place,sizeof(place),"/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0/ScintX1_%i/Bar_%i",i,j);
                    if (len >= sizeof(place)) {
                        std::cout << "string not long enough" << std::endl;
                        std::cout << "FAIL" << std::endl;
                        return 1;
                    }
                }
                else {
                    unsigned int len = snprintf(place,sizeof(place),"/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0/ScintY1_%i/Bar_%i",i,j);
                    if (len >= sizeof(place)) {
                        std::cout << "string not long enough" << std::endl;
                        std::cout << "FAIL" << std::endl;
                        return 1;
                    }
                }

                // Start by figuring out what the position of this bar is
                gGeoManager->cd(place);            
                double local[3] = {0,0,0};
                double master[3];            
                gGeoManager->LocalToMaster(local,master);
                
                // Now check if we can re-find this bar.
                gGeoManager->cd("/t2k_1/OA_0/Magnet_0/LeftClam_0");            
                gGeoManager->FindNode(master[0],master[1],master[2]);
                
                std::string test1(place);
                std::string test2(gGeoManager->GetPath());
                
                if(test1 != test2){
                    std::cout << "Problem! " << place << " !=  " 
                              <<  gGeoManager->GetPath() << std::endl;
                    ++nbad;
                }
                else {
                    ++ngood;
                }
            }
        }
    }

    std::cout << "Number of successful checks: " << ngood << std::endl;
    std::cout << "Number of unsuccessful checks: " << nbad << std::endl;

    if (nbad) {
        std::cout << "FAIL" << std::endl;
        return 1;
    }

    std::cout << "SUCCESS" << std::endl;
    return 0;
}
