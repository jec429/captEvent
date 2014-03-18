#include "TRuntimeParameters.hxx"
#include "TUnitsTable.hxx"

#include "HEPUnits.hxx"
#include "HEPConstants.hxx"
#include "TCaptLog.hxx"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <locale>

CP::TRuntimeParameters* 
CP::TRuntimeParameters::fTRuntimeParameters = NULL;

CP::TRuntimeParameters::TRuntimeParameters() {

    CaptLog("Initializing CP::TRuntimeParameters");

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
  
    CaptInfo("Opening parameter file: " << fullFileName);
  
    std::ifstream inputFile(fullFileName.c_str());
  
    // If input file doesn't exist.
    if (!inputFile) {
        // Just return if 'tryFile' is specified.
        if (tryFile) {
            CaptInfo("CP::TRuntimeParameters::ReadInputFile "
                      << "Cannot open input file '" 
                      << fullFileName << "'.");
            return;
        }
        else {
            // Otherwise, throw exception.
            CaptError("CP::TRuntimeParameters::ReadInputFile "
                       << "Cannot open input file '" 
                       << fullFileName << "'.");
            throw CP::EBadParameterFile();
        }
    }
  
    int inputState = 0;
    std::string inputString;
    std::string parameterName;
    std::string parValueUnit;
    std::string parValue;

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
        
                CaptError("CP::TRuntimeParameters::ReadInputFile "
                           "Input file '" << fileName << "'. Last parameter '"
                           << parameterName << "'.\n" 
                           << "Cannot find symbol '='.\n"
                           << "Badly formatted parameters file.");
                throw CP::EBadParameterFile();
            }
        }
        else if (inputState == 3) {	
            parValue = inputString;
            parValueUnit = inputString;
            inputState = 4;
        }
        else if (inputState == 4) {	
            if (inputString == ">") {
                // Finished reading. Save parameter; but only if the parameter
                // isn't already 'constant'
                if (fConstants.find(parameterName) == fConstants.end()) {
                    fRuntimeParameters[parameterName] = parValue;
                    // If fixParameters bool is set, fix this parameter now.
                    if (fixParameters) {
                        CaptLog("Override parameter \"" 
                                << parameterName
                                << "\" = \""
                                << parValue
                                << "\"");
                        fConstants.insert(parameterName);
                    }
                }
                inputState = 0;
            }
            else if (inputString == "<") {
                CaptError("CP::TRuntimeParameters::ReadInputFile "
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
                parValueUnit.append(" ");
                parValueUnit.append(inputString);

                // Use CP::TUnitsTable class to convert string of
                // value+unit to a double with value+unit.
                parValue 
                    = CP::TUnitsTable::Get().ConvertWithUnit(parValueUnit);
            }
        }
    }
  
    if (inputState != 0) {    
        CaptError("CP::TRuntimeParameters::ReadInputFile "
                   << "Input file '" << fileName << "'. Last parameter '"
                   << parameterName << "'.\n"
                   << "Cannot find symbol '>' at the end of file.\n"
                   << "Badly formatted parameters file."); 
        throw CP::EBadParameterFile();
    }  
    inputFile.close();
}


void CP::TRuntimeParameters::PrintListOfParameters() {

    CaptInfo("CP::TRuntimeParameters::PrintListOfParameters ");
    CaptInfo("List of parameters:");
  
    for (Parameters::iterator i = fRuntimeParameters.begin();
         i != fRuntimeParameters.end(); ++i)
        CaptInfo("  " << (*i).first << " = " << (*i).second);
    CaptInfo("");

}

bool 
CP::TRuntimeParameters::TryLoadingParametersFile(std::string parameterName) {
    CaptInfo("Trying to load parameters file for parameter: " 
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
    CaptLog("Using TRuntimeParameters override file = " << filename);

    // Setting final input variable to true forces the parameters
    // that are loaded to be 'fixed'; ie immutable.
    ReadInputFile(filename,"",false,true);
}

void CP::TRuntimeParameters::SetOverrideParameter(std::string name,
                                                  std::string override) {
    
    CaptLog("Override parameter \"" 
            << name 
            << "\" = \""
            << override
            << "\"");

    std::istringstream line(override);
    std::string value;
    std::string unit;
    if (!(line >> value >> unit)) {
        fConstants.insert(name);
        fRuntimeParameters[name] = override;
        return;
    }
    
    value = CP::TUnitsTable::Get().ConvertWithUnit(override);
    fConstants.insert(name);
    fRuntimeParameters[name] = override;
    
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
        CaptError("CP::TRuntimeParameters::GetParameterAsInteger "
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
        CaptError("CP::TRuntimeParameters::GetParameterAsDouble "
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
        CaptError("CP::TRuntimeParameters::GetParameterAsString "
                   << "Cannot find parameter '" << parameterName << "'.");
        throw CP::ENonexistantParameter();
    }
    return std::string();

}

void CP::TRuntimeParameters::ClearMapOfTRuntimeParameters() {
    fRuntimeParameters.clear();
}
