#include <iostream>
#include <vector>
#include <map>

#include <tut.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TManager.hxx"
#undef private
#undef protected
#include "TCaptLog.hxx"
#include "TGeometryId.hxx"
#include "TGeomIdManager.hxx"
#include "CaptGeomId.hxx"
#include "HEPUnits.hxx"

namespace tut {
    struct baseGeometry {
        baseGeometry() {
            // Run before each test.
            CP::TManager::Get().RegisterGeometryLookup(NULL);
            CP::TManager::Get().RegisterAlignmentLookup(NULL);
            CP::TManager::Get().ClearGeometryCallbacks();
            if (!gGeoManager) {
                CP::TManager::Get().GeomId().ReadGeometry(
                    CP::TSHAHashValue());
            }
            else {
                CP::TManager::Get().GeomId().ResetGeometry();
            }
        }
        ~baseGeometry() {
            // Run after each test.
            CP::TManager::Get().RegisterGeometryLookup(NULL);
            CP::TManager::Get().RegisterAlignmentLookup(NULL);
            CP::TManager::Get().ClearGeometryCallbacks();
        }
    };

    // Declare the test
    typedef test_group<baseGeometry>::object testGeometry;
    test_group<baseGeometry> groupGeometry("Geometry");

    // Test that a geometry can be found.  This is running without a file, and
    // the geometry has been loaded in the pre test setup, so this is testing
    // that the geometry override is working.  If the geometry isn't found,
    // this throws an exception, so there isn't any reason to have an "ensure"
    // check.
    template<> template<>
    void testGeometry::test<1> () {
        CP::TSHAHashValue hash 
            = CP::TManager::Get().GeomId().GetHash();
        CP::TManager::Get().SetGeometryOverride(hash);
        CP::TManager::Get().Geometry();
    }

    /// Implement a GeometryLookup class that returns alignment.  This works
    /// on the default geometry.
    class AlignmentLookup: public CP::TManager::AlignmentLookup {
        unsigned int fCalls;
    public:
        std::vector< std::pair<CP::TGeometryId,double> > fGeomIdZShift;
        AlignmentLookup() {}
        virtual ~AlignmentLookup() {}
        bool CheckAlignment(const CP::TEvent* const event) {
            return true;
        }
        CP::TAlignmentId StartAlignment(const CP::TEvent* const event) {
            fCalls = 0;
            unsigned int aid[5] = {0,0,0,0,0};
            aid[0] = fGeomIdZShift.size();
            for (unsigned i=0; i<fGeomIdZShift.size(); ++i) {
                aid[1 + (i%4)] += fGeomIdZShift[i].second/unit::mm;
            }
            return CP::TAlignmentId(aid);
        }
        TGeoMatrix* Align(const CP::TEvent* const event,
                          CP::TGeometryId& geomId) {
            TGeoMatrix* result = NULL;
            if (fCalls<fGeomIdZShift.size()) {
                geomId = fGeomIdZShift[fCalls].first;
                double zShift = fGeomIdZShift[fCalls].second;
                result = new TGeoTranslation(0,0,zShift);
            }
            ++fCalls;
            return result;
        }
    };
    AlignmentLookup alignmentLookup;

