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
    struct baseTReconVertex {
        baseTReconVertex() {
            // Run before each test.
        }
        ~baseTReconVertex() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTReconVertex>::object testTReconVertex;
    test_group<baseTReconVertex> groupTReconVertex("TReconVertex");

    // Test the TReconVertex constructor and destructor.
    template<> template<>
    void testTReconVertex::test<1> () {
        ND::TReconVertex recObj;

        ensure_distance("Default quality is zero", 
                        recObj.GetQuality(), 0.0, 
                        std::numeric_limits<double>::epsilon());

        ensure_equals("Default degrees of freedom",
                      recObj.GetNDOF(), 0);

        ensure("Default state",recObj.GetState());

        ND::THandle<ND::TVertexState> recState = recObj.GetState();
        ensure("Default state is TVertexState",recState);

        ND::THandle<ND::TClusterState> clusterState = recObj.GetState();
        ensure("Default state is not TClusterState",!clusterState);

        ensure_equals("Default TReconNodesContainer has size zero",
                      recObj.GetNodes().size(), (unsigned) 0);

        ensure("Default has no hits",!recObj.GetHits());
    }

    // Test the state definition.
    template<> template<>
    void testTReconVertex::test<2> () {
        ND::TReconVertex recObj;
        ND::THandle<ND::TVertexState> recState = recObj.GetState();

        ensure("The state is valid", recState);

        const int stateDim = recState->GetDimensions();
        ensure_equals("Vertex state dimensions", stateDim, 4);
    }

    // Test nodes which are saved have the correct type of state and all have
    // an associated object.
    template<> template<>
    void testTReconVertex::test<3> () {
        ND::TReconVertex recObj;
        ND::TReconNodeContainer& recNodes = recObj.GetNodes();
        ND::THandle<ND::TReconNode> recNode;
        ND::THandle<ND::TReconBase> recCluster(new ND::TReconCluster);
        ND::THandle<ND::TReconState> recState(new ND::TVertexState);

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
    void testTReconVertex::test<4> () {
        ND::TReconVertex recObj;
        ND::THandle<ND::TVertexState> recState = recObj.GetState();

        ensure("The state is valid", recState);

        const TLorentzVector pos(1.1,1.2,1.3,1.4);
        recState->SetPosition(pos);

        ensure_lessthan("Vertex position saved",
                        (recObj.GetPosition()-pos).Mag(), 0.0001);
    }
};
