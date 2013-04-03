#ifndef TGeomIdFinder_hxx_seen
#define TGeomIdFinder_hxx_seen

#include <string>
#include <vector>

#include "EoaCore.hxx"
#include "TGeomIdManager.hxx"

namespace CP {
    /// An exception thrown by a TGeomIdFinder when it doesn't want to follow
    /// a particular branch of the geometry.
    OA_EXCEPTION(EGeomIdStop,EoaCore);

    class TGeomIdFinder;
};

/// Find the TGeoNode associated with a geometry id.
class CP::TGeomIdFinder {
public:
    TGeomIdFinder() {}
    virtual ~TGeomIdFinder() {}

    /// Check to see if the current root node corresponds to a known
    /// TGeometryId.  If it does, then add the node to the fGeoNodeMap.  This
    /// can throw an EGeomIdStop exception to prevent searching any deeper in
    /// the geometry.
    virtual bool Search(const std::vector<std::string>& names, 
                        TGeometryId& id) {
        return false;
    }
};
#endif
