#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TGeometryId.hxx"
#include "ND280GeomId.hxx"
#include "ND280GeomIdDef.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTGeometryId {
        baseTGeometryId() {
            // Run before each test.
        }
        ~baseTGeometryId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTGeometryId>::object testTGeometryId;
    test_group<baseTGeometryId> groupTGeometryId("TGeometryId");

    // Test the default constructor and destructor.
    template<> template<>
    void testTGeometryId::test<1> () {
        ND::TGeometryId g1(1);
        ND::TGeometryId g2;
        g2 = g1;
        ensure_equals("TGeometryId value matchs", g2.AsInt(), 1);
    }

    // Test the validity.
    template<> template<>
    void testTGeometryId::test<2> () {
        ND::TGeometryId g1(1);
        ND::TGeometryId g2(-1);
        ensure("TGeometryId should be valid", g1.IsValid());
        ensure("TGeometryId should not be valid", !g2.IsValid());
    }

    // Test the field setters.
    template<> template<>
    void testTGeometryId::test<3> () {
        ND::TGeometryId g1;
        int msb = 25;
        int lsb = 11;
        int max = (1 << ((msb-lsb+1)));
        for (int val = 0; val < max; ++val) {
            g1.SetFieldSafe(val,msb,lsb);
            ensure_equals("Check field value", g1.GetFieldSafe(msb,lsb), val);
        }
    }

    // Test that an in range value doesn't throw exception.
    template<> template<>
    void testTGeometryId::test<4> () {
        ND::TGeometryId g1;
        int msb = 22;
        int lsb = 15;
        int max = 1 << ((msb-lsb)+1);
        try {
            g1.SetFieldSafe(max-1,msb,lsb);
        }
        catch (...) {
            fail("In range value threw exception.");
        }
    }

    // Test that an in range value doesn't throw exception.
    template<> template<>
    void testTGeometryId::test<5> () {
        ND::TGeometryId g1;
        int msb = 22;
        int lsb = 15;
        int max = 1 << ((msb-lsb)+1);
        try {
            g1.SetFieldSafe(max,msb,lsb);
            fail("Out of range value should throw exception.");
        }
        catch (ND::EGeomIdOutOfRange e) {
            /// All OK!
        }
        catch (...) {
            fail("Out of range value threw wrong exception.");
        }
    }
    
    // Test the bit definition for P0D scintillator bars.
    template<> template<>
    void testTGeometryId::test<6> () {
        ensure_equals("Bar LSB",ND::GeomId::Def::P0D::Bar::kBarLSB,
                      (unsigned)0);
        ensure_equals("Bar MSB",ND::GeomId::Def::P0D::Bar::kBarMSB,
                      (unsigned)10);
        ensure_equals("P0Dule LSB",ND::GeomId::Def::P0D::Bar::kP0DuleLSB,
                      (unsigned)11);
        ensure_equals("P0Dule MSB",ND::GeomId::Def::P0D::Bar::kP0DuleMSB,
                      (unsigned)16);
        ensure_equals("Layer LSB",ND::GeomId::Def::P0D::Bar::kLayerLSB,
                      (unsigned)17);
        ensure_equals("Layer MSB",ND::GeomId::Def::P0D::Bar::kLayerMSB,
                      (unsigned)17);
        ensure_equals("SP0Dule LSB",ND::GeomId::Def::P0D::Bar::kSP0DuleLSB,
                      (unsigned)18);
        ensure_equals("SP0Dule MSB",ND::GeomId::Def::P0D::Bar::kSP0DuleMSB,
                      (unsigned)20);
    }

    // Test the bit definition for TPC pads.
    template<> template<>
    void testTGeometryId::test<7> () {
        ensure_equals("Pad LSB",ND::GeomId::Def::TPC::Pad::kPadLSB,
                      (unsigned)0);
        ensure_equals("Pad MSB",ND::GeomId::Def::TPC::Pad::kPadMSB,
                      (unsigned)10);
        ensure_equals("Micromega LSB",ND::GeomId::Def::TPC::Pad::kMMegaLSB,
                      (unsigned)11);
        ensure_equals("Micromega MSB",ND::GeomId::Def::TPC::Pad::kMMegaMSB,
                      (unsigned)15);
        ensure_equals("Pad flag LSB",ND::GeomId::Def::TPC::Pad::kPadFlagLSB,
                      (unsigned)16);
        ensure_equals("Pad flag MSB",ND::GeomId::Def::TPC::Pad::kPadFlagMSB,
                      (unsigned)16);
        ensure_equals("Half LSB",ND::GeomId::Def::TPC::Pad::kHalfLSB,
                      (unsigned)17);
        ensure_equals("Half MSB",ND::GeomId::Def::TPC::Pad::kHalfMSB,
                      (unsigned)17);
        ensure_equals("TPC LSB",ND::GeomId::Def::TPC::Pad::kTPCLSB,
                      (unsigned)18);
        ensure_equals("TPC MSB",ND::GeomId::Def::TPC::Pad::kTPCMSB,
                      (unsigned)20);
    }

    /// Test the equality operators.
    template<> template<>
    void testTGeometryId::test<8> () {
        ND::TGeometryId a(ND::GeomId::P0D::Bar(30,1,30));
        ND::TGeometryId b(ND::GeomId::P0D::Bar(30,1,30));
        ND::TGeometryId c(ND::GeomId::P0D::Bar(30,1,31));

        ensure("Geometry Id equal to itself",(a == a));
        ensure("Geometry Id not not equal to itself",!(a != a));
        ensure("Geometry Id equal to same value",(a == b));
        ensure("Geometry Id not equal to different vlue",(a != c));

        ensure("Geometry id ordering " , (a<c));
        ensure("Geometry id reverse ordering " , !(c<a));
        ensure("Geometry id exclusive ordering " , !(a<b));
        ensure("Geometry id ordering " , std::less<ND::TGeometryId>()(a,c));
    }

    /// Test that maps work.
    template<> template<>
    void testTGeometryId::test<9> () {
        std::map<ND::TGeometryId, ND::TGeometryId> geoMap;
        ND::TGeometryId a(ND::GeomId::P0D::Bar(30,1,30));
        ND::TGeometryId b(ND::GeomId::P0D::Bar(30,1,30));
        ND::TGeometryId c(ND::GeomId::P0D::Bar(30,1,31));
        
        geoMap[a] = a;
        geoMap[c] = c;
        
        ensure_equals("Map element found", geoMap[a], a);
        ensure_equals("Map element found", geoMap[a], b);
        ensure_equals("Map element found", geoMap[c], c);
    }
    
};

