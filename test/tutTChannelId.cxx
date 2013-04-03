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
        g4.SetSubDetector(CP::TChannelId::kP0D);
        ensure("Invalid guard bit", !g4.IsValid()); 

        CP::TChannelId g5;
        g5.SetGuardBit();
        g5.SetSubDetector(CP::TChannelId::kP0D);
        ensure("Valid channel id.", g5.IsValid()); 
    }

    // Test the kP0D subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<3> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kP0D);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct P0D sub-detector value",
                      CP::TChannelId::kP0D,1);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),CP::TChannelId::kP0D);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"P0D");
        ensure("Is a TFB channel",g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kTPC subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<4> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kTPC);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct TPC sub-detector value",
                      CP::TChannelId::kTPC,2);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),CP::TChannelId::kTPC);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"TPC");
        ensure("Is a TPC channel",g1.IsTPCChannel());
        ensure("Is not a TFB channel",!g1.IsTFBChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kFGD subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<5> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kFGD);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct FGD sub-detector value",
                      CP::TChannelId::kFGD,3);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),CP::TChannelId::kFGD);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"FGD");
        ensure("Is a FGD channel",g1.IsFGDChannel());
        ensure("Is not a TFB channel",!g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
    }

    // Test the kECal subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<6> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kECal);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct ECal sub-detector value",
                      CP::TChannelId::kECal,4);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),CP::TChannelId::kECal);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"ECal");
        ensure("Is a TFB channel",g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kSMRD subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<9> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kSMRD);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct SMRD sub-detector value",
                      CP::TChannelId::kSMRD,7);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),CP::TChannelId::kSMRD);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"SMRD");
        ensure("Is a TFB channel",g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kINGRID subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<10> () {
        CP::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(CP::TChannelId::kINGRID);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct INGRID sub-detector value",
                      CP::TChannelId::kINGRID,8);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),CP::TChannelId::kINGRID);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"INGRID");
        ensure("Is a TFB channel",g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
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

    // Test that the full range of the bit field can be set.
    template <> template <>
    void testTChannelId::test<14> () {
        for (int i=0; i<64; ++i) {
            CP::TChannelId g1;
            g1.SetSubDetector(i);
            ensure_equals("Sub detector field value saved",
                          g1.GetSubDetector(), i);
            ensure_equals("Field doesn't over flow",
                          (g1.AsUInt()&CP::TChannelId::kGuard_Mask), (unsigned) 0);
        }
    }
};

