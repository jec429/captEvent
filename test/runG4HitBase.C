void runG4HitBase(const char* file, int events=10, const char* options="") 
{
// This is an example of a root macro to run a selector that is intended as a
// g4HitBase for user code.  To write your own user code (assume you are
// writing a pizero analysis), copy "g4HitBaseSelector.h" to "pizeroSelector.h",
// "g4HitBaseSelector.C" to "pizeroSelector.C, and "runG4HitBase.C" to
// "runPizero.C".  Then so a global search and replace of "g4HitBase" with
// "pizero" in all of the new files.  Add your analysis code to
// pizeroSelector::Process() in pizeroSelector.C.
//
// You can run your code with the following command:
//
//   root -b -q runPizero.C
//
// One minor warning: ROOT has *extremely* slow print functions, so don't
// print a lot of stuff in your analysis.

    printf("\nrun g4HitBase Selector\n");
    
    // Add include paths for any other libraries that you use here.
    gSystem->AddIncludePath("-I$OAEVENTROOT/src");

    // Load any other libraries that you use here.
    gSystem->Load("libGeom");
    gSystem->Load("liboaEvent");
    
    // Attach to the file.
    TFile f(file);

    // Read the detector geometry into memory.
    TOADatabase::Get().SetCurrentFile(&f);

    /// Get the tree of events.
    TTree *t = dynamic_cast<TTree*>(f.Get("ND280Events"));
    
    std::cout << "Process the file " << std::endl;

    // Run the g4HitBase selector.
    t->Process("g4HitBaseSelector.C+",options,events);
    
}
