#ifndef TShowerState_hxx_seen
#define TShowerState_hxx_seen

#include "TReconState.hxx"
#include "TReconNode.hxx"

namespace CP {
    class TShowerState;
}

/// A state holding the parameters associated with a TReconShower. 
class CP::TShowerState: public TReconState, 
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
    static CP::TCorrValues ProjectState(const CP::THandle<CP::TReconState>& state);

    ClassDef(TShowerState,1);
};
#endif
