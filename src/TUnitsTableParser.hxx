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
    ~TUnitsTableParser();
  
    ///  Get a reference to the singleton instance of runtime parameters.  If
    ///  this is first attempt at reference then singleton is instantiated and
    ///  parameters are read from text files.
    static CP::TUnitsTableParser& Get(void) {
        if (!fTUnitsTableParser)
            fTUnitsTableParser = new TUnitsTableParser();
        return *fTUnitsTableParser;
    }

    /// Converts a string like "1.5 cm" into a number represented by a string.
    /// For example, since the basic units are millimeters, "1.5 cm" is
    /// converted to "15"
    std::string ConvertWithUnit(std::string line);

    /// Convert a length value (i.e. in mm) into a string for printing.  The
    /// output will be formated as "xxx uu" where xxx is a number between -999
    /// and 999, and uu is a unit.
    std::string ConvertLength(double len);

    /// Convert a time value (i.e. in ns) into a string for printing.  The
    /// output will be formated as "xxx uu" where xxx is a number between -999
    /// and 999, and uu is a unit.
    std::string ConvertTime(double tim);

    /// Prints all the defined units.
    void PrintListOfUnits();

private:
    TUnitsTableParser();

    /// The table mapping known unit names to multipliers.
    std::map<std::string, double> units;

    /// The static pointer to the singleton instance.
    static TUnitsTableParser* fTUnitsTableParser;
};

#endif
