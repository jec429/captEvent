#include "TVertexState.hxx"

///////////////////////////////////////////////////////
ClassImp(ND::TVertexState);

ND::TVertexState::TVertexState(): TMReconState(this) {
    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));
    Init();
}

ND::TVertexState::~TVertexState() {}

ND::TVertexState::TVertexState(const ND::TVertexState& init)
  : TMReconState(this) {
    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));
    Init();

    for (int i=0; i<GetSize(); ++i) {
        SetValue(i,init.GetValue(i));
    }

    for (int i=0; i<GetSize(); ++i) {
        for (int j=0; j<GetSize(); ++j) {
            SetCovarianceValue(i,j,init.GetCovarianceValue(i,j));
        }
    }
    
}

ND::TVertexState& ND::TVertexState::operator=(const ND::TVertexState& rhs) {
    if (this == &rhs) return *this;

    for (int i=0; i<GetSize(); ++i) {
        SetValue(i,rhs.GetValue(i));
    }

    for (int i=0; i<GetSize(); ++i) {
        for (int j=0; j<GetSize(); ++j) {
            SetCovarianceValue(i,j,rhs.GetCovarianceValue(i,j));
        }
    }
    
    return *this;
}

ND::TCorrValues ND::TVertexState::ProjectState(
    const ND::THandle<TReconState>& proj) {
    TCorrValues values(TVertexState::GetSize());
    values.SetType("X Y Z T ");
    const TMPositionState* posState 
        = dynamic_cast<const TMPositionState*>(GetPointer(proj));
    int base = 0;
    if (posState) {
        const int offset = posState->GetPositionIndex();
        for (int i = 0; i < TMPositionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            posState->GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMPositionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    posState->GetThis().fValues.GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    return values;
}
