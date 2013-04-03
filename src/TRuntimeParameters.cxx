#include "TRuntimeParameters.hxx"
#include "TUnitsTableParser.hxx"

#include "HEPUnits.hxx"
#include "HEPConstants.hxx"
#include "TND280Log.hxx"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <locale>

CP::TRuntimeParameters* 
CP::TRuntimeParameters::fTRuntimeParameters = NULL;

CP::TRuntimeParameters::TRuntimeParameters() {

    ND280Log("Initializing CP::TRuntimeParameters");

    // Build the set of units for Geant4.
    fUnitsTableParser = new CP::TUnitsTableParser();

    // We do not open any parameter files by default in the constructor.
    // Instead, parameter files are open automatically when an entry in the
    // file is requested by the user.
}


CP::TRuntimeParameters::~TRuntimeParameters() { }

/// Read in each parameter file.
void CP::TRuntimeParameters::ReadInputFile(std::string fileName, 
                                             std::string dirName,
                                             bool tryFile,
                                             bool fixParameters) {
    std::string fullFileName;

    // Make full file name.
    if (dirName.empty()) {
        fullFileName = fileName;
    }
    else {
        fullFileName = dirName + "/" + fileName;
    }
  
    ND280Info("Opening parameter file: " << fullFileName);
  
    std::ifstream inputFile(fullFileName.c_str());
  
    // If input file doesn't exist.
    if (!inputFile) {
        // Just return if 'tryFile' is specified.
        if (tryFile) {
            ND280Info("CP::TRuntimeParameters::ReadInputFile "
                      << "Cannot open input file '" 
                      << fullFileName << "'.");
            return;
        }
        else {
            // Otherwise, throw exception.
            ND280Error("CP::TRuntimeParameters::ReadInputFile "
                       << "Cannot open input file '" 
                       << fullFileName << "'.");
            throw CP::EBadParameterFile();
        }
    }
  
    int inputState = 0;
    std::string inputString;
    std::string parameterName;
    std::string parameterValueUnit;
    std::string parameterValue;

    while (inputFile >> inputString) {
        if (inputState == 0) {
            if (inputString == "<")
                inputState = 1;
        }
        else if (inputState == 1) {
            parameterName = inputString;
            inputState = 2;
        }
        else if (inputState == 2) {
            if (inputString == "=")
                inputState = 3;
            else {
        
                ND280Error("CP::TRuntimeParameters::ReadInputFile "
                           "Input file '" << fileName << "'. Last parameter '"
                           << parameterName << "'.\n" 
                           << "Cannot find symbol '='.\n"
                           << "Badly formatted parameters file.");
                throw CP::EBadParameterFile();
            }
        }
        else if (inputState == 3) {	
            parameterValue = inputString;
            parameterValueUnit = inputString;
            inputState = 4;
        }
        else if (inputState == 4) {	
            if (inputString == ">") {
                // Finished reading. Save parameter; but only if the parameter
                // isn't already 'constant'
                if (fConstants.find(parameterName) == fConstants.end()) {
                    fRuntimeParameters[parameterName] = parameterValue;
                    // If fixParameters bool is set, fix this parameter now.
                    if (fixParameters)
                        fConstants.insert(parameterName);
                }
                inputState = 0;
            }
            else if (inputString == "<") {
                ND280Error("CP::TRuntimeParameters::ReadInputFile "
                           << "Input file '" << fileName 
                           << "'. Last parameter '"
                           << parameterName << "'." << std::endl
                           << "Unexpected symbol '<'." << std::endl
                           << "Badly formatted parameters file.");
                throw CP::EBadParameterFile();
            }
            else {

                // The parameter must have a unit.  Resave the value with the
                // correct unit.
                parameterValueUnit.append(" ");
                parameterValueUnit.append(inputString);

                // Use CP::TUnitsTableParser class to convert string of
                // value+unit to a double with value+unit.
                parameterValue 
                    = fUnitsTableParser->ConvertWithUnit(parameterValueUnit);
            }
        }
    }
  
    if (inputState != 0) {    
        ND280Error("CP::TRuntimeParameters::ReadInputFile "
                   << "Input file '" << fileName << "'. Last parameter '"
                   << parameterName << "'.\n"
                   << "Cannot find symbol '>' at the end of file.\n"
                   << "Badly formatted parameters file."); 
        throw CP::EBadParameterFile();
    }  
    inputFile.close();
}


