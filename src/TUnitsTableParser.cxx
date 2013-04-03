#include "TUnitsTableParser.hxx"

#include "TCaptLog.hxx"
#include "HEPUnits.hxx"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath> 
#include <cstdlib>

CP::TUnitsTableParser::TUnitsTableParser() {
    // Define units.  Add to this list as needed.

    // Length 
    units["km"] = unit::kilometer;
    units["m"] = unit::meter;
    units["cm"] = unit::centimeter ;
    units["mm"] = unit::millimeter ;
    units["nm"] = unit::nanometer ;

    // Area
    units["m2"] = unit::meter*unit::meter ;
    units["cm2"] = unit::centimeter*unit::centimeter ;
    units["mm2"] = unit::millimeter*unit::millimeter ;
    units["barn"] = unit::barn ;
    units["mbarn"] = unit::millibarn ;
    units["mubarn"] = unit::microbarn  ;
    units["nbarn"] = unit::nanobarn ;
    units["pbarn"] = unit::picobarn ;

    // Volume
    units["m3"] = unit::meter*unit::meter*unit::meter ;
    units["cm3"] = unit::cm3  ;
    units["mm3"] = unit::mm3  ;

    // Degree
    units["rad"] = unit::radian ;
    units["mrad"] = unit::milliradian  ;
    units["sr"] = unit::steradian ;
    units["deg"] = unit::degree ;

    // Time
    units["s"] = unit::second ;
    units["ms"] = unit::millisecond ;
    units["ns"] = unit::nanosecond ;

    // Frequency
    units["Hz"] = unit::hertz ;
    units["kHz"] = unit::kilohertz ;
    units["MHz"] = unit::megahertz  ;

    // Electric Charge
    units["e+"] = unit::eplus  ;
    units["C"] = unit::coulomb ;
    units["muC"] = unit::microcoulomb ;
    units["fC"] = unit::femtocoulomb ;

    // Energy
    units["eV"] = unit::eV ;
    units["keV"] = unit::keV  ;
    units["MeV"] = unit::MeV ;
    units["GeV"] = unit::GeV  ;
    units["TeV"] = unit::TeV  ;
    units["J"] = unit::joule ;

    // Energy/Length
    units["GeV/cm"] = units["GeV"]/units["cm"]  ;
    units["MeV/cm"] = units["MeV"]/units["cm"]  ;
    units["keV/cm"] = units["keV"]/units["cm"]  ;
    units["eV/cm"] = units["eV"]/units["cm"] ;

    // Mass
    units["mg"] = unit::milligram  ;
    units["g"] = unit::gram ;
    units["kg"] = unit::kilogram  ;

    // Volumic Mass
    units["g/cm3"] = units["g"]/units["cm3"] ;
    units["mg/cm3"] = units["mg"]/units["cm3"]  ;
    units["kg/m3"] = units["kg"]/units["m3"] ;

    units["g/cm2"] = units["g"]/units["cm2"] ;
    units["mg/cm2"] = units["mg"]/units["cm2"] ;
    units["kg/m2"] = units["kg"]/units["m2"]  ;

    // Power
    units["W"] = unit::watt  ;

    // Pressure
    units["Pa"] = unit::pascal ;
    units["bar"] = unit::bar  ;
    units["atm"] = unit::atmosphere  ;

    // Electric current
    units["A"] = unit::ampere ;
    units["mA"] = unit::milliampere ;
    units["muA"] = unit::microampere  ;
    units["nA"] = unit::nanoampere ;

    // Electric potential
    units["V"] = unit::volt ;
    units["kV"] = unit::kilovolt  ;
    units["MV"] = unit::megavolt ;

    // Magnetic flux density
    units["T"] = unit::tesla ;
    units["kG"] = unit::kilogauss  ;
    units["G"] = unit::gauss ;

    // Speed
    units["cm/us"] = units["cm"]/units["mus"]  ;
    units["cm/ns"] = units["cm"]/units["ns"]  ;
    units["mm/ns"] = units["mm"]/units["ns"]  ;

    // Length/Energy
    units["mm/MeV"] = units["mm"]/units["MeV"];
    units["mm/keV"] = units["mm"]/units["keV"];
    units["cm/MeV"] = units["cm"]/units["MeV"];
    units["cm/keV"] = units["cm"]/units["keV"];

    // Dummy units for diffusion coefficient
    units["cm/sqrt(cm)"] = units["cm"]/std::sqrt(units["cm"]);
    units["mm/sqrt(cm)"] = units["mm"]/std::sqrt(units["cm"]);
    units["um/sqrt(cm)"] = units["um"]/std::sqrt(units["cm"]);

    // Dummy units for electron mobility
    units["cm2/(Vs)"] = units["cm2"]/(units["V"]*units["s"]);
}

CP::TUnitsTableParser::~TUnitsTableParser() {}

std::string CP::TUnitsTableParser::ConvertWithUnit(std::string input) {

    double value;
    std::string unit;
  
    std::istringstream line(input);
    if (!(line >> value >> unit)) {
        PrintListOfUnits();
        CaptError("Badly formatted input string.");
        std::exit(1);
    }
  
    // Check if requested unit is in map.
    if (units.find(unit) == units.end()) {
        PrintListOfUnits();
        CaptError("CP::TUnitsTableParser: requested unit '"
                   << unit << "' not found.");
        std::exit(1);
    }
  
    value = value * units[unit];
    std::ostringstream result;
    result << value;

    return result.str();
}

void CP::TUnitsTableParser::PrintListOfUnits() {
    std::cout << std::endl;
    std::cout << "***** List of available units *****" << std::endl;  
    for (std::map<std::string, double>::iterator unit = units.begin();
         unit != units.end(); unit++) {
        std::cout << "    Unit Name: " << (*unit).first 
                  << "    Multiplier: " << (*unit).second
                  <<  std::endl;
    }
    std::cout << std::endl;
}
