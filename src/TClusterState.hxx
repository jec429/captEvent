#ifndef TClusterState_hxx_seen
#define TClusterState_hxx_seen

#include "TReconState.hxx"
#include "TReconNode.hxx"

namespace CP {
    class TClusterState;
}

/// A state holding the parameters associated with a TReconCluster.  
class CP::TClusterState: public TReconState, 
                         virtual public TMReconState,
                         virtual public TMEDepositState,
                         virtual public TMPositionState {
public:
    TClusterState();
    virtual ~TClusterState();
    TClusterState(const TClusterState& init);
    virtual TClusterState& operator=(const TClusterState& rhs);

    /// Return the number of entries for the Direction in the TCorrValues
    /// vector.
    static int GetSize() {
        return TMEDepositState::GetSize() + TMPositionState::GetSize();
    }

    /// The projection operator to get the full state.
    static CP::TCorrValues ProjectState(const CP::THandle<CP::TReconState>& state);

    ClassDef(TClusterState,1);
};
#endif
