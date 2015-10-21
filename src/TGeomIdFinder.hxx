#ifndef TGeomIdFinder_hxx_seen
#define TGeomIdFinder_hxx_seen

#include <string>
#include <vector>

#include "ECore.hxx"
#include "TGeomIdManager.hxx"

namespace CP {
    /// An exception thrown by a TGeomIdFinder when it doesn't want to follow
    /// a particular branch of the geometry.
    EXCEPTION(EGeomIdStop,ECore);

    class TGeomIdFinder;
};

/// Find the TGeoNode associated with a geometry id.
class CP::TGeomIdFinder {
public:
    TGeomIdFinder() {}
    virtual ~TGeomIdFinder() {}

    /// Check to see if the current root node corresponds to a known
    /// TGeometryId.  If it does, then the id (which is passed by reference)
    /// will be set to the geometry id, and this will return true.  The Search
    /// method can throw an EGeomIdStop exception to prevent recursing any
    /// deeper into the geometry.  The vector of names will contain the path
    /// of the current node.  For instance, a path of
    /// "/world/detector/subDetector", would be passed as a vector containing
    /// {"world", "detector", "subDetector"}.
    virtual bool Search(const std::vector<std::string>& names, 
                        TGeometryId& id) {
        return false;
    }
};
#endif
