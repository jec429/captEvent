#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TSHAHashValue.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTSHAHashValue {
        baseTSHAHashValue() {
            // Run before each test.
        }
        ~baseTSHAHashValue() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTSHAHashValue>::object testTSHAHashValue;
    test_group<baseTSHAHashValue> groupTSHAHashValue("TSHAHashValue");

    // Test the default constructor and destructor.
    template<> template<>
    void testTSHAHashValue::test<1> () {
        CP::TSHAHashValue g1;

        ensure_equals("Default element is zero",g1(0),(unsigned) 0);
        ensure_equals("Default element is zero",g1(1),(unsigned) 0);
        ensure_equals("Default element is zero",g1(2),(unsigned) 0);
        ensure_equals("Default element is zero",g1(3),(unsigned) 0);
        ensure_equals("Default element is zero",g1(4),(unsigned) 0);

    }

    // Test the value constructor.
    template<> template<>
    void testTSHAHashValue::test<2> () {
        CP::TSHAHashValue g1(1,2,3,4,5);

        ensure_equals("Constructed hash value is correct",g1(0),(UInt_t)1);
        ensure_equals("Constructed hash value is correct",g1(1),(UInt_t)2);
        ensure_equals("Constructed hash value is correct",g1(2),(UInt_t)3);
        ensure_equals("Constructed hash value is correct",g1(3),(UInt_t)4);
        ensure_equals("Constructed hash value is correct",g1(4),(UInt_t)5);

    }

    // Test the < comparison 
    template<> template<>
    void testTSHAHashValue::test<3> () {
        CP::TSHAHashValue g1;
        CP::TSHAHashValue g2;

        ensure("Default is not less than itself",!(g1<g2));
        ensure("Default is not less than itself",!(g2<g1));

        CP::TSHAHashValue g3(1,2,3,4,5);
        CP::TSHAHashValue g4(1,2,3,4,5);

        ensure("Equal hash values are not less than itself",!(g3<g4));
        ensure("Equal hash values are not less than itself",!(g4<g3));

        ensure("Default less than all other hashs.",(g1<g3));
        ensure("Default not greater than all other hashs.",!(g3<g1));
        
        CP::TSHAHashValue g5(1,2,3,4,6);

        ensure("Test less than", (g4<g5));
        ensure("Test less than (inverted)", !(g5<g4));

    }

    template<> template<>
    void testTSHAHashValue::test<4> () {
        CP::TSHAHashValue g1;
        CP::TSHAHashValue g2;

        ensure("Default value equal to itself",(g1==g2));

        CP::TSHAHashValue g3(1,2,3,4,5);
        CP::TSHAHashValue g4(1,2,3,4,5);

        ensure("Set value equal to itself", (g3==g4));

        ensure("Default value not equal to set value",!(g1==g3));
    }
};

