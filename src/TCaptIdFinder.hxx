#ifndef TCaptIdFinder_hxx_seen
#define TCaptIdFinder_hxx_seen

#include "TGeomIdFinder.hxx"

namespace CP {
    class TCaptIdFinder;
};

/// Find TGeoManager node ids associated with the TGeometryId objects.
class CP::TCaptIdFinder : public TGeomIdFinder {
public: 
    TCaptIdFinder();
    ~TCaptIdFinder();

    bool Search(const std::vector<std::string>& names, TGeometryId& id);

private: 

    // The most recently seen plane
    int fPlane;

    // The most recently seen wire.
    int fWire;

    // The most recently seen photosensor.
    int fPhotosensor;

};
#endif
