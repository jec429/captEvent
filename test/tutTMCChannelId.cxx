#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TMCChannelId.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTMCChannelId {
        baseTMCChannelId() {
            // Run before each test.
        }
        ~baseTMCChannelId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTMCChannelId>::object testTMCChannelId;
    test_group<baseTMCChannelId> groupTMCChannelId("TMCChannelId");

    // Test the default constructor and destructor.
    template<> template<>
    void testTMCChannelId::test<1> () {
        CP::TMCChannelId g1(1);
        CP::TChannelId g2;
        CP::TMCChannelId g3;
        g2 = g1;
        g3 = CP::TMCChannelId(g2);
        ensure_equals("TMCChannelId value matchs", g1.AsUInt(), (unsigned)1);
        ensure_equals("TChannelId value matchs", g2.AsUInt(), (unsigned) 1);
        ensure_equals("TMCChannelId copied value matchs", g3.AsUInt(), 
                      (unsigned) 1);
        ensure("TChannelId and TMCChannelId are equal", g1 == g2);
    }

    // Test the normal constructor.
    template<> template<>
    void testTMCChannelId::test<2> () {
        unsigned int type = 2;
        unsigned int plane = 15;
        unsigned int wire = 21;
        CP::TMCChannelId g1(type,plane,wire);
        ensure_equals("type value matchs", g1.GetType(),type);
        ensure_equals("plane value matchs", g1.GetPlane(),plane);
        ensure_equals("wire value matchs", g1.GetWire(),wire);
        ensure("TMCChannelId is valid", g1.IsValid());
    }

};

