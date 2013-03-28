#include <iostream>
#include <tut.h>

#include "THit.hxx"
#include "TMCHit.hxx"
#include "TMultiHit.hxx"
#include "THandle.hxx"
#include "THandleHack.hxx"
#include "ND280GeomId.hxx"

namespace tut {
    ND::THandle<ND::TSingleHit> MakeHit(int index) {
        ND::TWritableMCHit hit;
        hit.SetGeomId(ND::GeomId::FGD::Bar(0,0,0,0));
        hit.SetTime(1.0*index);
        hit.SetCharge(1);
        ND::TMCHit *p = new ND::TMCHit(hit);
        p->SetBit(kCanDelete,true);
        return ND::THandle<ND::TSingleHit>(p);
    }

    struct baseTMultiHit {
        baseTMultiHit() {
            // Run before each test.
        }
        ~baseTMultiHit() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTMultiHit>::object testTMultiHit;
    test_group<baseTMultiHit> groupTMultiHit("TMultiHit");

    // Test the default constructor and destructor.  Make sure the default
    // values are all null.
    template<> template<>
    void testTMultiHit::test<1> () {
        ND::TMultiHit hit;
        ensure("Multi hit is empty", hit.size() == 0);
        ensure("Iterators are 'void'", hit.begin() == hit.end());
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetGeomId()
    template<> template<>
    void testTMultiHit::test<2> () {
        ND::TMultiHit hit;
        try {
            hit.GetGeomId();
            ensure("Empty hit throws exception for GetGeomId()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetCharge()
    template<> template<>
    void testTMultiHit::test<4> () {
        ND::TMultiHit hit;
        try {
            hit.GetCharge();
            ensure("Empty multi-hit throws exception for GetCharge()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetTime()
    template<> template<>
    void testTMultiHit::test<5> () {
        ND::TMultiHit hit;
        try {
            hit.GetTime();
            ensure("Empty multi-hit throws exception for GetTime()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetPosition()
    template<> template<>
    void testTMultiHit::test<6> () {
        ND::TMultiHit hit;
        try {
            hit.GetPosition();
            ensure("Empty multi-hit throws exception for GetPosition()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through IsXHit()
    template<> template<>
    void testTMultiHit::test<7> () {
        ND::TMultiHit hit;
        try {
            hit.IsXHit();
            ensure("Empty multi-hit throws exception for IsXHit()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through IsYHit()
    template<> template<>
    void testTMultiHit::test<8> () {
        ND::TMultiHit hit;
        try {
            hit.IsYHit();
            ensure("Empty multi-hit throws exception for IsYHit()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through IsZHit()
    template<> template<>
    void testTMultiHit::test<9> () {
        ND::TMultiHit hit;
        try {
            hit.IsZHit();
            ensure("Empty multi-hit throws exception for IsZHit()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetSpread()
    template<> template<>
    void testTMultiHit::test<10> () {
        ND::TMultiHit hit;
        try {
            hit.GetSpread();
            ensure("Empty multi-hit throws exception for GetSpread()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetUncertainty()
    template<> template<>
    void testTMultiHit::test<11> () {
        ND::TMultiHit hit;
        try {
            hit.GetUncertainty();
            ensure("Empty multi-hit throws for GetUncertainty()",false);
        }
        catch (ND::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Construct a ND::TMultiHit with actual entries and make sure the accessors
    // work.  This can only check some 
    template<> template<>
    void testTMultiHit::test<12> () {
        {
            // Build a vector of single hits.
            std::vector< ND::THandle<ND::TSingleHit> > hits;
            for (int i=0; i<10; ++i) hits.push_back(MakeHit(20-i));
            ND::TMultiHit hit(hits.begin(),hits.end());
            ensure_equals("MultiHit size is correct", hit.size(), (unsigned) 10);
            // Make sure that none of the accessors throw an exception.
            hit.GetGeomId();
            ensure_distance("hit charge same as size",hit.GetCharge(),10.0,0.1);
            ensure_lessthan("overall hit time not zero", 10, hit.GetTime());
            hit.GetPosition();
            hit.IsXHit();
            hit.IsYHit();
            hit.IsZHit();
            hit.GetSpread();
            hit.GetUncertainty();
            
            unsigned int hitCount = 0;
            double time = 0;
            for (ND::TMultiHit::iterator h = hit.begin(); h != hit.end(); ++h) {
                ++hitCount;
                ensure_lessthan("hit time is increasing",time,(*h)->GetTime());
                time = (*h)->GetTime();
            }
            ensure_equals("Same number of hits visited as hit.size()",
                          hitCount, hit.size());
        }
        ensure("Handle Registry is clean", ND::CleanHandleRegistry());
    }
};
