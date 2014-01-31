#include "TTrackState.hxx"

///////////////////////////////////////////////////////
ClassImp(CP::TTrackState);

CP::TTrackState::TTrackState(): TMReconState(this) {
    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));
    Init();
}

CP::TTrackState::~TTrackState() {}

CP::TTrackState::TTrackState(const CP::TTrackState& init)
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

CP::TTrackState& CP::TTrackState::operator=(const CP::TTrackState& rhs) {
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

CP::TCorrValues CP::TTrackState::ProjectState(
    const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TTrackState::GetSize());
    values.SetType("EDeposit X Y Z T DX DY DZ Mass W1 W2 ");
    const TMEDepositState* eDepositState 
        = dynamic_cast<const TMEDepositState*>(GetPointer(proj));
    int base = 0;
    if (eDepositState) {
        const int offset = eDepositState->GetEDepositIndex();
        for (int i = 0; i < TMEDepositState::GetSize(); ++i) {
            values.SetValue(i+base,
                            eDepositState->
                            GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMEDepositState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    eDepositState->
                    GetThis().fValues.GetCovarianceValue(i+offset,
                                                         j+offset));
            }
        }
    }
    const TMPositionState* posState 
        = dynamic_cast<const TMPositionState*>(GetPointer(proj));
    base += TMEDepositState::GetSize();
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
    const TMDirectionState* dirState 
        = dynamic_cast<const TMDirectionState*>(GetPointer(proj));
    base += TMPositionState::GetSize();
    if (dirState) {
        const int offset = dirState->GetDirectionIndex();
        for (int i = 0; i < TMDirectionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            dirState->GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMDirectionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    dirState->GetThis().fValues.GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    const TMMassState* curvState 
        = dynamic_cast<const TMMassState*>(GetPointer(proj));
    base += TMDirectionState::GetSize();
    if (curvState) {
        const int offset = curvState->GetMassIndex();
        for (int i = 0; i < TMMassState::GetSize(); ++i) {
            values.SetValue(i+base,
                            curvState->GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMMassState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    curvState->GetThis().fValues.GetCovarianceValue(i+offset,
                                                                    j+offset));
            }
        }
    }
    const TMWidthState* widthState 
        = dynamic_cast<const TMWidthState*>(GetPointer(proj));
    base += TMMassState::GetSize();
    if (widthState) {
        const int offset = widthState->GetWidthIndex();
        for (int i = 0; i < TMWidthState::GetSize(); ++i) {
            values.SetValue(i+base,
                            widthState->GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMWidthState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    widthState->GetThis().fValues.GetCovarianceValue(i+offset,
                                                                    j+offset));
            }
        }
    }
    return values;
}
