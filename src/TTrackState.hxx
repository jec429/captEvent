#ifndef TTrackState_hxx_seen
#define TTrackState_hxx_seen

#include "TReconState.hxx"
#include "TReconNode.hxx"

namespace ND {
    class TTrackState;
}

/// A state holding parameters associated with a TReconTrack, and the
/// intermediate states.
class ND::TTrackState: public TReconState,
                       virtual public TMReconState,
                       virtual public TMEDepositState,
                       virtual public TMPosDirCurvState,
                       virtual public TMWidthState {
public:
    TTrackState();
    TTrackState(const TTrackState& init);
    virtual ~TTrackState();
    virtual TTrackState& operator=(const TTrackState& rhs);

    /// Return the number of entries for the Direction in the TCorrValues
    /// vector.
    static int GetSize() {
        return TMEDepositState::GetSize() 
            + TMPosDirCurvState::GetSize()
            + TMWidthState::GetSize();
    }

    /// The projection operator to get the full state.
    static ND::TCorrValues ProjectState(const ND::THandle<ND::TReconState>& state);

    ClassDef(TTrackState,1);
};

#endif
