#include <iostream>
#include <vector>
#include <map>

#include <tut.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TOADatabase.hxx"
#undef private
#undef protected
#include "TCaptLog.hxx"
#include "TGeometryId.hxx"
#include "TGeomIdManager.hxx"
#include "ND280GeomId.hxx"
#include "HEPUnits.hxx"

namespace tut {
    struct baseGeometry {
        baseGeometry() {
            // Run before each test.
            CP::TOADatabase::Get().RegisterGeometryLookup(NULL);
            CP::TOADatabase::Get().RegisterAlignmentLookup(NULL);
            CP::TOADatabase::Get().ClearGeometryCallbacks();
            if (!gGeoManager) {
                CP::TOADatabase::Get().GeomId().ReadGeometry(
                    CP::TSHAHashValue());
            }
            else {
                CP::TOADatabase::Get().GeomId().ResetGeometry();
            }
        }
        ~baseGeometry() {
            // Run after each test.
            CP::TOADatabase::Get().RegisterGeometryLookup(NULL);
            CP::TOADatabase::Get().RegisterAlignmentLookup(NULL);
            CP::TOADatabase::Get().ClearGeometryCallbacks();
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
            = CP::TOADatabase::Get().GeomId().GetHash();
        CP::TOADatabase::Get().SetGeometryOverride(hash);
        CP::TOADatabase::Get().Geometry();
    }

    /// Implement a GeometryLookup class that returns alignment.  This works
    /// on the default geometry.
    class AlignmentLookup: public CP::TOADatabase::AlignmentLookup {
        unsigned int fCalls;
    public:
        std::vector< std::pair<CP::TGeometryId,double> > fGeomIdYShift;
        AlignmentLookup() {}
        virtual ~AlignmentLookup() {}
        bool CheckAlignment(const CP::TEvent* const event) {
            return true;
        }
        CP::TAlignmentId StartAlignment(const CP::TEvent* const event) {
            fCalls = 0;
            unsigned int aid[5] = {0,0,0,0,0};
            aid[0] = fGeomIdYShift.size();
            for (unsigned i=0; i<fGeomIdYShift.size(); ++i) {
                aid[1 + (i%4)] += fGeomIdYShift[i].second/unit::mm;
            }
            return CP::TAlignmentId(aid);
        }
        TGeoMatrix* Align(const CP::TEvent* const event,
                          CP::TGeometryId& geomId) {
            TGeoMatrix* result = NULL;
            if (fCalls<fGeomIdYShift.size()) {
                geomId = fGeomIdYShift[fCalls].first;
                double yShift = fGeomIdYShift[fCalls].second;
                result = new TGeoTranslation(0,yShift,0);
            }
            ++fCalls;
            return result;
        }
    };
    AlignmentLookup alignmentLookup;

    /// Test the basic alignment capability.
    template<> template<>
    void testGeometry::test<2> () {
        // CP::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::Detector());
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::Detector(),
                                              2*unit::mm));

        CP::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::Detector());
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        2.0*unit::mm, 0.1*unit::mm);
    }

    /// Test the cumulative alignment capability.
    template<> template<>
    void testGeometry::test<3> () {
        // CP::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::P0Dule(3));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::Detector(),
                                              3*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::P0Dule(3),
                                              2*unit::mm));

        CP::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::P0Dule(3));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        5.0*unit::mm, 0.1*unit::mm);
    }

    /// Same as test 3, but apply shifts in different order.
    template<> template<>
    void testGeometry::test<4> () {
        // CP::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::P0Dule(3));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::P0Dule(3),
                                              2*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::Detector(),
                                              3*unit::mm));

        CP::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::P0Dule(3));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        5.0*unit::mm, 0.1*unit::mm);
    }

    /// Apply multiple alignments to the same volume.
    template<> template<>
    void testGeometry::test<5> () {
        // CP::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::P0Dule(3));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::P0Dule(3),
                                              2*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::P0Dule(3),
                                              3*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<CP::TGeometryId,double>(CP::GeomId::P0D::P0Dule(3),
                                              4*unit::mm));

        CP::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        CP::TOADatabase::Get().GeomId().CdId(CP::GeomId::P0D::P0Dule(3));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        9.0*unit::mm, 0.1*unit::mm);
    }

    /// Make sure that the TGeomIdManager::ResetGeometry is working.
    template<> template<>
    void testGeometry::test<6> () {
        // Get the existing geometry.
        // CP::TOADatabase::Get().Geometry();

        CP::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        CP::TOADatabase::Get().AlignGeometry(NULL);

        CP::TSHAHashValue hc = CP::TOADatabase::Get().GeomId().GetHash();
        CP::TAlignmentId aid = CP::TOADatabase::Get().GeomId().GetAlignmentId();
 
        CP::TOADatabase::Get().GeomId().ResetGeometry();

        ensure_equals("Hash Code before and after reset",
                      hc, CP::TOADatabase::Get().GeomId().GetHash());

        ensure_equals("Alignment Id Code before and after reset",
                      aid, CP::TOADatabase::Get().GeomId().GetAlignmentId());

    }        

    /// Make sure that the CdId method is working.
    template<> template<>
    void testGeometry::test<7> () {
        // CP::TOADatabase::Get().Geometry();
        for (int i=0; i<40; ++i) {
            ensure("CdId finds volume matching geometry id.",
                   CP::TOADatabase::Get().GeomId().CdId(
                       CP::GeomId::P0D::P0Dule(i)));
        }
        ensure("CdId returns false for non-existent volume.",
               !CP::TOADatabase::Get().GeomId().CdId(
                   CP::GeomId::P0D::P0Dule(40)));
    }        

    class LocalGeometryChange: public CP::TOADatabase::GeometryChange {
    public:
        LocalGeometryChange() : fCallCount(0) {}
        void Callback(const CP::TEvent* const event) {++fCallCount;}
        int fCallCount;
    };
    LocalGeometryChange localGeometryChange;
        
    /// Test that the GeometryChange call back is working.
    template<> template<>
    void testGeometry::test<8> () {
        CP::TSHAHashValue hash 
            = CP::TOADatabase::Get().GeomId().GetHash();
        CP::TOADatabase::Get().SetGeometryOverride(hash);
        CP::TOADatabase::Get().RegisterGeometryCallback(&localGeometryChange);
        CP::TOADatabase::Get().Geometry();
        CP::TOADatabase::Get().Geometry();
        CP::TOADatabase::Get().Geometry();
        ensure_equals("Call backs called", localGeometryChange.fCallCount,1);
    }

    /// Test that the water target identifiers.
    template<> template<>
    void testGeometry::test<9> () {
        for (int i=0; i<25; ++i) {
            CP::TGeometryId targetId = CP::GeomId::P0D::Target(i);
            ensure("CdId find the target volume.",
                   CP::TOADatabase::Get().GeomId().CdId(targetId));
        }
        ensure("CdId returns false for non-existent target.",
               !CP::TOADatabase::Get().GeomId().CdId(
                   CP::GeomId::P0D::Target(25)));
    }
};
