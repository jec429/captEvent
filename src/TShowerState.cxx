#include "TShowerState.hxx"

///////////////////////////////////////////////////////
ClassImp(CP::TShowerState);

CP::TShowerState::TShowerState(): TMReconState(this) {
    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));
    Init();
}

CP::TShowerState::~TShowerState() {}

CP::TShowerState::TShowerState(const CP::TShowerState& init)
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

CP::TShowerState& CP::TShowerState::operator=(const CP::TShowerState& rhs) {
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

CP::TCorrValues CP::TShowerState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TShowerState::GetSize());
    values.SetType("EDeposit X Y Z T DX DY DZ C1 C2 ");
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
    const TMConeState* coneState 
        = dynamic_cast<const TMConeState*>(GetPointer(proj));
    base += TMDirectionState::GetSize();
    if (coneState) {
        const int offset = coneState->GetConeIndex();
        for (int i = 0; i < TMConeState::GetSize(); ++i) {
            values.SetValue(i+base,
                            coneState->GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMConeState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    coneState->GetThis().fValues.GetCovarianceValue(i+offset,
                                                                    j+offset));
            }
        }
    }
    return values;
}
