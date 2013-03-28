#include "TClusterState.hxx"
#include "TND280Log.hxx"
///////////////////////////////////////////////////////
ClassImp(ND::TClusterState);

ND::TClusterState::TClusterState(): TMReconState(this) {
    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));
    Init();
}

ND::TClusterState::~TClusterState() {}

ND::TClusterState::TClusterState(const ND::TClusterState& init)
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

ND::TClusterState& ND::TClusterState::operator=(const ND::TClusterState& rhs) {
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

ND::TCorrValues ND::TClusterState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
