#include <TManager.hxx>
#include <TGeomIdManager.hxx>

#include <iostream>
#include <fstream>
#include <string>

void usage(int argc, char **argv) {
    std::cout << std::endl
              << argv[0] << " -o <output-file-name> <input-file-name>" 
              << std::endl
              << std::endl
              << "   Dump a map of geometry identifiers to geometry path names."
              << std::endl;
}

int main(int argc, char** argv) {
    std::string outputName;

    for (;;) {
        int c = getopt(argc, argv, "ho:");
        switch (c) {
        case 'h':
            usage(argc,argv);
            return 0;
        case 'o':
            outputName = optarg;
            continue;
        }
        if (c<0) break;
    }

    
    if (argc<optind+1) { 
        std::cerr << "ERROR: Missing input file" << std::endl;
        usage(argc,argv);
        return 1;
    }
    
    // Load the geometry.
    std::string inputFile = argv[optind++];
    CP::TManager::Get().SetGeometryOverride(inputFile.c_str());
    CP::TManager::Get().Geometry();

    std::ostream *output = &std::cout;
    std::ofstream outputFile;
    if (outputName.size() > 0) {
        outputFile.open(outputName.c_str());
        output = &outputFile;
    }

    *output << "#File: " << inputFile << std::endl;
    *output << "#Hash: "
            << CP::TManager::Get().GeomId().GetHash().AsString()
            << std::endl;
    
    // Iterate through all of the defined geometry identifiers.
    for (CP::TGeomIdManager::GeomIdMap::const_iterator g
             = CP::TManager::Get().GeomId().GetGeomIdMap().begin();
         g != CP::TManager::Get().GeomId().GetGeomIdMap().end();
         ++g) {
        CP::TGeometryId geomId(g->first);
        if (!geomId.IsValid()) {
            std::cerr << "Invalid geometry id: " << g->first << std::endl;
            continue;
        }
        *output << (unsigned) g->first
                  << " " << geomId.GetName() << std::endl;
    }

    return 0;
}
