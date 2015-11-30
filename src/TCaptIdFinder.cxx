#include <iostream>

#include <TGeoManager.h>

#include "CaptGeomId.hxx"
#include "CaptGeomIdDef.hxx"
#include "TCaptIdFinder.hxx"

CP::TCaptIdFinder::TCaptIdFinder() 
    : fPlane(-1), fWire(-1), fPhotosensor(-1), fWavelengthShifter(-1) {}
CP::TCaptIdFinder::~TCaptIdFinder() {}

bool CP::TCaptIdFinder::Search(const std::vector<std::string>& names, 
                              TGeometryId& id) {

#ifdef DEBUG_CaptFinder
    for (std::vector<std::string>::const_iterator s = names.begin();
         s != names.end();
         ++s) std::cout << " " << *s;
    std::cout << std::endl;
#endif

    // Assign the main detector volume.
    if (names.back().find("Cryostat_")!=std::string::npos) {
        id = CP::GeomId::Captain::Detector();
        return true;
    }

    // Assign the drift volume.
    if (names.back().find("Drift_")!=std::string::npos) {
        id = CP::GeomId::Captain::Drift();
        return true;
    }

    // Assign the layer 
    if (names.back().find("XPlane_")!=std::string::npos) {
        fPlane = 0;
        id = CP::GeomId::Captain::Plane(fPlane);
        fWire = -1;
        return true;
    }
        
    if (names.back().find("VPlane_")!=std::string::npos) {
        fPlane = 1;
        id = CP::GeomId::Captain::Plane(fPlane);
        fWire = -1;
        return true;
    }
        
    if (names.back().find("UPlane_")!=std::string::npos) {
        fPlane = 2;
        id = CP::GeomId::Captain::Plane(fPlane);
        fWire = -1;
        return true;
    }
        
    // Assign the wire.
    if (names.back().find("Wire_")!=std::string::npos) {
        if ((names.end()-2)->find("UPlane_") == std::string::npos
            && (names.end()-2)->find("VPlane_") == std::string::npos
            && (names.end()-2)->find("XPlane_") == std::string::npos) {
            return false;
        }
        id = CP::GeomId::Captain::Wire(fPlane, ++fWire);
        return true;
    }

    // Assign the photosensor.
    if (names.back().find("PhotoCathode_") != std::string::npos) {
        id = CP::GeomId::Captain::Photosensor(++fPhotosensor);
        return true;
    }

    // Assign the wavelength shifter.
    if (names.back().find("TPB_") != std::string::npos) {
        id = CP::GeomId::Captain::WavelengthShifter(++fWavelengthShifter);
        return true;
    }

    return false;
}
