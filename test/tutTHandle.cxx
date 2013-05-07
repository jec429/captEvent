#include <iostream>
#include <tut.h>

#include "THit.hxx"
#include "TMCHit.hxx"
#include "THandle.hxx"
#include "THandleHack.hxx"

namespace tut {
    CP::THit* CanDelete(void) {
        CP::THit* p = new CP::TMCHit();
        p->SetBit(kCanDelete,true);
        return p;
    }
    CP::THit* CannotDelete(void) {
        CP::THit* p = new CP::TMCHit();
        p->SetBit(kCanDelete,false);
        return p;
    }
    struct baseTHandle {
        baseTHandle() {
            // Run before each test.
        }
        ~baseTHandle() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTHandle>::object testTHandle;
    test_group<baseTHandle> groupTHandle("THandle");

    // Test the default constructor and destructor.  Make sure the default
    // values are all null.
    template<> template<>
    void testTHandle::test<1> () {
        {
            CP::THandle<CP::THit> handle;
            ensure("Handle value is null", GetPointer(handle) == NULL);
            ensure("Handle is false", !handle);
            ensure("Handle is null", handle == NULL);
        }
        ensure("Handle Registry is clean", CP::CleanHandleRegistry());
    }

    /// Test the constructor and destructor.
    template<> template<>
    void testTHandle::test<2> () {
        {
            CP::THit* hit = CanDelete();
            CP::THandle<CP::THit> handle(hit);
            ensure("Handle is valid", handle);
            ensure_equals("Handle value is matchs pointer",
                          GetPointer(handle), hit);
        }
        ensure("Handle Registry is clean", CP::CleanHandleRegistry());
    }

    // Test the copy constructor.
    template <> template <>
    void testTHandle::test<3> () {
        {
            CP::THit* hit = CanDelete();
            CP::THandle<CP::THit> a(hit); 
            ensure("Copy from pointer", a);
            ensure_equals("Pointers equal", GetPointer(a), hit);
            
            CP::THandle<CP::THit> b(a); 
            ensure("Copy from handle", b);
            ensure("Handle copied pointer", GetPointer(b) == hit);
            ensure("Handles equal", a == b);
            
            CP::THandle<CP::TMCHit> c(a); 
            ensure("Copy from handle with down cast", b);
            ensure("Handle cast copied pointer", GetPointer(b) == hit);
            ensure("Handles equal after cast", a == b);
            
        }
        ensure("Handle Registry is clean", CP::CleanHandleRegistry());
    }
    
    // Test the assigments
    template <> template <>
    void testTHandle::test<4> () {
        CP::THit* hit = CanDelete();
        CP::THandle<CP::THit> a(hit); 
        ensure("Copy from pointer", a);
        ensure("Pointers equal", GetPointer(a) == hit);

        CP::THandle<CP::THit> b = a; 
        ensure("THandle<CP::THit> b = a", a == b);

        CP::THandle<CP::THit> c;
        c = a;
        ensure("Assign handle", c == a);

        CP::THandle<CP::TMCHit> d = c;
        ensure("Assign with cast", d == a);

        CP::THandle<CP::TMCHit> e;
        e = c;
        ensure("Assignment with down cast", e == d);
        
        CP::THandle<CP::THit> f;
        f = e;
        ensure("Assignment with up cast", f == a);

    }
    
    namespace {
        void ReferenceTest(const CP::THit& hit, std::string expectedType) {
            ensure_equals("Reference test hit type mismatch", 
                          hit.ClassName(), expectedType);
        }
    }

    // Test call by reference
    template <> template <>
    void testTHandle::test<5> () {
        CP::THit* hit = CanDelete();
        CP::THandle<CP::THit> a(hit);
        CP::THandle<CP::TMCHit> b(a);
        ReferenceTest(*a,"CP::TMCHit");
        ReferenceTest(*b,"CP::TMCHit");
    }

    namespace {
        void ValueTest(const CP::THandle<CP::THit> h, std::string expectedType) {
            ensure_equals("Value test hit type mismatch", 
                          h->ClassName(), expectedType);
        }
    }

    // Test call by reference
    template <> template <>
    void testTHandle::test<6> () {
        CP::THit* hit = CanDelete();
        CP::THandle<CP::THit> a(hit);
        CP::THandle<CP::TMCHit> b(a);
        ValueTest(a,"CP::TMCHit");
        ValueTest(b,"CP::TMCHit");
    }

    namespace {
        CP::THandle<CP::THit> ReturnTest() {
            CP::THit* hit = CanDelete();
            CP::THandle<CP::THit> a(hit);
            ensure_equals("Returned Pointer is OK", GetPointer(a), hit);
            return a; 
        }
    }

    // Test function return value.
    template <> template <>
    void testTHandle::test<7> () {
        {
            CP::THandle<CP::THit> a(ReturnTest());
            CP::THandle<CP::THit> b;
            b = ReturnTest();
            ensure("Returned handle was copied",a);
            ensure("Returned handle was assigned",b);
            ensure_equals("Returned class name",
                          a->ClassName(),
                          std::string("CP::TMCHit"));
            ensure_equals("Returned class name",
                          b->ClassName(),
                          std::string("CP::TMCHit"));
        }
        ensure("Handle Registry is clean", CP::CleanHandleRegistry());
    }
    
    // Test the less than operator.
    template <> template <>
    void testTHandle::test<8> () {
        {
            CP::THit* aPtr = CanDelete();
            CP::THit* bPtr = CanDelete();
            CP::THandle<CP::THit> a(aPtr);
            CP::THandle<CP::THit> b(bPtr);
            ensure("First pointer is valid",aPtr);
            ensure("First Handle is valid",a);
            ensure("Second pointer is valid",bPtr);
            ensure("Second Handle is valid",b);
            ensure("Pointers are not equal",aPtr != bPtr);
            ensure("Handles are not equal", !(a == b));
            if (aPtr < bPtr) {
                ensure("aPtr < bPtr && a < b",aPtr < bPtr && a < b);
                ensure("aPtr < bPtr && !(b < a)",aPtr < bPtr && !(b < a));
            }
            else {
                ensure("bPtr < aPtr && b < a",bPtr < aPtr && b < a);
                ensure("bPtr < aPtr && !(a < b)",bPtr < aPtr && !(a < b));
            }
        }
        ensure("Handle Registry is clean", CP::CleanHandleRegistry());
    }
};
