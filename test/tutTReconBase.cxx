#include "TReconBase.hxx"
#include "TReconCluster.hxx"
#include "TIntegerDatum.hxx"
#include "TRealDatum.hxx"

#include <tut.h>

#include <iostream>
#include <sstream>
#include <string>


namespace tut {
    struct baseTReconBase {
        baseTReconBase() {
            // Run before each test.
        }

        ~baseTReconBase() {
            // Run after each test.
        }
        
    };

    // Declare the test
    typedef test_group<baseTReconBase>::object testTReconBase;
    test_group<baseTReconBase> groupTReconBase("TReconBase");

    // Test the basic constructor and destructor.
    template<> template <>
    void testTReconBase::test<1> () {
        CP::TReconCluster base;

        ensure_equals("Default status is empty",base.GetStatus(),(unsigned)0);
        ensure_equals("Default quality is zero",base.GetQuality(),0);
        ensure_equals("Default degrees of freedom are zero",base.GetNDOF(),0);
        ensure("No default constituents", !base.GetConstituents());
    }

    // Test SetStatus(), ClearStatus(), and CheckStatus().
    template<> template <>
    void testTReconBase::test<2> () {
        CP::TReconCluster base;

        ensure("No status bit is set", 
               !base.CheckStatus(CP::TReconBase::kStatusMask));
        ensure("Default kRan is false",
               !base.CheckStatus(CP::TReconBase::kRan));
        base.SetStatus(CP::TReconBase::kRan);
        ensure("The kRan bit is true",
               base.CheckStatus(CP::TReconBase::kRan));
        ensure("Bit in status mask is set",
               base.CheckStatus(CP::TReconBase::kStatusMask));
        ensure("The kSuccess bit is false",
               !base.CheckStatus(CP::TReconBase::kSuccess));
        ensure("The kChi2Fit bit is false",
               !base.CheckStatus(CP::TReconBase::kChi2Fit));
        ensure("The kLikelihoodFit bit is false",
               !base.CheckStatus(CP::TReconBase::kLikelihoodFit));
        ensure("The kKalman bit is false",
               !base.CheckStatus(CP::TReconBase::kKalmanFit));
        ensure("Check several bits (including kRan) is true.",
               base.CheckStatus(CP::TReconBase::kRan
                                | CP::TReconBase::kSuccess));
        ensure("Check several bits (not including kRan) are false",
               !base.CheckStatus(CP::TReconBase::kSuccess
                                 | CP::TReconBase::kChi2Fit));
        base.ClearStatus(CP::TReconBase::kRan);
        ensure("The kRan Bit is cleared",
               !base.CheckStatus(CP::TReconBase::kRan));
        ensure("No status bit set after clear", 
               !base.CheckStatus(CP::TReconBase::kStatusMask));
    }

    // Check the detector bits.
    template<> template <>
    void testTReconBase::test<3> () {
        CP::TReconCluster base;
        
        ensure("No detector used by default",
               !base.UsesDetector(CP::TReconBase::kDetectorMask));


        ensure("TPC is not used by default",
               !base.UsesDetector(CP::TReconBase::kTPC));

        base.AddDetector(CP::TReconBase::kTPC);
        ensure("TPC was added",
               base.UsesDetector(CP::TReconBase::kTPC));

        base.RemoveDetector(CP::TReconBase::kTPC);
        ensure("TPC was removed",
               !base.UsesDetector(CP::TReconBase::kTPC));

    }

    // Check the copy constructor.
    template<> template <>
    void testTReconBase::test<4> () {
        CP::TReconCluster orig;
        orig.SetName("name");
        orig.SetTitle("title");
        orig.SetMoments(1,2,3,4,5,6);
        CP::TIntegerDatum* iDatum 
            = new CP::TIntegerDatum("integerDatum","title");
        orig.AddDatum(iDatum);

        CP::THandle<CP::TIntegerDatum> origInt 
            = orig.Get<CP::TIntegerDatum>("integerDatum");
        (*origInt)[0] = 1;
        origInt->push_back(2);
        origInt->push_back(3);

        CP::TReconCluster copy(orig);

        ensure_equals("Name of orig and copy are the same",
                      std::string(copy.GetName()),
                      std::string(orig.GetName()));

        ensure_equals("Title of orig and copy are the same",
                      std::string(copy.GetTitle()),
                      std::string(orig.GetTitle()));

        CP::THandle<CP::TIntegerDatum> copyInt 
            = copy.Get<CP::TIntegerDatum>("integerDatum");

        ensure_equals("Integer datums have the same titles",
                      std::string(copyInt->GetTitle()),
                      std::string(origInt->GetTitle()));

        for (int i=0; i<3; ++i) {
            ensure_equals("Original integer datum has correct value",
                          (*origInt)[i],i+1);
            ensure_equals("Copy integer datum has correct value",
                          (*copyInt)[i],i+1);
            ensure_equals("Integer datums have the same value",
                          (*copyInt)[0], (*origInt)[0]);
        }

    }

    // Check the copy constructor.
    template<> template <>
    void testTReconBase::test<5> () {
        CP::TReconCluster orig;
        orig.SetName("name");
        orig.SetTitle("title");
        orig.SetMoments(1,2,3,4,5,6);
        CP::TRealDatum* iDatum 
            = new CP::TRealDatum("realDatum","title");
        orig.AddDatum(iDatum);

        CP::THandle<CP::TRealDatum> origReal 
            = orig.Get<CP::TRealDatum>("realDatum");
        (*origReal)[0] = 1;
        origReal->push_back(2);
        origReal->push_back(3);

        CP::TReconCluster copy(orig);

        ensure_equals("Name of orig and copy are the same",
                      std::string(copy.GetName()),
                      std::string(orig.GetName()));

        ensure_equals("Title of orig and copy are the same",
                      std::string(copy.GetTitle()),
                      std::string(orig.GetTitle()));

        CP::THandle<CP::TRealDatum> copyReal 
            = copy.Get<CP::TRealDatum>("realDatum");

        ensure_equals("Real datums have the same titles",
                      std::string(copyReal->GetTitle()),
                      std::string(origReal->GetTitle()));

        for (int i=0; i<3; ++i) {
            ensure_distance("Original real datum has correct value",
                            (*origReal)[i],i+1.0,1E-6);
            ensure_distance("Copy real datum has correct value",
                            (*copyReal)[i],i+1.0,1E-6);
            ensure_equals("Real datums have the same value",
                          (*copyReal)[0], (*origReal)[0]);
        }

    }


};
