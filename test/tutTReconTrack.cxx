#include <iostream>
#include <tut.h>

#include "TReconBase.hxx"
#include "TReconNode.hxx"
#include "TReconState.hxx"
#include "TReconCluster.hxx"
#include "TReconShower.hxx"
#include "TReconTrack.hxx"
#include "TReconVertex.hxx"
#include "TReconPID.hxx"

namespace tut {
    struct baseTReconTrack {
        baseTReconTrack() {
            // Run before each test.
        }
        ~baseTReconTrack() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTReconTrack>::object testTReconTrack;
    test_group<baseTReconTrack> groupTReconTrack("TReconTrack");

    // Test the TReconTrack constructor and destructor.
    template<> template<>
    void testTReconTrack::test<1> () {
        ND::TReconTrack recObj;

        ensure_distance("Default quality is zero", 
                        recObj.GetQuality(), 0.0, 
                        std::numeric_limits<double>::epsilon());

        ensure_equals("Default degrees of freedom",
                      recObj.GetNDOF(), 0);

        ensure("Default state",recObj.GetState());

        ND::THandle<ND::TTrackState> recState = recObj.GetState();
        ensure("Default state is TTrackState",recState);

        ND::THandle<ND::TClusterState> clusterState = recObj.GetState();
        ensure("Default state is not TClusterState",!clusterState);

        ensure_equals("Default TReconNodesContainer has size zero",
                      recObj.GetNodes().size(), (unsigned) 0);

        ensure("Default has no hits",!recObj.GetHits());
    }

    // Test the state definition.
    template<> template<>
    void testTReconTrack::test<2> () {
        ND::TReconTrack recObj;
        ND::THandle<ND::TTrackState> recState = recObj.GetState();

        ensure("The state is valid", recState);

        const int stateDim = recState->GetDimensions();
        ensure_equals("Track state dimensions", stateDim, 11);
    }

    // Test nodes which are saved have the correct type of state and all have
    // an associated object.
    template<> template<>
    void testTReconTrack::test<3> () {
        ND::TReconTrack recObj;
        ND::TReconNodeContainer& recNodes = recObj.GetNodes();
        ND::THandle<ND::TReconNode> recNode;
        ND::THandle<ND::TReconBase> recCluster(new ND::TReconCluster);
        ND::THandle<ND::TReconState> recState(new ND::TTrackState);

        ensure_equals("Node container starts empty.",recNodes.size(),
                      (unsigned)0);

#ifdef MISSING_STATE_CHECK
        try {
            recNode = ND::THandle<ND::TReconNode>(new ND::TReconNode);
            recNode->SetObject(recCluster);
            recNodes.push_back(recNode);
            fail("TReconNodeContainer node state is empty");
        } 
        catch (ND::EWrongStateType& ex) {
            // OK!
            std::cout << std::endl << "Caught " << ex.what() << std::endl;
        }
#endif

        try {
            recNode = ND::THandle<ND::TReconNode>(new ND::TReconNode);
            recNode->SetState(recState);
            recNodes.push_back(recNode);
            recNode->ls();
            fail("TReconNodeContainer object is empty");
        } 
        catch (ND::EObjectEmpty& ex) {
            // OK!
            std::cout << std::endl << "Caught " << ex.what() << std::endl;
        }

        try {
            recNode = ND::THandle<ND::TReconNode>(new ND::TReconNode);
            recNode->SetObject(recCluster);
            recNode->SetState(recState);
            recNodes.push_back(recNode);
        }
        catch (...) {
            fail("TReconNodeContainer failed with correct object and state");
        }
    }

    // Test the getters and setters.
    template<> template<>
    void testTReconTrack::test<4> () {
        ND::TReconTrack recObj;
        ND::THandle<ND::TTrackState> recState = recObj.GetState();

        ensure("The state is valid", recState);

        const double eDeposit = 1.414;
        recState->SetEDeposit(eDeposit);

        const TLorentzVector pos(1.1,1.2,1.3,1.4);
        recState->SetPosition(pos);

        TVector3 dir(0.1, 0.2, 0.3);
        dir = dir.Unit();
        recState->SetDirection(dir);

        const double curvature = 2.17;
        recState->SetCurvature(curvature);

        const TVector3 width(0.3, 0.4, 0.0);
        recState->SetWidth(width.X(), width.Y());
        
        ensure_distance("Track energy deposit saved",
                        recObj.GetEDeposit(), eDeposit, 0.00001);

        ensure_lessthan("Track position saved",
                        (recObj.GetPosition()-pos).Mag(), 0.0001);

        ensure_lessthan("Track direction saved",
                        (recObj.GetDirection()-dir).Mag(), 0.0001);

        ensure_distance("Track curvature saved",
                        recObj.GetCurvature(), curvature, 0.00001);

        ensure_lessthan("Track width saved",
                        (recObj.GetWidth()-width).Mag(), 0.0001);

    }

    // Test the getters and setters.
    template<> template<>
    void testTReconTrack::test<5> () {
        ND::TReconTrack recObj;
        ND::THandle<ND::TTrackState> recState = recObj.GetState();

        ensure("The state is valid", recState);

        const TLorentzVector pos(1.1,1.2,1.3,1.4);
        recState->SetPosition(pos);

        ensure_distance("Track X position and value",
                        recState->GetPosition().X(),
                        recState->GetValue(recState->GetPositionIndex()),
                        0.01);
        ensure_distance("Track Y position and value",
                        recState->GetPosition().Y(),
                        recState->GetValue(recState->GetPositionIndex()+1),
                        0.01);
        ensure_distance("Track Z position and value",
                        recState->GetPosition().Z(),
                        recState->GetValue(recState->GetPositionIndex()+2),
                        0.01);
        ensure_distance("Track T position and value",
                        recState->GetPosition().T(),
                        recState->GetValue(recState->GetPositionIndex()+3),
                        0.01);

        ensure_lessthan("Track position saved",
                        (recObj.GetPosition()-pos).Mag(), 0.0001);

    }
};
