#include <iostream>
#include <iomanip>
#include <cmath>

#include <TROOT.h>

#include "TCaptLog.hxx"
#include "TReconState.hxx"

ClassImp(CP::TReconState);

CP::TReconState::TReconState() { }

CP::TReconState::TReconState(const TReconState& state) 
    : TObject(state), fValues(state.fValues), fFieldNames(state.fFieldNames) { }

CP::TReconState::~TReconState() { }

std::string CP::TReconState::GetStateFields(void) const {
    // Construct a type name out of the field names.  This in turn is used by
    // the TCorrValues class to construct a type hash which is used to make
    // sure that operations are done on compatible TCorrValues objects.
    std::string typeName;
    for (std::vector<std::string>::const_iterator n = fFieldNames.begin();
         n != fFieldNames.end();
         ++n) {
        typeName += *n;
        typeName += " ";
    };
    return typeName;
}

// Build the internal state vector.
void CP::TReconState::Init() {
    fValues.ResizeTo(fFieldNames.size());
    fValues.SetType(GetStateFields().c_str());
}

int CP::TReconState::GetDimensions() const {
    return fValues.GetDimensions();
}

double CP::TReconState::GetValue(int i) const {
    return fValues.GetValue(i);
}

void CP::TReconState::SetValue(int i, double val) {
    return fValues.SetValue(i, val);
}

double CP::TReconState::GetCovarianceValue(int i, int j) const {
    return fValues.GetCovarianceValue(i,j);
}

void CP::TReconState::SetCovarianceValue(int i, int j, double val) {
    fValues.SetCovarianceValue(i,j,val);
}

void CP::TReconState::SetFree(int i) {
    fValues.SetFree(i);
}

bool CP::TReconState::IsFree(int i) const {
    return fValues.IsFree(i);
}

bool CP::TReconState::IsFree(double v) const {
    return fValues.IsFree(v);
}

void CP::TReconState::SetFixed(int i) {
    fValues.SetFixed(i);
}

bool CP::TReconState::IsFixed(int i) const {
    return fValues.IsFixed(i);
}

bool CP::TReconState::IsFixed(double v) const {
    return fValues.IsFixed(v);
}

void CP::TReconState::Validate() {
    fValues.Validate(true);
}

CP::TCorrValues CP::TReconState::ProjectState(const CP::THandle<CP::TReconState>& state) {
    return state->fValues;
}

/// Print the object information.
void CP::TReconState::ls(Option_t*) const {
    TROOT::IndentLevel();
    std::cout << ClassName() << "(" << this << ")::" << std::endl;
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << GetStateFields() << std::endl;
    TROOT::IncreaseDirLevel();
    std::ios::fmtflags save = std::cout.flags();
    for (int i = 0; i<GetDimensions(); ++i) {
        if (IsFree(i)) continue;
        TROOT::IndentLevel();
        std::cout << "  " << std::setw(6) << fFieldNames[i];
        std::cout << ":: " 
                  << std::setw(9) << std::setprecision(3) 
                  << GetValue(i);
        if (IsFixed(i)) std::cout << "    fixed";
        else {
            std::cout << " +- " 
                      << std::setw(5) << std::setprecision(3) 
                      << std::sqrt(GetCovarianceValue(i,i));
        }
        std::cout << std::endl;
    }
    std::cout.flags(save);
    TROOT::DecreaseDirLevel();
    TROOT::DecreaseDirLevel();
}

////////////////////////////////////////////////////////
CP::TMReconState::~TMReconState() {}

CP::TMReconState::TMReconState(): fThis(NULL) {}

CP::TMReconState::TMReconState(TReconState* state): fThis(state) {}

CP::TReconState& CP::TMReconState::GetThis() const {
    if (!fThis) {
        CaptError("Base pointer (fThis) is NULL");
    }
    return *fThis;
}

///////////////////////////////////////////////////////
CP::TMEDepositState::~TMEDepositState() {}

