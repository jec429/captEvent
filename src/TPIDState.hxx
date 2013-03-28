#ifndef TPIDState_hxx_seen
#define TPIDState_hxx_seen

#include "TReconState.hxx"
#include "TTrackState.hxx"
#include "TShowerState.hxx"
#include "TReconNode.hxx"

namespace ND {
    class TPIDState;
}
    
/// A state holding parameters associated with a TReconPID.  
class ND::TPIDState: public TReconState, 
                     virtual public TMReconState,
                     virtual public TMPositionDirectionState, 
                     virtual public TMMomentumState,
                     virtual public TMChargeState {
public:
    TPIDState();
    TPIDState(const ND::TPIDState& init);
    TPIDState(const ND::TTrackState& tstate);
    TPIDState(const ND::TShowerState& tstate);

    virtual ~TPIDState();
    virtual TPIDState& operator=(const ND::TPIDState& rhs);

    /// Return the number of entries for the Direction in the TCorrValues
    /// vector.
    static int GetSize() {
        return TMPositionDirectionState::GetSize() 
            + TMMomentumState::GetSize()
            + TMChargeState::GetSize();
    }

    /// The projection operator to get the full state.
    static ND::TCorrValues ProjectState(const ND::THandle<ND::TReconState>& state);

    ClassDef(TPIDState,1);
};
#endif
