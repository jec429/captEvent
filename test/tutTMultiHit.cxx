#include <iostream>
#include <tut.h>

#include "THit.hxx"
#include "TMCHit.hxx"
#include "TMultiHit.hxx"
#include "THandle.hxx"
#include "THandleHack.hxx"
#include "ND280GeomId.hxx"

namespace tut {
    CP::THandle<CP::TSingleHit> MakeHit(int index) {
        CP::TWritableMCHit hit;
        hit.SetGeomId(CP::GeomId::FGD::Bar(0,0,0,0));
        hit.SetTime(1.0*index);
        hit.SetCharge(1);
        CP::TMCHit *p = new CP::TMCHit(hit);
        p->SetBit(kCanDelete,true);
        return CP::THandle<CP::TSingleHit>(p);
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
        CP::TMultiHit hit;
        ensure("Multi hit is empty", hit.size() == 0);
        ensure("Iterators are 'void'", hit.begin() == hit.end());
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetGeomId()
    template<> template<>
    void testTMultiHit::test<2> () {
        CP::TMultiHit hit;
        try {
            hit.GetGeomId();
            ensure("Empty hit throws exception for GetGeomId()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetCharge()
    template<> template<>
    void testTMultiHit::test<4> () {
        CP::TMultiHit hit;
        try {
            hit.GetCharge();
            ensure("Empty multi-hit throws exception for GetCharge()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetTime()
    template<> template<>
    void testTMultiHit::test<5> () {
        CP::TMultiHit hit;
        try {
            hit.GetTime();
            ensure("Empty multi-hit throws exception for GetTime()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetPosition()
    template<> template<>
    void testTMultiHit::test<6> () {
        CP::TMultiHit hit;
        try {
            hit.GetPosition();
            ensure("Empty multi-hit throws exception for GetPosition()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through IsXHit()
    template<> template<>
    void testTMultiHit::test<7> () {
        CP::TMultiHit hit;
        try {
            hit.IsXHit();
            ensure("Empty multi-hit throws exception for IsXHit()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through IsYHit()
    template<> template<>
    void testTMultiHit::test<8> () {
        CP::TMultiHit hit;
        try {
            hit.IsYHit();
            ensure("Empty multi-hit throws exception for IsYHit()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through IsZHit()
    template<> template<>
    void testTMultiHit::test<9> () {
        CP::TMultiHit hit;
        try {
            hit.IsZHit();
            ensure("Empty multi-hit throws exception for IsZHit()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetSpread()
    template<> template<>
    void testTMultiHit::test<10> () {
        CP::TMultiHit hit;
        try {
            hit.GetSpread();
            ensure("Empty multi-hit throws exception for GetSpread()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Make sure that the right exception is thrown when an empty hit is
    // accessed through GetUncertainty()
    template<> template<>
    void testTMultiHit::test<11> () {
        CP::TMultiHit hit;
        try {
            hit.GetUncertainty();
            ensure("Empty multi-hit throws for GetUncertainty()",false);
        }
        catch (CP::EMultiHitIllegal) {
            /// This should happen!
        }
    }

    // Construct a CP::TMultiHit with actual entries and make sure the accessors
    // work.  This can only check some 
    template<> template<>
    void testTMultiHit::test<12> () {
        {
            // Build a vector of single hits.
            std::vector< CP::THandle<CP::TSingleHit> > hits;
            for (int i=0; i<10; ++i) hits.push_back(MakeHit(20-i));
            CP::TMultiHit hit(hits.begin(),hits.end());
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
            for (CP::TMultiHit::iterator h = hit.begin(); h != hit.end(); ++h) {
                ++hitCount;
                ensure_lessthan("hit time is increasing",time,(*h)->GetTime());
                time = (*h)->GetTime();
            }
            ensure_equals("Same number of hits visited as hit.size()",
                          hitCount, hit.size());
        }
        ensure("Handle Registry is clean", CP::CleanHandleRegistry());
    }
};