    /// Test the basic alignment capability.
    template<> template<>
    void testGeometry::test<2> () {
        // CP::TManager::Get().Geometry();
        ensure("Have valid geometry", gGeoManager != NULL);

        // Get the original position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Detector());
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdZShift.clear();
        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Detector(),
                                              2*unit::mm));

        CP::TManager::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TManager::Get().GeomId().ApplyAlignment(NULL);

        // Get the shifted position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Detector());
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        2.0*unit::mm, 0.1*unit::mm);
    }

    /// Test the cumulative alignment capability.
    template<> template<>
    void testGeometry::test<3> () {
        // CP::TManager::Get().Geometry();
        ensure("Have valid geometry", gGeoManager != NULL);

        // Get the original position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Plane(0));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdZShift.clear();
        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Detector(),
                                              3*unit::mm));

        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Plane(0),
                                              2*unit::mm));

        CP::TManager::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TManager::Get().GeomId().ApplyAlignment(NULL);

        // Get the shifted position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Plane(0));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        5.0*unit::mm, 0.1*unit::mm);
    }

    /// Same as test 3, but apply shifts in different order.
    template<> template<>
    void testGeometry::test<4> () {
        // CP::TManager::Get().Geometry();
        ensure("Have valid geometry", gGeoManager != NULL);

        // Get the original position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Plane(0));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdZShift.clear();
        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Plane(0),
                                              2*unit::mm));

        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Detector(),
                                              3*unit::mm));

        CP::TManager::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TManager::Get().GeomId().ApplyAlignment(NULL);

        // Get the shifted position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Plane(0));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        5.0*unit::mm, 0.1*unit::mm);
    }

    /// Apply multiple alignments to the same volume.
    template<> template<>
    void testGeometry::test<5> () {
        // CP::TManager::Get().Geometry();
        ensure("Have valid geometry", gGeoManager != NULL);

        // Get the original position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Plane(0));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdZShift.clear();
        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Plane(0),
                                              2*unit::mm));

        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Plane(0),
                                              3*unit::mm));

        alignmentLookup.fGeomIdZShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::Captain::Plane(0),
                                              4*unit::mm));

        CP::TManager::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TManager::Get().GeomId().ApplyAlignment(NULL);

        // Get the shifted position of the P0D detector.
        CP::TManager::Get().GeomId().CdId(CP::GeomId::Captain::Plane(0));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        9.0*unit::mm, 0.1*unit::mm);
    }

    /// Make sure that the TGeomIdManager::ResetGeometry is working.
    template<> template<>
    void testGeometry::test<6> () {
        // Get the existing geometry.
        // CP::TManager::Get().Geometry();
        ensure("Have valid geometry", gGeoManager != NULL);

        CP::TManager::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TManager::Get().GeomId().ApplyAlignment(NULL);

        CP::TSHAHashValue hc = CP::TManager::Get().GeomId().GetHash();
        CP::TAlignmentId aid = CP::TManager::Get().GeomId().GetAlignmentId();
 
        CP::TManager::Get().GeomId().ResetGeometry();

        ensure_equals("Hash Code before and after reset",
                      hc, CP::TManager::Get().GeomId().GetHash());

        ensure_equals("Alignment Id Code before and after reset",
                      aid, CP::TManager::Get().GeomId().GetAlignmentId());

    }        

    /// Make sure that the CdId method is working.
    template<> template<>
    void testGeometry::test<7> () {
        ensure("Have valid geometry", gGeoManager != NULL);

        // CP::TManager::Get().Geometry();
        for (int i=0; i<40; ++i) {
            ensure("CdId finds volume matching geometry id.",
                   CP::TManager::Get().GeomId().CdId(
                       CP::GeomId::Captain::Plane(i)));
        }
        ensure("CdId returns false for non-existent volume.",
               !CP::TManager::Get().GeomId().CdId(
                   CP::GeomId::Captain::Plane(40)));
    }        

    class LocalGeometryChange: public CP::TManager::GeometryChange {
    public:
        LocalGeometryChange() : fCallCount(0) {}
        void Callback(const CP::TEvent* const event) {++fCallCount;}
        int fCallCount;
    };
    LocalGeometryChange localGeometryChange;
        
    /// Test that the GeometryChange call back is working.
    template<> template<>
    void testGeometry::test<8> () {
        ensure("Have valid geometry", gGeoManager != NULL);

        CP::TSHAHashValue hash 
            = CP::TManager::Get().GeomId().GetHash();
        CP::TManager::Get().SetGeometryOverride(hash);
        CP::TManager::Get().RegisterGeometryCallback(&localGeometryChange);
        CP::TManager::Get().Geometry();
        CP::TManager::Get().Geometry();
        CP::TManager::Get().Geometry();
        ensure_equals("Call backs called", localGeometryChange.fCallCount,1);
    }

};
