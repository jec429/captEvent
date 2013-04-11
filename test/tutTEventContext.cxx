#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TEventContext.hxx"
#undef private
#undef protected

namespace tut {
    struct baseTEventContext {
        baseTEventContext() {
            // Run before each test.
        }
        ~baseTEventContext() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTEventContext>::object testTEventContext;
    test_group<baseTEventContext> groupTEventContext("TEventContext");

    // Test the default constructor and destructor.
    template<> template<>
    void testTEventContext::test<1> () {
        CP::TEventContext g1;

        int invalid = 0xdeadbeef;
        ensure_equals("Invald value matches 0XDEADBEEF",
                      CP::TEventContext::Invalid, invalid);
        ensure_equals("Invald value matches 0XDEADBEEF",
                      CP::TEventContext::Invalid, (int) 0XDEADBEEF);
    }

    // Test the default constructor and destructor for the partition.
    template<> template<>
    void testTEventContext::test<2> () {
        CP::TEventContext g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Partition value is 0xDEADBEEF", 
                      g1.GetPartition(), invalid);
        ensure_equals("Partition value matches CP::TEventContext::Invalid", 
                      g1.GetPartition(), CP::TEventContext::Invalid);

    }

    // Test the default constructor and destructor for the run.
    template<> template<>
    void testTEventContext::test<3> () {
        CP::TEventContext g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Run value is 0xDEADBEEF", 
                      g1.GetRun(), invalid);
        ensure_equals("Run value matches CP::TEventContext::Invalid", 
                      g1.GetRun(), CP::TEventContext::Invalid);

    }

    // Test the default constructor and destructor for the sub run.
    template<> template<>
    void testTEventContext::test<4> () {
        CP::TEventContext g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Sub run value is 0xDEADBEEF", 
                      g1.GetSubRun(), invalid);
        ensure_equals("Sub run value matches CP::TEventContext::Invalid", 
                      g1.GetSubRun(), CP::TEventContext::Invalid);

    }

    // Test the default constructor and destructor for the event.
    template<> template<>
    void testTEventContext::test<5> () {
        CP::TEventContext g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Event number value is 0xDEADBEEF", 
                      g1.GetEvent(), invalid);
        ensure_equals("Event number value matches CP::TEventContext::Invalid", 
                      g1.GetEvent(), CP::TEventContext::Invalid);

    }

    // Test the default constructor and destructor for the spill.
    template<> template<>
    void testTEventContext::test<6> () {
        CP::TEventContext g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Spill number value is 0xDEADBEEF", 
                      g1.GetSpill(), invalid);
        ensure_equals("Spill number value matches CP::TEventContext::Invalid", 
                      g1.GetSpill(), CP::TEventContext::Invalid);

    }

    // Test the default constructor and destructor for the time stamp.
    template<> template<>
    void testTEventContext::test<7> () {
        CP::TEventContext g1;
        int invalid = 0xdeadbeef;

        ensure_equals("Time stamp value is 0xDEADBEEF", 
                      g1.GetTimeStamp(), invalid);
        ensure_equals("Partition value matches CP::TEventContext::Invalid", 
                      g1.GetTimeStamp(), CP::TEventContext::Invalid);

    }

    //Test value constructor for the partition.
    template<> template<>
    void testTEventContext::test<8> () {
        CP::TEventContext g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Partition value is 1", 
                      g1.GetPartition(), value);

    }

    //Test value constructor for the run.
    template<> template<>
    void testTEventContext::test<9> () {
        CP::TEventContext g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Run value is 1", 
                      g1.GetRun(), value);

    }

    //Test value constructor for the sub run.
    template<> template<>
    void testTEventContext::test<10> () {
        CP::TEventContext g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Sub run value is 1", 
                      g1.GetSubRun(), value);

    }

    //Test value constructor for the event.
    template<> template<>
    void testTEventContext::test<11> () {
        CP::TEventContext g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Event number value is 1", 
                      g1.GetEvent(), value);

    }

    //Test value constructor for the spill.
    template<> template<>
    void testTEventContext::test<12> () {
        CP::TEventContext g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Spill number value is 1", 
                      g1.GetSpill(), value);

    }

    //Test value constructor for the time stamp.
    template<> template<>
    void testTEventContext::test<13> () {
        CP::TEventContext g1(1,1,1,1,1,1);
        int value = 1;

        ensure_equals("Time stamp value is 1", 
                      g1.GetTimeStamp(), value);

        ensure_equals("Time stamp value is '1970-01-01 00:00:01'", 
                      g1.GetTimeStampString(), "'1970-01-01 00:00:01'");

    }

};

