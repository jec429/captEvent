#ifndef TShowerState_hxx_seen
#define TShowerState_hxx_seen

#include "TReconState.hxx"
#include "TReconNode.hxx"

namespace ND {
    class TShowerState;
}

/// A state holding the parameters associated with a TReconShower. 
class ND::TShowerState: public TReconState, 
                        virtual public TMReconState,
                        virtual public TMEDepositState,
                        virtual public TMPositionDirectionState,
                        virtual public TMConeState {
public:
    TShowerState();
    TShowerState(const TShowerState& init);
    virtual ~TShowerState();
    virtual TShowerState& operator=(const TShowerState& rhs);

    /// Return the number of entries for the Direction in the TCorrValues
    /// vector.
    static int GetSize() {
        return TMEDepositState::GetSize() 
            + TMPositionDirectionState::GetSize()
            + TMConeState::GetSize();
    }

    /// The projection operator to get the full state.
    static ND::TCorrValues ProjectState(const ND::THandle<ND::TReconState>& state);

    ClassDef(TShowerState,1);
};
#endif
