#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TND280Context.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTND280Context {
        baseTND280Context() {
            // Run before each test.
        }
        ~baseTND280Context() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTND280Context>::object testTND280Context;
    test_group<baseTND280Context> groupTND280Context("TND280Context");

    // Test the default constructor and destructor.
    template<> template<>
    void testTND280Context::test<1> () {
        CP::TND280Context g1;

        int invalid = 0xdeadbeef;
        ensure_equals("Invald value matches 0XDEADBEEF",
                      CP::TND280Context::Invalid, invalid);
        ensure_equals("Invald value matches 0XDEADBEEF",
                      CP::TND280Context::Invalid, (int) 0XDEADBEEF);
    }

    // Test the default constructor and destructor for the partition.
    template<> template<>
    void testTND280Context::test<2> () {
        CP::TND280Context g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Partition value is 0xDEADBEEF", 
                      g1.GetPartition(), invalid);
        ensure_equals("Partition value matches CP::TND280Context::Invalid", 
                      g1.GetPartition(), CP::TND280Context::Invalid);

    }

    // Test the default constructor and destructor for the run.
    template<> template<>
    void testTND280Context::test<3> () {
        CP::TND280Context g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Run value is 0xDEADBEEF", 
                      g1.GetRun(), invalid);
        ensure_equals("Run value matches CP::TND280Context::Invalid", 
                      g1.GetRun(), CP::TND280Context::Invalid);

    }

    // Test the default constructor and destructor for the sub run.
    template<> template<>
    void testTND280Context::test<4> () {
        CP::TND280Context g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Sub run value is 0xDEADBEEF", 
                      g1.GetSubRun(), invalid);
        ensure_equals("Sub run value matches CP::TND280Context::Invalid", 
                      g1.GetSubRun(), CP::TND280Context::Invalid);

    }

    // Test the default constructor and destructor for the event.
    template<> template<>
    void testTND280Context::test<5> () {
        CP::TND280Context g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Event number value is 0xDEADBEEF", 
                      g1.GetEvent(), invalid);
        ensure_equals("Event number value matches CP::TND280Context::Invalid", 
                      g1.GetEvent(), CP::TND280Context::Invalid);

    }

    // Test the default constructor and destructor for the spill.
    template<> template<>
    void testTND280Context::test<6> () {
        CP::TND280Context g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Spill number value is 0xDEADBEEF", 
                      g1.GetSpill(), invalid);
        ensure_equals("Spill number value matches CP::TND280Context::Invalid", 
                      g1.GetSpill(), CP::TND280Context::Invalid);

    }

    // Test the default constructor and destructor for the time stamp.
    template<> template<>
    void testTND280Context::test<7> () {
        CP::TND280Context g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Time stamp value is 0xDEADBEEF", 
                      g1.GetTimeStamp(), invalid);
        ensure_equals("Partition value matches CP::TND280Context::Invalid", 
                      g1.GetTimeStamp(), CP::TND280Context::Invalid);

    }

    //Test value constructor for the partition.
    template<> template<>
    void testTND280Context::test<8> () {
        CP::TND280Context g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Partition value is 1", 
                      g1.GetPartition(), value);

    }

    //Test value constructor for the run.
    template<> template<>
    void testTND280Context::test<9> () {
        CP::TND280Context g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Run value is 1", 
                      g1.GetRun(), value);

    }

    //Test value constructor for the sub run.
    template<> template<>
    void testTND280Context::test<10> () {
        CP::TND280Context g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Sub run value is 1", 
                      g1.GetSubRun(), value);

    }

    //Test value constructor for the event.
    template<> template<>
    void testTND280Context::test<11> () {
        CP::TND280Context g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Event number value is 1", 
                      g1.GetEvent(), value);

    }

    //Test value constructor for the spill.
    template<> template<>
    void testTND280Context::test<12> () {
        CP::TND280Context g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Spill number value is 1", 
                      g1.GetSpill(), value);

    }

    //Test value constructor for the time stamp.
    template<> template<>
    void testTND280Context::test<13> () {
        CP::TND280Context g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Time stamp value is 1", 
                      g1.GetTimeStamp(), value);

        ensure_equals("Time stamp value is '1970-01-01 00:00:01'", 
                      g1.GetTimeStampString(), "'1970-01-01 00:00:01'");

    }

};

