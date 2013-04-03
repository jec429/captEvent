#include "TClusterState.hxx"
#include "TCaptLog.hxx"
///////////////////////////////////////////////////////
ClassImp(CP::TClusterState);

CP::TClusterState::TClusterState(): TMReconState(this) {
    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));
    Init();
}

CP::TClusterState::~TClusterState() {}

CP::TClusterState::TClusterState(const CP::TClusterState& init)
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

CP::TClusterState& CP::TClusterState::operator=(const CP::TClusterState& rhs) {
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

CP::TCorrValues CP::TClusterState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TClusterState::GetSize());
    values.SetType("EDeposit X Y Z T ");
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
    return values;
}
