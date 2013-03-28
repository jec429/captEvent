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
#include "TND280Log.hxx"
#include "TGeometryId.hxx"
#include "TGeomIdManager.hxx"
#include "ND280GeomId.hxx"
#include "HEPUnits.hxx"

namespace tut {
    struct baseGeometry {
        baseGeometry() {
            // Run before each test.
            ND::TOADatabase::Get().RegisterGeometryLookup(NULL);
            ND::TOADatabase::Get().RegisterAlignmentLookup(NULL);
            ND::TOADatabase::Get().ClearGeometryCallbacks();
            if (!gGeoManager) {
                ND::TOADatabase::Get().GeomId().ReadGeometry(
                    ND::TSHAHashValue());
            }
            else {
                ND::TOADatabase::Get().GeomId().ResetGeometry();
            }
        }
        ~baseGeometry() {
            // Run after each test.
            ND::TOADatabase::Get().RegisterGeometryLookup(NULL);
            ND::TOADatabase::Get().RegisterAlignmentLookup(NULL);
            ND::TOADatabase::Get().ClearGeometryCallbacks();
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
        ND::TSHAHashValue hash 
            = ND::TOADatabase::Get().GeomId().GetHash();
        ND::TOADatabase::Get().SetGeometryOverride(hash);
        ND::TOADatabase::Get().Geometry();
    }

    /// Implement a GeometryLookup class that returns alignment.  This works
    /// on the default geometry.
    class AlignmentLookup: public ND::TOADatabase::AlignmentLookup {
        unsigned int fCalls;
    public:
        std::vector< std::pair<ND::TGeometryId,double> > fGeomIdYShift;
        AlignmentLookup() {}
        virtual ~AlignmentLookup() {}
        bool CheckAlignment(const ND::TND280Event* const event) {
            return true;
        }
        ND::TAlignmentId StartAlignment(const ND::TND280Event* const event) {
            fCalls = 0;
            unsigned int aid[5] = {0,0,0,0,0};
            aid[0] = fGeomIdYShift.size();
            for (unsigned i=0; i<fGeomIdYShift.size(); ++i) {
                aid[1 + (i%4)] += fGeomIdYShift[i].second/unit::mm;
            }
            return ND::TAlignmentId(aid);
        }
        TGeoMatrix* Align(const ND::TND280Event* const event,
                          ND::TGeometryId& geomId) {
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
        // ND::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::Detector());
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::Detector(),
                                              2*unit::mm));

        ND::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        ND::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::Detector());
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        2.0*unit::mm, 0.1*unit::mm);
    }

    /// Test the cumulative alignment capability.
    template<> template<>
    void testGeometry::test<3> () {
        // ND::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::P0Dule(3));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::Detector(),
                                              3*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::P0Dule(3),
                                              2*unit::mm));

        ND::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        ND::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::P0Dule(3));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        5.0*unit::mm, 0.1*unit::mm);
    }

    /// Same as test 3, but apply shifts in different order.
    template<> template<>
    void testGeometry::test<4> () {
        // ND::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::P0Dule(3));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::P0Dule(3),
                                              2*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::Detector(),
                                              3*unit::mm));

        ND::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        ND::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::P0Dule(3));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        5.0*unit::mm, 0.1*unit::mm);
    }

    /// Apply multiple alignments to the same volume.
    template<> template<>
    void testGeometry::test<5> () {
        // ND::TOADatabase::Get().Geometry();

        // Get the original position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::P0Dule(3));
        double local[3] = {0,0,0};
        double master[3];
        gGeoManager->LocalToMaster(local,master);
        double oldY = master[1];

        // Prepare the alignments.
        alignmentLookup.fGeomIdYShift.clear();
        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::P0Dule(3),
                                              2*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::P0Dule(3),
                                              3*unit::mm));

        alignmentLookup.fGeomIdYShift.push_back(
            std::pair<ND::TGeometryId,double>(ND::GeomId::P0D::P0Dule(3),
                                              4*unit::mm));

        ND::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        ND::TOADatabase::Get().AlignGeometry(NULL);

        // Get the shifted position of the P0D detector.
        ND::TOADatabase::Get().GeomId().CdId(ND::GeomId::P0D::P0Dule(3));
        gGeoManager->LocalToMaster(local,master);
        double newY = master[1];
        ensure_distance("Alignment shift", newY-oldY, 
                        9.0*unit::mm, 0.1*unit::mm);
    }

    /// Make sure that the TGeomIdManager::ResetGeometry is working.
    template<> template<>
    void testGeometry::test<6> () {
        // Get the existing geometry.
        // ND::TOADatabase::Get().Geometry();

        ND::TOADatabase::Get().RegisterAlignmentLookup(&alignmentLookup);
        ND::TOADatabase::Get().AlignGeometry(NULL);

        ND::TSHAHashValue hc = ND::TOADatabase::Get().GeomId().GetHash();
        ND::TAlignmentId aid = ND::TOADatabase::Get().GeomId().GetAlignmentId();
 
        ND::TOADatabase::Get().GeomId().ResetGeometry();

        ensure_equals("Hash Code before and after reset",
                      hc, ND::TOADatabase::Get().GeomId().GetHash());

        ensure_equals("Alignment Id Code before and after reset",
                      aid, ND::TOADatabase::Get().GeomId().GetAlignmentId());

    }        

    /// Make sure that the CdId method is working.
    template<> template<>
    void testGeometry::test<7> () {
        // ND::TOADatabase::Get().Geometry();
        for (int i=0; i<40; ++i) {
            ensure("CdId finds volume matching geometry id.",
                   ND::TOADatabase::Get().GeomId().CdId(
                       ND::GeomId::P0D::P0Dule(i)));
        }
        ensure("CdId returns false for non-existent volume.",
               !ND::TOADatabase::Get().GeomId().CdId(
                   ND::GeomId::P0D::P0Dule(40)));
    }        

    class LocalGeometryChange: public ND::TOADatabase::GeometryChange {
    public:
        LocalGeometryChange() : fCallCount(0) {}
        void Callback(const ND::TND280Event* const event) {++fCallCount;}
        int fCallCount;
    };
    LocalGeometryChange localGeometryChange;
        
    /// Test that the GeometryChange call back is working.
    template<> template<>
    void testGeometry::test<8> () {
        ND::TSHAHashValue hash 
            = ND::TOADatabase::Get().GeomId().GetHash();
        ND::TOADatabase::Get().SetGeometryOverride(hash);
        ND::TOADatabase::Get().RegisterGeometryCallback(&localGeometryChange);
        ND::TOADatabase::Get().Geometry();
        ND::TOADatabase::Get().Geometry();
        ND::TOADatabase::Get().Geometry();
        ensure_equals("Call backs called", localGeometryChange.fCallCount,1);
    }

    /// Test that the water target identifiers.
    template<> template<>
    void testGeometry::test<9> () {
        for (int i=0; i<25; ++i) {
            ND::TGeometryId targetId = ND::GeomId::P0D::Target(i);
            ensure("CdId find the target volume.",
                   ND::TOADatabase::Get().GeomId().CdId(targetId));
        }
        ensure("CdId returns false for non-existent target.",
               !ND::TOADatabase::Get().GeomId().CdId(
                   ND::GeomId::P0D::Target(25)));
    }
};
