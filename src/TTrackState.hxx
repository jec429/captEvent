#ifndef TTrackState_hxx_seen
#define TTrackState_hxx_seen

#include "TReconState.hxx"
#include "TReconNode.hxx"

namespace CP {
    class TTrackState;
}

/// A state holding parameters associated with a TReconTrack, and the
/// intermediate states.
class CP::TTrackState:
#ifndef __CINT__
    public TReconState,
    virtual public TMReconState,
    virtual public TMEDepositState,
    virtual public TMPosDirMassState,
    virtual public TMWidthState
#else
    public TReconState
#endif
{
public:
    TTrackState();
    TTrackState(const TTrackState& init);
    virtual ~TTrackState();
    virtual TTrackState& operator=(const TTrackState& rhs);

    /// Return the number of entries for the Direction in the TCorrValues
    /// vector.
    static int GetSize() {
        return TMEDepositState::GetSize() 
            + TMPosDirMassState::GetSize()
            + TMWidthState::GetSize();
    }

    /// The projection operator to get the full state.
    static CP::TCorrValues ProjectState(const CP::THandle<CP::TReconState>& state);

    ClassDef(TTrackState,2);
};

#endif
