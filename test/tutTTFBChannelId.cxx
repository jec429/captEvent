#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TTFBChannelId.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTTFBChannelId {
        baseTTFBChannelId() {
            // Run before each test.
        }
        ~baseTTFBChannelId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTTFBChannelId>::object testTTFBChannelId;
    test_group<baseTTFBChannelId> groupTTFBChannelId("TTFBChannelId");

    // Test the default constructor and destructor.
    template<> template<>
    void testTTFBChannelId::test<1> () {
        ND::TTFBChannelId g1(1);
        ND::TChannelId g2;
        ND::TTFBChannelId g3;
        g2 = g1;
        g3 = ND::TTFBChannelId(g2);
        ensure_equals("TTFBChannelId value matchs", g1.AsUInt(), (unsigned)1);
        ensure_equals("TChannelId value matchs", g2.AsUInt(), (unsigned) 1);
        ensure_equals("TTFBChannelId copied value matchs", g3.AsUInt(), (unsigned) 1);
        ensure("TChannelId and TTFBChannelId are equal", g1 == g2);
    }

    // Test the normal constructor.
    template<> template<>
    void testTTFBChannelId::test<2> () {
        ND::TChannelId::SubDetId subDet = ND::TChannelId::kP0D;
        unsigned int rmm = 2;
        unsigned int tfb = 39;
        unsigned int tripChip = 2;
        unsigned int channel = 15;
        unsigned int capacitor = 21;
        ND::TTFBChannelId g1(subDet,rmm,tfb,tripChip,channel,capacitor);
        ensure("TTFBChannelId is valid", g1.IsValid());
        ensure_equals("Sub-detector value matchs", g1.GetSubDetector(),subDet);
        ensure_equals("RMM value matchs", g1.GetRMM(),rmm);
        ensure_equals("TFB value matchs", g1.GetTFB(),tfb);
        ensure_equals("TripT chip value matchs", g1.GetTripChip(),tripChip);
        ensure_equals("Channel value matchs", g1.GetChannel(),channel);
        ensure_equals("Capacitor value matchs", g1.GetCapacitor(),capacitor);
    }

    // Test the normal constructor.
    template<> template<>
    void testTTFBChannelId::test<3> () {
        ND::TChannelId::SubDetId subDet = ND::TChannelId::kP0D;
        unsigned int rmm = 2;
        unsigned int tfb = 39;
        unsigned int tripChip = 2;
        unsigned int channel = 15;
        unsigned int capacitor = 21;
        ND::TTFBChannelId g1(subDet,rmm,tfb,tripChip,channel,capacitor);
        ensure("TTFBChannelId is valid", g1.IsValid());
        ensure_equals("Sub-detector value matchs", g1.GetSubDetector(),subDet);
        ensure_equals("RMM value matchs", g1.GetRMM(),rmm);
        ensure_equals("TFB value matchs", g1.GetTFB(),tfb);
        ensure_equals("TripT chip value matchs", g1.GetTripChip(),tripChip);
        ensure_equals("Channel value matchs", g1.GetChannel(),channel);
        ensure_equals("Capacitor value matchs", g1.GetCapacitor(),capacitor);
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
    void testTTFBChannelId::test<10> () {
        CMP("Check RMM", ND::TTFBChannelId, 5, 20,
            SetRMM, GetRMM, 
            GetSubDetector, GetTFB);
    }

    template<> template<>
    void testTTFBChannelId::test<11> () {
        CMP("Check TFB", ND::TTFBChannelId, 6, 48,
             SetTFB, GetTFB, 
             GetRMM, GetCapacitor);
    }

    template<> template<>
    void testTTFBChannelId::test<12> () {
        CMP("Check capacitor", ND::TTFBChannelId, 5, 25,
             SetCapacitor, GetCapacitor, 
             GetTFB, GetTripChip);
    }

    template<> template<>
    void testTTFBChannelId::test<13> () {
        CMP("Check trip chip", ND::TTFBChannelId, 2, 4,
             SetTripChip, GetTripChip, 
             GetCapacitor, GetChannel);
    }

    template<> template<>
    void testTTFBChannelId::test<14> () {
        // test GetTripChip twice since this is the bottom of the int.
        CMP("Check channel", ND::TTFBChannelId, 5, 16,
             SetChannel, GetChannel, 
             GetTripChip, GetTripChip);
    }
};