CP::TMEDepositState::TMEDepositState() {
    fEDepositIndex = fLocalNames.size();
    fLocalNames.push_back("EDeposit");
}

double CP::TMEDepositState::GetEDeposit() const {
    return GetThis().GetValue(fEDepositIndex);
}

void CP::TMEDepositState::SetEDeposit(double enr) {
    GetThis().SetValue(fEDepositIndex,enr);
}

double CP::TMEDepositState::GetEDepositVariance() const {
    return GetThis().GetCovarianceValue(fEDepositIndex,fEDepositIndex);
}

void CP::TMEDepositState::SetEDepositVariance(double var) {
    GetThis().SetCovarianceValue(fEDepositIndex,fEDepositIndex,var);
}

CP::TCorrValues CP::TMEDepositState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMEDepositState::GetSize());
    values.SetType("EDeposit ");
    const TMEDepositState* eDepositState 
        = dynamic_cast<const TMEDepositState*>(GetPointer(proj));
    int base = 0;
    if (eDepositState) {
        const int offset = eDepositState->GetEDepositIndex();
        for (int i = 0; i < TMEDepositState::GetSize(); ++i) {
            values.SetValue(i+base,
                            eDepositState->
                            GetThis().GetValue(i+offset));
            for (int j = 0; j < TMEDepositState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    eDepositState->
                    GetThis().GetCovarianceValue(i+offset,
                                                         j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMPositionState::~TMPositionState() {}

CP::TMPositionState::TMPositionState() {
    fPositionIndex = fLocalNames.size();
    fLocalNames.push_back("X");
    fLocalNames.push_back("Y");
    fLocalNames.push_back("Z");
    fLocalNames.push_back("T");
}

TLorentzVector CP::TMPositionState::GetPosition() const {
    return TLorentzVector(GetThis().GetValue(fPositionIndex),
                          GetThis().GetValue(fPositionIndex+1),
                          GetThis().GetValue(fPositionIndex+2),
                          GetThis().GetValue(fPositionIndex+3));
}

void CP::TMPositionState::SetPosition(double x, double y,
                                      double z, double t) {
    GetThis().SetValue(fPositionIndex,x);
    GetThis().SetValue(fPositionIndex+1,y);
    GetThis().SetValue(fPositionIndex+2,z);
    GetThis().SetValue(fPositionIndex+3,t);
}

void CP::TMPositionState::SetPosition(const TLorentzVector& pos) {
    SetPosition(pos.X(), pos.Y(), pos.Z(), pos.T());
}

TLorentzVector CP::TMPositionState::GetPositionVariance() const {
    return TLorentzVector(
        GetThis().GetCovarianceValue(fPositionIndex,
                                             fPositionIndex),
        GetThis().GetCovarianceValue(fPositionIndex+1,
                                             fPositionIndex+1),
        GetThis().GetCovarianceValue(fPositionIndex+2,
                                             fPositionIndex+2),
        GetThis().GetCovarianceValue(fPositionIndex+3,
                                             fPositionIndex+3));
}

void CP::TMPositionState::SetPositionVariance(double x, double y, 
                                              double z, double t) {
    GetThis().SetCovarianceValue(fPositionIndex,
                               fPositionIndex,x);
    GetThis().SetCovarianceValue(fPositionIndex+1,
                               fPositionIndex+1,y);
    GetThis().SetCovarianceValue(fPositionIndex+2,
                               fPositionIndex+2,z);
    GetThis().SetCovarianceValue(fPositionIndex+3,
                               fPositionIndex+3,t);
}

void CP::TMPositionState::SetPositionCovariance(int i, int j, double v) {
    GetThis().SetCovarianceValue(fPositionIndex+i,fPositionIndex+j,v);
}

double CP::TMPositionState::GetPositionCovariance(int i, int j) {
    return GetThis().GetCovarianceValue(fPositionIndex+i,fPositionIndex+j);
}

CP::TCorrValues
CP::TMPositionState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMPositionState::GetSize());
    values.SetType("X Y Z T ");
    const TMPositionState* posState 
        = dynamic_cast<const TMPositionState*>(GetPointer(proj));
    int base = 0;
    if (posState) {
        const int offset = posState->GetPositionIndex();
        for (int i = 0; i < TMPositionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            posState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMPositionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    posState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMDirectionState::~TMDirectionState() {}

CP::TMDirectionState::TMDirectionState() {
    fDirectionIndex = fLocalNames.size();
    fLocalNames.push_back("DX");
    fLocalNames.push_back("DY");
    fLocalNames.push_back("DZ");
}

TVector3 CP::TMDirectionState::GetDirection() const {
    return TVector3(GetThis().GetValue(fDirectionIndex),
                    GetThis().GetValue(fDirectionIndex+1),
                    GetThis().GetValue(fDirectionIndex+2));
}

void CP::TMDirectionState::SetDirection(double x, double y,
                                      double z) {
    GetThis().SetValue(fDirectionIndex,x);
    GetThis().SetValue(fDirectionIndex+1,y);
    GetThis().SetValue(fDirectionIndex+2,z);
}

void CP::TMDirectionState::SetDirection(const TVector3& dir) {
    SetDirection(dir.X(), dir.Y(), dir.Z());
}

TVector3 CP::TMDirectionState::GetDirectionVariance() const {
    return TVector3(GetThis().GetCovarianceValue(fDirectionIndex,
                                                    fDirectionIndex),
                    GetThis().GetCovarianceValue(fDirectionIndex+1,
                                                    fDirectionIndex+1),
                    GetThis().GetCovarianceValue(fDirectionIndex+2,
                                                    fDirectionIndex+2));
}

void CP::TMDirectionState::SetDirectionVariance(double x, double y, 
                                                double z) {
    GetThis().SetCovarianceValue(fDirectionIndex,
                                    fDirectionIndex,x);
    GetThis().SetCovarianceValue(fDirectionIndex+1,
                                    fDirectionIndex+1,y);
    GetThis().SetCovarianceValue(fDirectionIndex+2,
                                    fDirectionIndex+2,z);
}

void CP::TMDirectionState::SetDirectionCovariance(int i, int j, double v) {
    GetThis().SetCovarianceValue(fDirectionIndex+i,fDirectionIndex+j,v);
}

double CP::TMDirectionState::GetDirectionCovariance(int i, int j) {
    return GetThis().GetCovarianceValue(fDirectionIndex+i,fDirectionIndex+j);
}

CP::TCorrValues 
CP::TMDirectionState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMDirectionState::GetSize());
    values.SetType("DX DY DZ ");
    const TMDirectionState* dirState 
        = dynamic_cast<const TMDirectionState*>(GetPointer(proj));
    int base = 0;
    if (dirState) {
        const int offset = dirState->GetDirectionIndex();
        for (int i = 0; i < TMDirectionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            dirState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMDirectionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    dirState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMMomentumState::~TMMomentumState() {}

CP::TMMomentumState::TMMomentumState() {
    fMomentumIndex = fLocalNames.size();
    fLocalNames.push_back("Momentum");
}

double CP::TMMomentumState::GetMomentum() const {
    return GetThis().GetValue(fMomentumIndex);
}

void CP::TMMomentumState::SetMomentum(double enr) {
    GetThis().SetValue(fMomentumIndex,enr);
}

double CP::TMMomentumState::GetMomentumVariance() const {
    return GetThis().GetCovarianceValue(fMomentumIndex,fMomentumIndex);
}

void CP::TMMomentumState::SetMomentumVariance(double var) {
    GetThis().SetCovarianceValue(fMomentumIndex,fMomentumIndex,var);
}

CP::TCorrValues CP::TMMomentumState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMMomentumState::GetSize());
    values.SetType("Momentum ");
    const TMMomentumState* momState 
        = dynamic_cast<const TMMomentumState*>(GetPointer(proj));
    int base = 0;
    if (momState) {
        const int offset = momState->GetMomentumIndex();
        for (int i = 0; i < TMMomentumState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            momState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMMomentumState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    momState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMChargeState::~TMChargeState() {}

CP::TMChargeState::TMChargeState() {
    fChargeIndex = fLocalNames.size();
    fLocalNames.push_back("Charge");
}

double CP::TMChargeState::GetCharge() const {
    return GetThis().GetValue(fChargeIndex);
}

void CP::TMChargeState::SetCharge(double enr) {
    GetThis().SetValue(fChargeIndex,enr);
}

double CP::TMChargeState::GetChargeVariance() const {
    return GetThis().GetCovarianceValue(fChargeIndex,fChargeIndex);
}

void CP::TMChargeState::SetChargeVariance(double var) {
    GetThis().SetCovarianceValue(fChargeIndex,fChargeIndex,var);
}

CP::TCorrValues CP::TMChargeState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMChargeState::GetSize());
    values.SetType("Charge ");
    const TMChargeState* chgState 
        = dynamic_cast<const TMChargeState*>(GetPointer(proj));
    int base = 0;
    if (chgState) {
        const int offset = chgState->GetChargeIndex();
        for (int i = 0; i < TMChargeState::GetSize(); ++i) {
            values.SetValue(i+base,
                            chgState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMChargeState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    chgState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMCurvatureState::~TMCurvatureState() {}

CP::TMCurvatureState::TMCurvatureState() {
    fCurvatureIndex = fLocalNames.size();
    fLocalNames.push_back("Curvature");
}

double CP::TMCurvatureState::GetCurvature() const {
    return GetThis().GetValue(fCurvatureIndex);
}

void CP::TMCurvatureState::SetCurvature(double enr) {
    GetThis().SetValue(fCurvatureIndex,enr);
}

double CP::TMCurvatureState::GetCurvatureVariance() const {
    return GetThis().GetCovarianceValue(fCurvatureIndex,
                                                fCurvatureIndex);
}

void CP::TMCurvatureState::SetCurvatureVariance(double var) {
    GetThis().SetCovarianceValue(fCurvatureIndex,fCurvatureIndex,var);
}

CP::TCorrValues CP::TMCurvatureState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMCurvatureState::GetSize());
    values.SetType("Curvature ");
    const TMCurvatureState* curvState 
        = dynamic_cast<const TMCurvatureState*>(GetPointer(proj));
    int base = 0;
    if (curvState) {
        const int offset = curvState->GetCurvatureIndex();
        for (int i = 0; i < TMCurvatureState::GetSize(); ++i) {
            values.SetValue(i+base,
                            curvState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMCurvatureState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    curvState->GetThis().GetCovarianceValue(i+offset,
                                                                    j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMConeState::~TMConeState() {}

CP::TMConeState::TMConeState() {
    fConeIndex = fLocalNames.size();
    fLocalNames.push_back("Cone");
}

double CP::TMConeState::GetCone() const {
    return GetThis().GetValue(fConeIndex);
}

void CP::TMConeState::SetCone(double x) {
    GetThis().SetValue(fConeIndex,x);
}

double CP::TMConeState::GetConeVariance() const {
    return GetThis().GetCovarianceValue(fConeIndex,fConeIndex);
}

void CP::TMConeState::SetConeVariance(double x) {
    GetThis().SetCovarianceValue(fConeIndex,fConeIndex,x);
}

CP::TCorrValues 
CP::TMConeState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMConeState::GetSize());
    values.SetType("Cone ");
    const TMConeState* coneState 
        = dynamic_cast<const TMConeState*>(GetPointer(proj));
    int base = 0;
    if (coneState) {
        const int offset = coneState->GetConeIndex();
        for (int i = 0; i < TMConeState::GetSize(); ++i) {
            values.SetValue(i+base,coneState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMConeState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    coneState->GetThis().GetCovarianceValue(i+offset,
                                                            j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMWidthState::~TMWidthState() {}

CP::TMWidthState::TMWidthState() {
    fWidthIndex = fLocalNames.size();
    fLocalNames.push_back("width");
}


double CP::TMWidthState::GetWidth() const {
    return GetThis().GetValue(fWidthIndex);
}

void CP::TMWidthState::SetWidth(double x) {
    GetThis().SetValue(fWidthIndex,x);
}

double CP::TMWidthState::GetWidthVariance() const {
    return GetThis().GetCovarianceValue(fWidthIndex,fWidthIndex);
}

void CP::TMWidthState::SetWidthVariance(double x) {
    GetThis().SetCovarianceValue(fWidthIndex,fWidthIndex,x);
}

CP::TCorrValues CP::TMWidthState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMWidthState::GetSize());
    values.SetType("W1 W2 ");
    const TMWidthState* wdthState 
        = dynamic_cast<const TMWidthState*>(GetPointer(proj));
    int base = 0;
    if (wdthState) {
        const int offset = wdthState->GetWidthIndex();
        for (int i = 0; i < TMWidthState::GetSize(); ++i) {
            values.SetValue(i+base,
                            wdthState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMWidthState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    wdthState->GetThis().GetCovarianceValue(i+offset,
                                                            j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMPositionDirectionState::~TMPositionDirectionState() {}

CP::TCorrValues CP::TMPositionDirectionState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMPositionDirectionState::GetSize());
    values.SetType("X Y Z T DX DY DZ ");
    const TMPositionState* posState 
        = dynamic_cast<const TMPositionState*>(GetPointer(proj));
    int base = 0;
    if (posState) {
        int offset = posState->GetPositionIndex();
        for (int i = 0; i < TMPositionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            posState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMPositionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    posState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    base += TMDirectionState::GetSize();
    const TMDirectionState* dirState 
        = dynamic_cast<const TMDirectionState*>(GetPointer(proj));
    if (dirState) {
        int offset = dirState->GetDirectionIndex();
        for (int i = 0; i < TMDirectionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            dirState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMDirectionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    dirState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    return values;
}

///////////////////////////////////////////////////////
CP::TMPosDirCurvState::~TMPosDirCurvState() {}

CP::TCorrValues CP::TMPosDirCurvState::ProjectState(const CP::THandle<CP::TReconState>& proj) {
    TCorrValues values(TMPosDirCurvState::GetSize());
    values.SetType("X Y Z T DX DY DZ Curvature ");
    const TMPositionState* posState 
        = dynamic_cast<const TMPositionState*>(GetPointer(proj));
    int base = 0;
    if (posState) {
        int offset = posState->GetPositionIndex();
        for (int i = 0; i < TMPositionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            posState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMPositionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    posState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    base += TMDirectionState::GetSize();
    const TMDirectionState* dirState 
        = dynamic_cast<const TMDirectionState*>(GetPointer(proj));
    if (dirState) {
        int offset = dirState->GetDirectionIndex();
        for (int i = 0; i < TMDirectionState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            dirState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMDirectionState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    dirState->GetThis().GetCovarianceValue(i+offset,
                                                                   j+offset));
            }
        }
    }
    base += TMDirectionState::GetSize();
    const TMCurvatureState* curvState 
        = dynamic_cast<const TMCurvatureState*>(GetPointer(proj));
    if (curvState) {
        int offset = curvState->GetCurvatureIndex();
        for (int i = 0; i < TMCurvatureState::GetSize(); ++i) {
            values.SetValue(i+base, 
                            curvState->GetThis().GetValue(i+offset));
            for (int j = 0; j < TMCurvatureState::GetSize(); ++j) {
                values.SetCovarianceValue(
                    i+base, j+base,
                    curvState->GetThis().GetCovarianceValue(i+offset,
                                                                    j+offset));
            }
        }
    }
    return values;
}
