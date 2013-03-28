void runExample(const char* file, int events=10, const char* options="") 
{
// This is an example of a root macro to run a selector that is intended as a
// template for user code.  To write your own user code (assume you are
// writing a pizero analysis), copy "exampleSelector.h" to "pizeroSelector.h",
// "exampleSelector.C" to "pizeroSelector.C, and "runExample.C" to
// "runPizero.C".  Then so a global search and replace of "example" with
// "pizero" in all of the new files.  Add your analysis code to
// pizeroSelector::Process() in pizeroSelector.C.
//
// You can run your code with the following command:
//
//   root -b -q runPizero.C
//
// One minor warning: ROOT has *extremely* slow print functions, so don't
// print a lot of stuff in your analysis.

    printf("\nrun example Selector\n");
    
    // Add include paths for any other libraries that you use here.
    gSystem->AddIncludePath("-I$OAEVENTROOT/src");

    // Load any other libraries that you use here.
    gSystem->Load("libGeom");
    gSystem->Load("liboaEvent");
    
    // Attach to the file.
    TFile f(file);

//     TGeoManager *g = dynamic_cast<TGeoManager*>(f.Get("ND280Geometry"));
//     // Build the node cache.
//     g->GetCache()->BuildIdArray();

    // Read the detector geometry into memory.
    TOADatabase::Get().SetCurrentFile(&f);

    /// Get the tree of events.
    TTree *t = dynamic_cast<TTree*>(f.Get("ND280Events"));
    
    std::cout << "Process the file " << std::endl;

    // Run the example selector.
    t->Process("exampleSelector.C+",options,events);
    
}
