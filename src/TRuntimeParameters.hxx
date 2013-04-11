#ifndef TRuntimeParameters_hxx_seen
#define TRuntimeParameters_hxx_seen

#include "ECore.hxx"

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <set>

namespace CP {
    class TRuntimeParameters;
    class TUnitsTableParser;

    // General run time parameters exceptions
    EXCEPTION(EOARuntimeParameters,CP::ECore);
    // Exception for missing parameters
    EXCEPTION(ENonexistantParameter,EOARuntimeParameters);
    // Exception for reading parameter file
    EXCEPTION(EBadParameterFile,EOARuntimeParameters);
};

/// This class is meant to provide access to a set of parameters that are
/// defined in a set of text files; in particular, the text files are used to
/// store 'software runtime parameters' in calibration and reconstruction
/// algorithms.  This reads parameter values saved in parameters files that
/// are found in each package.  The files are named
/// ${<PACKAGE>ROOT}/parameters/<package>.parameters.dat.  The files are found
/// by taking the first part of the parameter name and using it as a package
/// name.  For instance, parameters for the captEvent package would be saved
/// in a file name ${CAPTEVENTROOT}/parameters/captEvent.parameters.dat.  The
/// file  should look something like:
///
/// \code
/// This is a parameters file.  It is free form text, with parameters embedded
/// like this
///
/// < captEvent.someParameter = value > 
///
/// All text outside of the greaterthan and less than signs will be treated as
/// comments.
/// \endcode
///
/// This is described in detail in the \ref runtimeParameters documentation. 
class CP::TRuntimeParameters {
public:
    ~TRuntimeParameters();

    ///  Get a reference to the singleton instance of runtime parameters.  If
    ///  this is first attempt at reference then singleton is instantiated and
    ///  parameters are read from text files.
    static CP::TRuntimeParameters& Get(void) {
        if (!fTRuntimeParameters)
            fTRuntimeParameters = new TRuntimeParameters();
        return *fTRuntimeParameters;
    }

    /// Check if Parameter is stored in database
    bool HasParameter(std::string);

    /// Get parameter.  Value is returned as integer. 
    int GetParameterI(std::string);

    /// Get parameter.  Value is returned as double. 
    double GetParameterD(std::string);

    /// Get parameter.  Value is returned as string. 
    std::string GetParameterS(std::string);

    /// The te object to translate units.
    CP::TUnitsTableParser* GetUnitsTableParser() {
        return fUnitsTableParser;
    }

    /// This command allows the user to set parameters from the
    /// command line; the command is different from the standard
    /// file reading, in that the parameters that are set are 'fixed'.
    /// Ie, they are immutable and cannot be changed, even if they
    /// exist in some other parameters file that is read in later.
    void ReadParamOverrideFile(std::string filename);

private:
    /// Yes, it's private...
    TRuntimeParameters();

    /// Add a non-functional private copy constructor.
    TRuntimeParameters(const TRuntimeParameters &) {}

    /// Create the parameters.
    void ClearMapOfTRuntimeParameters();

    /// Reads parameters from input files.  Function can be used to read in
    /// extra parameter files.
    void ReadInputFile(std::string filename, 
                       std::string dirName="", 
                       bool tryFile = false, 
                       bool fixParameters = false);  


    /// Prints list of saved parameters
    void PrintListOfParameters();

    /// Helper method to attempt to read parameters file based on parameters
    /// name.  The return value indicates whether we can now find this
    /// parameter after trying to open this file (return 1 means parameter
    /// found).
    bool TryLoadingParametersFile(std::string parameterName);

    typedef std::map<std::string, std::string, 
                     std::less<std::string> > Parameters;

    /// map containing list of parameters and their values.
    Parameters fRuntimeParameters;
    
    /// A set of all the 'fixed'.  'Fixed' parameters cannot be changed, even
    /// if they reappear in a different parameters file.  This is used for the
    /// parameter override files.
    std::set<std::string> fConstants;

    /// The units parser.
    CP::TUnitsTableParser *fUnitsTableParser;

    /// The static pointer to the singleton instance.
    static TRuntimeParameters* fTRuntimeParameters;
};
#endif
