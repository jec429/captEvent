#ifndef TPIDState_hxx_seen
#define TPIDState_hxx_seen

#include "TReconState.hxx"
#include "TTrackState.hxx"
#include "TShowerState.hxx"
#include "TReconNode.hxx"

namespace CP {
    class TPIDState;
}
    
/// A state holding parameters associated with a TReconPID.  
class CP::TPIDState: public TReconState, 
                     virtual public TMReconState,
                     virtual public TMPositionDirectionState, 
                     virtual public TMMomentumState,
                     virtual public TMChargeState {
public:
    TPIDState();
    TPIDState(const CP::TPIDState& init);
    TPIDState(const CP::TTrackState& tstate);
    TPIDState(const CP::TShowerState& tstate);

    virtual ~TPIDState();
    virtual TPIDState& operator=(const CP::TPIDState& rhs);

    /// Return the number of entries for the Direction in the TCorrValues
    /// vector.
    static int GetSize() {
        return TMPositionDirectionState::GetSize() 
            + TMMomentumState::GetSize()
            + TMChargeState::GetSize();
    }

    /// The projection operator to get the full state.
    static CP::TCorrValues ProjectState(const CP::THandle<CP::TReconState>& state);

    ClassDef(TPIDState,1);
};
#endif