void CP::TRuntimeParameters::PrintListOfParameters() {

    ND280Info("CP::TRuntimeParameters::PrintListOfParameters ");
    ND280Info("List of parameters:");
  
    for (Parameters::iterator i = fRuntimeParameters.begin();
         i != fRuntimeParameters.end(); ++i)
        ND280Info("  " << (*i).first << " = " << (*i).second);
    ND280Info("");

}

bool 
CP::TRuntimeParameters::TryLoadingParametersFile(std::string parameterName) {
    ND280Info("Trying to load parameters file for parameter: " 
              << parameterName);

    // Figure out the name of the package
    int pos = parameterName.find(".");
    std::string packageName(parameterName.c_str(), pos);

    // and the file name for parameters file
    std::string fileName = packageName + ".parameters.dat";

    // and the directory of parameters file.
    std::string packageROOT = packageName + "ROOT";
    std::transform(packageROOT.begin(), packageROOT.end(),packageROOT.begin(),
                   (int(*)(int)) std::toupper);
    
    std::string dirName =  std::getenv(packageROOT.c_str()) 
        + std::string("/parameters/");

    // Now try reading in this file.  Last input variable is set to true,
    // indicating that we don't want to throw exception if a file is not found.
    ReadInputFile(fileName,dirName,true);
 
    // Now try to find this parameter again
    Parameters::iterator i = fRuntimeParameters.find(parameterName);  
    if (i != fRuntimeParameters.end()) return true;
    return false;
}


void CP::TRuntimeParameters::ReadParamOverrideFile(std::string filename) {
    ND280Log("Using TRuntimeParameters override file = " << filename);

    // Setting final input variable to true forces the parameters
    // that are loaded to be 'fixed'; ie immutable.
    ReadInputFile(filename,"",false,true);
}


bool CP::TRuntimeParameters::HasParameter(std::string parameterName) {
    Parameters::iterator i = fRuntimeParameters.find(parameterName);

    if (i != fRuntimeParameters.end()) {
        return true;
    }
    else {
        // Okay, we didn't find this parameter on the first try.  
        // Let's see if we can try loading the parameters file.
        // The function will return whether this parameter 
        // was found afterwards.
        return TryLoadingParametersFile(parameterName);
    }
}


int CP::TRuntimeParameters::GetParameterI(std::string parameterName) {
  
    if (HasParameter(parameterName)) {
        return atoi(fRuntimeParameters[parameterName].c_str());
    } 
    else {
        ND280Error("CP::TRuntimeParameters::GetParameterAsInteger "
                   << "Cannot find parameter '" << parameterName << "'.");
        throw CP::ENonexistantParameter();
    }
    return -1;

}


double CP::TRuntimeParameters::GetParameterD(std::string parameterName) {

    if (HasParameter(parameterName)) {
        return atof(fRuntimeParameters[parameterName].c_str());
    }
    else { 
        ND280Error("CP::TRuntimeParameters::GetParameterAsDouble "
                   << "Cannot find parameter '" << parameterName << "'.");
        throw CP::ENonexistantParameter();
    }
    return -1;

}


std::string CP::TRuntimeParameters::GetParameterS(std::string parameterName) {

    if (HasParameter(parameterName)) {
        return fRuntimeParameters[parameterName];
    }
    else {
        ND280Error("CP::TRuntimeParameters::GetParameterAsString "
                   << "Cannot find parameter '" << parameterName << "'.");
        throw CP::ENonexistantParameter();
    }
    return std::string();

}

void CP::TRuntimeParameters::ClearMapOfTRuntimeParameters() {
    fRuntimeParameters.clear();
}
