#include <tut.h>

#include "TND280Event.hxx"

namespace tut {
    struct baseHeader {
        baseHeader() {
            // Run before each test.
        }
        ~baseHeader() {
            // Run after each test.
        }
    };

    class WritableHeader : public ND::TND280Event::Header {
    public:
        WritableHeader() {
            fMCMSecond = 42;
        }
    };

    // Declare the test
    typedef test_group<baseHeader>::object testHeader;
    test_group<baseHeader> groupHeader("Header");

    // Test the default constructor and destructor.
    template<> template<>
    void testHeader::test<1> () {
        ND::TND280Event e1;

        ensure_equals("Default header values are Invalid",
                      e1.GetHeader().GetMCMSecond(),
                      ND::TND280Event::Header::Invalid);
    }

    // Test setting the header.
    template<> template<>
    void testHeader::test<2> () {
        ND::TND280Event e1;
        WritableHeader h1;

        ensure_equals("Check the writable header MCM second value",
                      h1.GetMCMSecond(),
                      (UInt_t) 42);
        
        e1.SetHeader() = h1;
        ensure_equals("Check the event header MCM second value",
                      e1.GetHeader().GetMCMSecond(),
                      (UInt_t) 42);
    }

};

