#include <cmath>

#include "TPIDState.hxx"
#include "HEPUnits.hxx"

///////////////////////////////////////////////////////
ClassImp(ND::TPIDState);

ND::TPIDState::TPIDState(): TMReconState(this) {
    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));
    Init();
}

ND::TPIDState::TPIDState(const ND::TTrackState& tstate): TMReconState(this) {

    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));


    Init();

    // retrieve the position and it's covariance. 
    for(int i = 0;i < TMPositionState::GetSize(); ++i) {
        SetValue(i+GetPositionIndex(),
                 tstate.GetValue(i+tstate.GetPositionIndex()));
        for(int j = 0;j < TMPositionState::GetSize(); ++j) {
            SetCovarianceValue(i+GetPositionIndex(),
                               j+GetPositionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetPositionIndex(), 
                                   j+tstate.GetPositionIndex()));
        }
    }  
    
    // retrieve the direction and it's covariance.
    for(int i = 0;i < TMDirectionState::GetSize();++i){
        SetValue(i+GetDirectionIndex(),
                 tstate.GetValue(i+tstate.GetDirectionIndex()));
        for(int j = 0;j < TMDirectionState::GetSize();++j){
            SetCovarianceValue(i+GetDirectionIndex(),
                               j+GetDirectionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetDirectionIndex(),
                                   j+tstate.GetDirectionIndex()));
            SetCovarianceValue(i+GetPositionIndex(),
                               j+GetDirectionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetPositionIndex(),
                                   j+tstate.GetDirectionIndex()));
            SetCovarianceValue(i+GetDirectionIndex(),
                               j+GetPositionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetDirectionIndex(),
                                   j+tstate.GetPositionIndex()));
        }
    }  
    
    // If curvature is available, then convert to momentum.  This is bogus
    // since it depends on a magnetic field, so the momentum is initialized
    // assuming a constant 0.2T field, but is set as a free parameter.  be
    // free (by default).
    double B=0.2;
    double factor = -0.3*B;
    double p = 0;
    if (!tstate.IsFree(tstate.GetCurvatureIndex())) {
        p=std::fabs(factor*1/tstate.GetValue(tstate.GetCurvatureIndex()));
    }
    
    SetValue(GetMomentumIndex(),p);
    SetFree(GetMomentumIndex());

    // If the curvature is available, then convert to charge.
    double q = 0;
    if (!tstate.IsFree(tstate.GetCurvatureIndex())
        && tstate.GetValue(tstate.GetCurvatureIndex())>1E-15) {
        q=-tstate.GetValue(tstate.GetCurvatureIndex())
            /std::fabs(tstate.GetValue(tstate.GetCurvatureIndex()));
    }
    SetValue(GetChargeIndex(),q);
    if (tstate.IsFree(tstate.GetCurvatureIndex()) || std::fabs(q) < 0.1) {
        SetFree(GetChargeIndex());
    }
    else if (tstate.IsFixed(tstate.GetCurvatureIndex())) {
        SetFixed(GetChargeIndex());
    }
    else {
        double cv = tstate.GetCovarianceValue(tstate.GetCurvatureIndex(),
                                              tstate.GetCurvatureIndex());
        SetCovarianceValue(
            GetChargeIndex(),GetChargeIndex(),
            std::fabs(cv/tstate.GetValue(tstate.GetCurvatureIndex())));
    }

}


ND::TPIDState::TPIDState(const ND::TShowerState& tstate): TMReconState(this) {

    std::copy(fLocalNames.begin(), fLocalNames.end(), 
              std::back_inserter(fFieldNames));

    Init();

    // retrieve the position and it's covariance. 
    for(int i = 0;i < TMPositionState::GetSize(); ++i) {
        SetValue(i+GetPositionIndex(),
                 tstate.GetValue(i+tstate.GetPositionIndex()));
        for(int j = 0;j < TMPositionState::GetSize(); ++j) {
            SetCovarianceValue(i+GetPositionIndex(),
                               j+GetPositionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetPositionIndex(), 
                                   j+tstate.GetPositionIndex()));
        }
    }  
    
    // retrieve the direction and it's covariance.
    for(int i = 0;i < TMDirectionState::GetSize();++i){
        SetValue(i+GetDirectionIndex(),
                 tstate.GetValue(i+tstate.GetDirectionIndex()));
        for(int j = 0;j < TMDirectionState::GetSize();++j){
            SetCovarianceValue(i+GetDirectionIndex(),
                               j+GetDirectionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetDirectionIndex(),
                                   j+tstate.GetDirectionIndex()));
            SetCovarianceValue(i+GetPositionIndex(),
                               j+GetDirectionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetPositionIndex(),
                                   j+tstate.GetDirectionIndex()));
            SetCovarianceValue(i+GetDirectionIndex(),
                               j+GetPositionIndex(),
                               tstate.GetCovarianceValue(
                                   i+tstate.GetDirectionIndex(),
                                   j+tstate.GetPositionIndex()));
        }
    }  
    
    // Set momentum and charge
    double p = tstate.GetEDeposit(); // Use the deposited energy (bogus)
    double q = 0;                    // Don't have any curvature.
    
    SetValue(GetMomentumIndex(),p);
    SetFree(GetMomentumIndex());

    SetValue(GetChargeIndex(),q);
    SetFree(GetChargeIndex());
}

ND::TPIDState::TPIDState(const ND::TPIDState& init)
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

ND::TPIDState& ND::TPIDState::operator=(const ND::TPIDState& rhs) {
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

ND::TPIDState::~TPIDState() {}

ND::TCorrValues ND::TPIDState::ProjectState(
    const ND::THandle<ND::TReconState>& proj) {
    TCorrValues values(TPIDState::GetSize());
    values.SetType("X Y Z T DX DY DZ Momentum Charge");
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
    const TMMomentumState* momState 
        = dynamic_cast<const TMMomentumState*>(GetPointer(proj));
    base += TMDirectionState::GetSize();
    if (momState) {
        const int offset = momState->GetMomentumIndex();
        for (int i = 0; i < TMMomentumState::GetSize(); ++i) {
            values.SetValue(i+base,
                            momState->GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMMomentumState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    momState->GetThis().fValues.GetCovarianceValue(i+offset,
                                                                    j+offset));
            }
        }
    }
    const TMChargeState* chgState 
        = dynamic_cast<const TMChargeState*>(GetPointer(proj));
    base += TMMomentumState::GetSize();
    if (chgState) {
        const int offset = chgState->GetChargeIndex();
        for (int i = 0; i < TMChargeState::GetSize(); ++i) {
            values.SetValue(i+base,
                            chgState->GetThis().fValues.GetValue(i+offset));
            for (int j = 0; j < TMChargeState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    chgState->GetThis().fValues.GetCovarianceValue(i+offset,
                                                                    j+offset));
            }
        }
    }
    return values;
}
