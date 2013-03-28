#ifndef TVertexState_hxx_seen
#define TVertexState_hxx_seen

#include "TReconState.hxx"
#include "TReconNode.hxx"

namespace ND {
    class TVertexState;
}

/// A state holding parameters associated with a TReconVertex.
class ND::TVertexState: public TReconState,
                        virtual public TMReconState,
                        virtual public TMPositionState {
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
    static ND::TCorrValues ProjectState(const 
                                        ND::THandle<ND::TReconState>& state);
    
    ClassDef(TVertexState,1);
};
#endif
