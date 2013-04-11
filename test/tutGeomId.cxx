#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#include "TGeometryId.hxx"
#include "CaptGeomId.hxx"
#include "CaptGeomIdDef.hxx"

namespace tut {
    struct baseGeomId {
        baseGeomId() {
            // Run before each test.
        }
        ~baseGeomId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseGeomId>::object testGeomId;
    test_group<baseGeomId> groupGeomId("GeomId");

    // Test P0D geometry identifiers.
    template<> template<>
    void testGeomId::test<1> () {
        CP::TGeometryId gid = CP::GeomId::Captain::Detector();
        
        ensure("Captain Detector ", CP::GeomId::Captain::IsCryostat(gid));

        for (int xuv = 0; xuv < 3; ++xuv) {
                for (int b=0; b<1000; ++b) {
                    gid = CP::GeomId::Captain::Wire(xuv,b);
                    ensure("Wire IsCryostat",
                           CP::GeomId::Captain::IsCryostat(gid));
                    ensure_equals("Wire plane matches id",
                                  CP::GeomId::Captain::GetWirePlane(gid), xuv);
                    ensure_equals("Wire number matches id",
                                  CP::GeomId::Captain::GetWireNumber(gid), b);
                }
        }
    }
    
};

