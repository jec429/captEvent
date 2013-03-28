#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TFGDChannelId.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTFGDChannelId {
        baseTFGDChannelId() {
            // Run before each test.
        }
        ~baseTFGDChannelId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTFGDChannelId>::object testTFGDChannelId;
    test_group<baseTFGDChannelId> groupTFGDChannelId("TFGDChannelId");

    // Test the default constructor and destructor.
    template<> template<>
    void testTFGDChannelId::test<1> () {
        ND::TFGDChannelId g1(1);
        ND::TChannelId g2;
        ND::TFGDChannelId g3;
        g2 = g1;
        g3 = ND::TFGDChannelId(g2);
        ensure_equals("TFGDChannelId value matchs", g1.AsUInt(), (unsigned) 1);
        ensure_equals("TChannelId value matchs", g2.AsUInt(), (unsigned) 1);
        ensure_equals("TFGDChannelId copied value matchs", g3.AsUInt(), (unsigned) 1);
        ensure("TChannelId and TFGDChannelId are equal", g1 == g2);
    }

    // Test the normal constructor.
    template<> template<>
    void testTFGDChannelId::test<2> () {
        ND::TChannelId::SubDetId subDet = ND::TChannelId::kFGD;
        unsigned int crate = 15;
        unsigned int feb = 2;
        unsigned int chip = 1;
        unsigned int channel = 65;
        ND::TFGDChannelId g1(subDet,crate,feb,chip,channel);
        ensure("TFGDChannelId is valid", g1.IsValid());
        ensure_equals("Sub-detector value matchs", g1.GetSubDetector(),subDet);
        ensure_equals("Crate value matchs", g1.GetMinicrate(),crate);
        ensure_equals("FEB value matchs", g1.GetFEB(),feb);
        ensure_equals("Chip value matchs", g1.GetAfterChip(),chip);
        ensure_equals("Channel value matchs", g1.GetChannel(),channel);
    }

    // Test the secondary constructor.
    template<> template<>
    void testTFGDChannelId::test<3> () {
        ND::TChannelId::SubDetId subDet = ND::TChannelId::kFGD;
        unsigned int crate = 2;
        unsigned int feb = 1;
        int channel = 36;
        ND::TFGDChannelId g1(subDet,crate,feb,channel);
        ensure("TFGDChannelId is valid", g1.IsValid());
        ensure_equals("Sub-detector value matchs", g1.GetSubDetector(),subDet);
        ensure_equals("Crate value matchs", g1.GetMinicrate(),crate);
        ensure_equals("FEB value matchs", g1.GetFEB(),feb);
        ensure_equals("Channel value matchs", g1.GetFebChannel(),channel);
    }

// Check that all the setters work, and don't overflow bits into neighboring
// fields.  This is done with the CMP macro which sets all possible field
// values.  The arguments are:
//
//    mes   -- The message to prefix to the ensure output
//    cls   -- The class to construct
//    bits  -- The number of bits in the field.
//    physMax -- The actual maximum value that needs to be saved.
//    setter -- The setter for the field
//    getter -- The getter for the field
//    upper -- The getter for the more significant field (upper neighbor)
//    lower -- The getter for the less significant field (lower neighbor)
// 
// The tests should start at test<10>.
#define CMP(mes,cls,bits,physMax,setter,getter,upper,lower)             \
    {ensure_lessthan(mes " enough bits allocated",physMax-1,(1<<bits)); \
        for (int i=0; i< (1<<bits); ++i) {                              \
            cls tmp ; tmp.setter(i);                                    \
            ensure_equals(mes " returns correct value" ,                \
                          (unsigned) tmp.getter(), (unsigned) i);       \
            ensure_equals(mes " stays in field",                        \
                          (unsigned) tmp.upper(), (unsigned) 0);        \
            ensure_equals(mes " stays in field",                        \
                          (unsigned) tmp.lower(), (unsigned) 0);}}
    
    template<> template<>
    void testTFGDChannelId::test<10> () {
        /// \bug Maximum number of crates needs to be checked by FGD group.
        CMP("Check Crate", ND::TFGDChannelId, 6, 64,
            SetMinicrate, GetMinicrate, 
            GetSubDetector, GetFEB);
    }

    template<> template<>
    void testTFGDChannelId::test<11> () {
        /// \bug Maximum number of FEB needs to be checked by FGD group.
        CMP("Check FEB", ND::TFGDChannelId, 2, 4,
            SetFEB, GetFEB, 
            GetMinicrate, GetAfterChip);
    }

    template<> template<>
    void testTFGDChannelId::test<12> () {
        /// \bug Maximum number of After Chips needs to be checked by FGD group.
        CMP("Check after chip", ND::TFGDChannelId, 1, 2, 
            SetAfterChip, GetAfterChip, 
            GetFEB, GetChannel);
    }

    template<> template<>
    void testTFGDChannelId::test<13> () {
        /// \bug Maximum number of channels needs to be checked by FGD group.
        CMP("Check channel", ND::TFGDChannelId, 7, 128,
            SetChannel, GetChannel, 
            GetAfterChip, GetAfterChip);
    }
};

