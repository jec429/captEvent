#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TTPCChannelId.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTTPCChannelId {
        baseTTPCChannelId() {
            // Run before each test.
        }
        ~baseTTPCChannelId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTTPCChannelId>::object testTTPCChannelId;
    test_group<baseTTPCChannelId> groupTTPCChannelId("TTPCChannelId");

    // Test the default constructor and destructor.
    template<> template<>
    void testTTPCChannelId::test<1> () {
        CP::TTPCChannelId g1(1);
        CP::TChannelId g2;
        CP::TTPCChannelId g3;
        g2 = g1;
        g3 = CP::TTPCChannelId(g2);
        ensure_equals("TTPCChannelId value matchs", g1.AsUInt(), (unsigned) 1);
        ensure_equals("TChannelId value matchs", g2.AsUInt(), (unsigned) 1);
        ensure_equals("TTPCChannelId copied value matchs", g3.AsUInt(),
                      (unsigned) 1);
        ensure("TChannelId and TTPCChannelId are equal", g1 == g2);
    }

    // Test the normal constructor.
    template<> template<>
    void testTTPCChannelId::test<2> () {
        int subDet = CP::TChannelId::kTPC;
        unsigned int dcc = 15;
        unsigned int fem = 2;
        unsigned int fec = 1;
        unsigned int asic = 2;
        unsigned int channel = 39;
        CP::TTPCChannelId g1(dcc,fem,fec,asic,channel);
        ensure("TTPCChannelId is valid", g1.IsValid());
        ensure_equals("Sub-detector value matchs", g1.GetSubDetector(),subDet);
        ensure_equals("DCC value matchs", g1.GetDCC(),dcc);
        ensure_equals("FEM value matchs", g1.GetFEM(),fem);
        ensure_equals("FEC value matchs", g1.GetFEC(),fec);
        ensure_equals("Asic value matchs", g1.GetAsic(),asic);
        ensure_equals("Channel value matchs", g1.GetChannel(),channel);
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
                          (unsigned)tmp.getter(), (unsigned)i);         \
            ensure_equals(mes " stays in field",                        \
                          (unsigned) tmp.upper(), (unsigned)0);         \
            ensure_equals(mes " stays in field",                        \
                          (unsigned) tmp.lower(), (unsigned)0);}}
    
    template<> template<>
    void testTTPCChannelId::test<10> () {
        CMP("Check DCC", CP::TTPCChannelId, 5, 18,
            SetDCC, GetDCC, 
            GetSubDetector, GetFEM);
    }

    template<> template<>
    void testTTPCChannelId::test<11> () {
        CMP("Check FEM", CP::TTPCChannelId, 4, 12,
            SetFEM, GetFEM, 
            GetDCC, GetFEC);
    }

    template<> template<>
    void testTTPCChannelId::test<12> () {
        CMP("Check FEC", CP::TTPCChannelId, 3, 6, 
            SetFEC, GetFEC, 
            GetFEM, GetAsic);
    }

    template<> template<>
    void testTTPCChannelId::test<13> () {
        CMP("Check Asic", CP::TTPCChannelId, 2, 4,
            SetAsic, GetAsic, 
            GetFEC, GetChannel);
    }

    template<> template<>
    void testTTPCChannelId::test<14> () {
        CMP("Check channel", CP::TTPCChannelId, 7, 80,
            SetChannel, GetChannel, 
            GetAsic, GetAsic);
    }
};

