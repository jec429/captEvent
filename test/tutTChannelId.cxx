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
        ND::TChannelId g1(1);
        ND::TChannelId g2;
        g2 = g1;
        ensure_equals("TChannelId value matchs", g2.AsUInt(), unsigned(1));
        ensure("TChannelId values are equal", g1 == g2);
    }

    // Test the validity checks in TChannelId.
    template<> template<>
    void testTChannelId::test<2> () {
        ND::TChannelId g1(0x00000000);
        ensure("Invalid guard and sub-detector",!g1.IsValid());

        ND::TChannelId g2;
        g2.SetGuardBit();
        ensure("Sub-detector not set", !g2.IsValid());

        ND::TChannelId g3;
        g3.SetGuardBit();
        g3.SetSubDetector(ND::TChannelId::kMaxDetector);
        ensure("Sub-detector out of range", !g3.IsValid()); 

        ND::TChannelId g4;
        g4.SetSubDetector(ND::TChannelId::kP0D);
        ensure("Invalid guard bit", !g4.IsValid()); 

        ND::TChannelId g5;
        g5.SetGuardBit();
        g5.SetSubDetector(ND::TChannelId::kP0D);
        ensure("Valid channel id.", g5.IsValid()); 
    }

    // Test the kP0D subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<3> () {
        ND::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(ND::TChannelId::kP0D);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct P0D sub-detector value",
                      ND::TChannelId::kP0D,1);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),ND::TChannelId::kP0D);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"P0D");
        ensure("Is a TFB channel",g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kTPC subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<4> () {
        ND::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(ND::TChannelId::kTPC);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct TPC sub-detector value",
                      ND::TChannelId::kTPC,2);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),ND::TChannelId::kTPC);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"TPC");
        ensure("Is a TPC channel",g1.IsTPCChannel());
        ensure("Is not a TFB channel",!g1.IsTFBChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kFGD subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<5> () {
        ND::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(ND::TChannelId::kFGD);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct FGD sub-detector value",
                      ND::TChannelId::kFGD,3);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),ND::TChannelId::kFGD);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"FGD");
        ensure("Is a FGD channel",g1.IsFGDChannel());
        ensure("Is not a TFB channel",!g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
    }

    // Test the kECal subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<6> () {
        ND::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(ND::TChannelId::kECal);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct ECal sub-detector value",
                      ND::TChannelId::kECal,4);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),ND::TChannelId::kECal);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"ECal");
        ensure("Is a TFB channel",g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kSMRD subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<9> () {
        ND::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(ND::TChannelId::kSMRD);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct SMRD sub-detector value",
                      ND::TChannelId::kSMRD,7);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),ND::TChannelId::kSMRD);
        ensure_equals("Correct sub-detector name ",
                      g1.SubDetAsString(),"SMRD");
        ensure("Is a TFB channel",g1.IsTFBChannel());
        ensure("Is not a TPC channel",!g1.IsTPCChannel());
        ensure("Is not a FGD channel",!g1.IsFGDChannel());
    }

    // Test the kINGRID subdetector getter and setter.
    template<> template<>
    void testTChannelId::test<10> () {
        ND::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(ND::TChannelId::kINGRID);
        ensure("Valid channel id", g1.IsValid());
        ensure_equals("Correct INGRID sub-detector value",
                      ND::TChannelId::kINGRID,8);
        ensure_equals("Correct sub-detector value in channel id",
                      g1.GetSubDetector(),ND::TChannelId::kINGRID);
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
        ND::TChannelId g1;
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
        ND::TChannelId g1;
        g1.SetGuardBit();
        g1.SetSubDetector(ND::TChannelId::kMaxDetector);
        ensure_equals("Set a MaxDetector value for sub-detector",
                      g1.GetSubDetector(),ND::TChannelId::kMaxDetector);
        ensure("Out of range sub-detector id is not valid",
               !g1.IsValid());
    }
    
    // Test out of range detector values.  These values cannot be saved in the
    // bit field, so the field should be set to zero.
    template <> template <>
    void testTChannelId::test<13> () {
        ND::TChannelId g1;
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
            ND::TChannelId g1;
            g1.SetSubDetector(i);
            ensure_equals("Sub detector field value saved",
                          g1.GetSubDetector(), i);
            ensure_equals("Field doesn't over flow",
                          (g1.AsUInt()&ND::TChannelId::kGuard_Mask), (unsigned) 0);
        }
    }
};

