#include <iostream>
#include <tut.h>

#include "THit.hxx"
#include "TMCHit.hxx"
#include "TComboHit.hxx"
#include "THandle.hxx"
#include "THandleHack.hxx"

namespace tut {
    ND::THit* CanDelete(void) {
        ND::THit* p = new ND::TMCHit();
        p->SetBit(kCanDelete,true);
        return p;
    }
    ND::THit* CannotDelete(void) {
        ND::THit* p = new ND::TMCHit();
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
            ND::THandle<ND::THit> handle;
            ensure("Handle value is null", GetPointer(handle) == NULL);
            ensure("Handle is false", !handle);
            ensure("Handle is null", handle == NULL);
        }
        ensure("Handle Registry is clean", ND::CleanHandleRegistry());
    }

    /// Test the constructor and destructor.
    template<> template<>
    void testTHandle::test<2> () {
        {
            ND::THit* hit = CanDelete();
            ND::THandle<ND::THit> handle(hit);
            ensure("Handle is valid", handle);
            ensure_equals("Handle value is matchs pointer",
                          GetPointer(handle), hit);
        }
        ensure("Handle Registry is clean", ND::CleanHandleRegistry());
    }

    // Test the copy constructor.
    template <> template <>
    void testTHandle::test<3> () {
        {
            ND::THit* hit = CanDelete();
            ND::THandle<ND::THit> a(hit); 
            ensure("Copy from pointer", a);
            ensure_equals("Pointers equal", GetPointer(a), hit);
            
            ND::THandle<ND::THit> b(a); 
            ensure("Copy from handle", b);
            ensure("Handle copied pointer", GetPointer(b) == hit);
            ensure("Handles equal", a == b);
            
            ND::THandle<ND::TMCHit> c(a); 
            ensure("Copy from handle with down cast", b);
            ensure("Handle cast copied pointer", GetPointer(b) == hit);
            ensure("Handles equal after cast", a == b);
            
            ND::THandle<ND::TComboHit> combo(a);
            ensure("Copy from ND::TMCHit to ND::TComboHit should produce null",!combo);
        }
        ensure("Handle Registry is clean", ND::CleanHandleRegistry());
    }
    
    // Test the assigments
    template <> template <>
    void testTHandle::test<4> () {
        ND::THit* hit = CanDelete();
        ND::THandle<ND::THit> a(hit); 
        ensure("Copy from pointer", a);
        ensure("Pointers equal", GetPointer(a) == hit);

        ND::THandle<ND::THit> b = a; 
        ensure("THandle<ND::THit> b = a", a == b);

        ND::THandle<ND::THit> c;
        c = a;
        ensure("Assign handle", c == a);

        ND::THandle<ND::TMCHit> d = c;
        ensure("Assign with cast", d == a);

        ND::THandle<ND::TMCHit> e;
        e = c;
        ensure("Assignment with down cast", e == d);
        
        ND::THandle<ND::THit> f;
        f = e;
        ensure("Assignment with up cast", f == a);

        ND::THandle<ND::TComboHit> g = a;
        ensure("Assignment with down cast to incompatible base",!g);

        ND::THandle<ND::TComboHit> h;
        h = a;
        ensure("Assign handle with down cast to incompatible base",!h);

    }
    
    namespace {
        void ReferenceTest(const ND::THit& hit, std::string expectedType) {
            ensure_equals("Reference test hit type mismatch", 
                          hit.ClassName(), expectedType);
        }
    }

    // Test call by reference
    template <> template <>
    void testTHandle::test<5> () {
        ND::THit* hit = CanDelete();
        ND::THandle<ND::THit> a(hit);
        ND::THandle<ND::TMCHit> b(a);
        ReferenceTest(*a,"ND::TMCHit");
        ReferenceTest(*b,"ND::TMCHit");
    }

    namespace {
        void ValueTest(const ND::THandle<ND::THit> h, std::string expectedType) {
            ensure_equals("Value test hit type mismatch", 
                          h->ClassName(), expectedType);
        }
    }

    // Test call by reference
    template <> template <>
    void testTHandle::test<6> () {
        ND::THit* hit = CanDelete();
        ND::THandle<ND::THit> a(hit);
        ND::THandle<ND::TMCHit> b(a);
        ValueTest(a,"ND::TMCHit");
        ValueTest(b,"ND::TMCHit");
    }

    namespace {
        ND::THandle<ND::THit> ReturnTest() {
            ND::THit* hit = CanDelete();
            ND::THandle<ND::THit> a(hit);
            ensure_equals("Returned Pointer is OK", GetPointer(a), hit);
            return a; 
        }
    }

    // Test function return value.
    template <> template <>
    void testTHandle::test<7> () {
        {
            ND::THandle<ND::THit> a(ReturnTest());
            ND::THandle<ND::THit> b;
            b = ReturnTest();
            ensure("Returned handle was copied",a);
            ensure("Returned handle was assigned",b);
            ensure_equals("Returned class name",
                          a->ClassName(),
                          std::string("ND::TMCHit"));
            ensure_equals("Returned class name",
                          b->ClassName(),
                          std::string("ND::TMCHit"));
        }
        ensure("Handle Registry is clean", ND::CleanHandleRegistry());
    }
    
    // Test the less than operator.
    template <> template <>
    void testTHandle::test<8> () {
        {
            ND::THit* aPtr = CanDelete();
            ND::THit* bPtr = CanDelete();
            ND::THandle<ND::THit> a(aPtr);
            ND::THandle<ND::THit> b(bPtr);
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
        ensure("Handle Registry is clean", ND::CleanHandleRegistry());
    }
};
