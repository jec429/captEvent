#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TChannelId.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTChannelId {
        baseTChannelId() {
            // Run before each test.
        }
        ~baseTChannelId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTChannelId>::object testTChannelId;
    test_group<baseTChannelId> groupTChannelId("TChannelId");

    // Test the default constructor and destructor.
    template<> template<>
    void testTChannelId::test<1> () {
        CP::TChannelId g1(1);
        CP::TChannelId g2;
        g2 = g1;
        ensure_equals("TChannelId value matchs", g2.AsUInt(), unsigned(1));
        ensure("TChannelId values are equal", g1 == g2);
    }

    // Test the validity checks in TChannelId.
    template<> template<>
    void testTChannelId::test<2> () {
        CP::TChannelId g1(0x00000000);
        ensure("Invalid guard and sub-detector",!g1.IsValid());

        CP::TChannelId g2;
        g2.SetGuardBit();
        ensure("Sub-detector not set", !g2.IsValid());

        CP::TChannelId g3;
        g3.SetGuardBit();
        g3.SetSubDetector(CP::TChannelId::kMaxDetector);
        ensure("Sub-detector out of range", !g3.IsValid()); 

        CP::TChannelId g4;
        g4.SetSubDetector(CP::TChannelId::kMC);
        ensure("Invalid guard bit", !g4.IsValid()); 

        CP::TChannelId g5;
        g5.SetGuardBit();
        g5.SetSubDetector(CP::TChannelId::kMC);
        ensure("Valid channel id.", g5.IsValid()); 
    }

    // Test the kMC subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<3> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kMC);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct MC sub-detector value",
                      CP::TChannelId::kMC,1);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),CP::TChannelId::kMC);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"MC");
        ensure("Is an MC channel",g1.IsMCChannel());
    }

    // Test that subdetector zero is correctly flaged as a "bad" sub-detector
    // value, but that the correct value is still saved.
    template <> template <>
    void testTChannelId::test<11> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(0);
        ensure_equals("Set a zero value for sub-detector",
                      g1.GetSubDetector(), 0);
        ensure("Out of range sub-detector id is not valid",
               !g1.IsValid());
    }
    
    // Test that subdetector "kMaxDetector" is correctly flaged as a "bad"
    // sub-detector value, but that the correct value is still saved.
    template <> template <>
    void testTChannelId::test<12> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kMaxDetector);
        ensure_equals("Set a MaxDetector value for sub-detector",
                      g1.GetSubDetector(),CP::TChannelId::kMaxDetector);
        ensure("Out of range sub-detector id is not valid",
               !g1.IsValid());
    }
    
    // Test out of range detector values.  These values cannot be saved in the
    // bit field, so the field should be set to zero.
    template <> template <>
    void testTChannelId::test<13> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(-1);
        ensure_equals("Set a negative value for sub-detector",
                      g1.GetSubDetector(), 0);
        ensure("Out of range sub-detector id is not valid",
               !g1.IsValid());
        g1.SetSubDetector(99999);
        ensure_equals("Set a huge value for sub-detector",
                      g1.GetSubDetector(), 0);
        ensure("Out of range sub-detector id is not valid",
               !g1.IsValid());
    }

};

