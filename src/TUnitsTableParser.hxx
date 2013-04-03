#ifndef TUnitsTableParser_h
#define TUnitsTableParser_h 
#include <string>
#include <map>

namespace CP {
    class TUnitsTableParser;
};

/// This class provides a method for converting a string like "1.5 cm" into a
/// double with the appropriate unit.  To do so it defines a set of units,
/// using the same base units as in HEPUnits.hxx (e.g. mm, ns, MeV).  Only a
/// fairly limited set of units is defined.
class CP::TUnitsTableParser {
public:

    /// Constructor.  Creates list of units.
    TUnitsTableParser();
    ~TUnitsTableParser();
  
    /// Converts a string like "1.5 cm" into a number represented by a string.
    /// For example, since the basic units are millimeters, "1.5 cm" is
    /// converted to "15"
    std::string ConvertWithUnit(std::string line);

    /// Prints all the defined units.
    void PrintListOfUnits();

private:

    /// The table mapping known unit names to multipliers.
    std::map<std::string, double> units;
};

#endif
