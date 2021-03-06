#ifndef TVertexState_hxx_seen
#define TVertexState_hxx_seen

#include "TReconState.hxx"
#include "TReconNode.hxx"

namespace CP {
    class TVertexState;
}

/// A state holding parameters associated with a TReconVertex.
class CP::TVertexState:
#ifndef __CINT__
    public TReconState,
    virtual public TMReconState,
    virtual public TMPositionState
#else
    public TReconState
#endif
{
public:
    TVertexState();
    virtual ~TVertexState();
    TVertexState(const TVertexState& init);
    virtual TVertexState& operator=(const TVertexState& rhs);

    /// Return the number of entries for the Direction in the TCorrValues
    /// vector.
    static int GetSize() {
        return TMPositionState::GetSize();
    }

    /// The projection operator to get the full state.
    static CP::TCorrValues ProjectState(const 
                                        CP::THandle<CP::TReconState>& state);
    
    ClassDef(TVertexState,2);
};
#endif
