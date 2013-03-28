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
    struct baseTReconPID {
        baseTReconPID() {
            // Run before each test.
        }
        ~baseTReconPID() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTReconPID>::object testTReconPID;
    test_group<baseTReconPID> groupTReconPID("TReconPID");

    // Test the TReconPID constructor and destructor.
    template<> template<>
    void testTReconPID::test<1> () {
        ND::TReconPID recObj;

        ensure_distance("Default quality is zero", 
                        recObj.GetQuality(), 0.0, 
                        std::numeric_limits<double>::epsilon());

        ensure_equals("Default degrees of freedom",
                      recObj.GetNDOF(), 0);

        ensure("Default state",recObj.GetState());

        ND::THandle<ND::TPIDState> recState = recObj.GetState();
        ensure("Default state is TPIDState",recState);

        ND::THandle<ND::TClusterState> clusterState = recObj.GetState();
        ensure("Default state is not TClusterState",!clusterState);

        ensure_equals("Default TReconNodesContainer has size zero",
                      recObj.GetNodes().size(), (unsigned) 0);

        ensure("Default has no hits",!recObj.GetHits());
    }

    // Test the state definition.
    template<> template<>
    void testTReconPID::test<2> () {
        ND::TReconPID recObj;
        ND::THandle<ND::TPIDState> recState = recObj.GetState();

        ensure("The state is valid", recState);

        const int stateDim = recState->GetDimensions();
        ensure_equals("PID state dimensions", stateDim, 9);
    }

    // Test nodes which are saved have the correct type of state and all have
    // an associated object.
    template<> template<>
    void testTReconPID::test<3> () {
        ND::TReconPID recObj;
        ND::TReconNodeContainer& recNodes = recObj.GetNodes();
        ND::THandle<ND::TReconNode> recNode;
        ND::THandle<ND::TReconBase> recCluster(new ND::TReconCluster);
        ND::THandle<ND::TReconState> recState(new ND::TPIDState);

        ensure_equals("Node container starts empty.",recNodes.size(),
                      (unsigned) 0);

#ifdef CHECK_MISSING_STATE
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
    void testTReconPID::test<4> () {
        ND::TReconPID recObj;
        ND::THandle<ND::TPIDState> recState = recObj.GetState();

        ensure("The state is valid", recState);

        const double momentum = 1.414;
        recState->SetMomentum(momentum);

        const TLorentzVector pos(1.1,1.2,1.3,1.4);
        recState->SetPosition(pos);

        ensure_distance("PID momentum saved",
                        recObj.GetMomentum(), momentum, 0.00001);

        ensure_lessthan("PID position saved",
                        (recObj.GetPosition()-pos).Mag(), 0.0001);
    }
};
