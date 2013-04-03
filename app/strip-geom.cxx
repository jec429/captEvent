/// Copy the geometry found in a ROOT file into a second root file.

#include <memory>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <vector>

#include <TFile.h>
#include <TGeoManager.h>
#include <TGeoOverlap.h>

#include <HEPUnits.hxx>
#include <TOADatabase.hxx>
#include <TGeomIdManager.hxx>
#include <TSHAHashValue.hxx>
#include <TCaptLog.hxx>

void usage(int argc, char **argv) {
    std::cout << std::endl
              << argv[0] << " [options] <intput-file-name>" 
              << std::endl
              << "    -c            -- Toggle the overlap check [default: on]"
              << std::endl
              << "    -p [prefix]   -- prefix applied to output name"
              << std::endl
              << "    -h            -- print this message"
              << std::endl
              << std::endl
              << "   Strip the geometry out of the input file, and write it"
              << std::endl
              << "   to a file with the geometry hash code in the file name."
              << std::endl;
}

int main(int argc, char** argv) {
    std::string outputName;
    bool checkOverlaps = true;
    for (;;) {
        int c = getopt(argc, argv, "cp:h");
        switch (c) {
        case 'c':
            checkOverlaps = !checkOverlaps;
            break;
        case 'p':
            outputName = optarg;
            break;
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
    
    std::string inputName = argv[optind++];

    TFile* inputPtr(TFile::Open(inputName.c_str(),"OLD"));
    if (!inputPtr) {
        return 1;
    }
    std::auto_ptr<TFile> inputFile(inputPtr);
    if (!CP::TOADatabase::Get().GeomId().LoadGeometry(*inputFile,
                                                      CP::TSHAHashValue())) {
        std::cerr << "Error: Geometry not found in input file." << std::endl;
        return 1;
    }
    inputFile->Close();

    if (checkOverlaps) {
        // Check for overlaps with sampling.  Look for overlaps at 0.1 mm
        // size.
        gGeoManager->CheckOverlaps(0.1*unit::mm,"s100000");
        {
            TIter next(gGeoManager->GetListOfOverlaps());
            int count = 0;
            TGeoOverlap* overlap;
            while ((overlap=(TGeoOverlap*)next())) {
                ++count;
                overlap->PrintInfo();
            }
            if (count > 0) {
                CaptError("Geometry has overlaps");
                std::exit(1);
            }
        }
        
        // Check for overlaps at volume corners.  Look for overlaps at 0.1 mm
        // size.
        gGeoManager->CheckOverlaps(0.1*unit::mm);
        {
            TIter next(gGeoManager->GetListOfOverlaps());
            int count = 0;
            TGeoOverlap* overlap;
            while ((overlap=(TGeoOverlap*)next())) {
                ++count;
                overlap->PrintInfo();
            }
            if (count > 0) {
                CaptError("Geometry has overlaps");
                std::exit(1);
            }
        }
    }

    CP::TSHAHashValue hc = CP::TOADatabase::Get().GeomId().GetHash();
    std::ostringstream geomName;
    geomName << "geom"; 
    geomName << std::hex << std::nouppercase << std::setfill('0');
    for (int i = 0; i<5; ++i) {
        geomName << "-";
        geomName << std::setw(8) << hc(i);
    }
    geomName  << ".root";

    if (outputName != "") outputName += "_";
    outputName += geomName.str();
    std::cout << outputName << std::endl;
    
    TFile outputFile(outputName.c_str(),"RECREATE");
    if (!outputFile.IsOpen()) {
        std::cerr << "Cannot open output file" << std::endl;
        return 1;
    }

    // Override the name of the geometry to use the old form of the name.
    // This allows the geometry files to be used with older (per v8) oaEvent
    // libraries.
    gGeoManager->SetName("ND280Geometry");

    gGeoManager->Write();
    outputFile.Close();

    return 0;
}
