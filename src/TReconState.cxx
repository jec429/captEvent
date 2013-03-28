#include <iostream>
#include <iomanip>
#include <cmath>

#include <TROOT.h>

#include "TND280Log.hxx"
#include "TReconState.hxx"

ClassImp(ND::TReconState);

ND::TReconState::TReconState() { }

ND::TReconState::TReconState(const TReconState& state) 
    : TObject(state), fValues(state.fValues), fFieldNames(state.fFieldNames) { }

ND::TReconState::~TReconState() { }

std::string ND::TReconState::GetStateFields(void) const {
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
void ND::TReconState::Init() {
    fValues.ResizeTo(fFieldNames.size());
    fValues.SetType(GetStateFields().c_str());
}

int ND::TReconState::GetDimensions() const {
    return fValues.GetDimensions();
}

double ND::TReconState::GetValue(int i) const {
    return fValues.GetValue(i);
}

void ND::TReconState::SetValue(int i, double val) {
    return fValues.SetValue(i, val);
}

double ND::TReconState::GetCovarianceValue(int i, int j) const {
    return fValues.GetCovarianceValue(i,j);
}

void ND::TReconState::SetCovarianceValue(int i, int j, double val) {
    fValues.SetCovarianceValue(i,j,val);
}

void ND::TReconState::SetFree(int i) {
    fValues.SetFree(i);
}

bool ND::TReconState::IsFree(int i) const {
    return fValues.IsFree(i);
}

bool ND::TReconState::IsFree(double v) const {
    return fValues.IsFree(v);
}

void ND::TReconState::SetFixed(int i) {
    fValues.SetFixed(i);
}

bool ND::TReconState::IsFixed(int i) const {
    return fValues.IsFixed(i);
}

bool ND::TReconState::IsFixed(double v) const {
    return fValues.IsFixed(v);
}

void ND::TReconState::Validate() {
    fValues.Validate(true);
}

ND::TCorrValues ND::TReconState::ProjectState(const ND::THandle<ND::TReconState>& state) {
    return state->fValues;
}

/// Print the object information.
void ND::TReconState::ls(Option_t*) const {
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
ND::TMReconState::~TMReconState() {}

ND::TMReconState::TMReconState(): fThis(NULL) {}

ND::TMReconState::TMReconState(TReconState* state): fThis(state) {}

ND::TReconState& ND::TMReconState::GetThis() const {
    if (!fThis) {
        ND280Error("Base pointer (fThis) is NULL");
    }
    return *fThis;
}

///////////////////////////////////////////////////////
ND::TMEDepositState::~TMEDepositState() {}

ND::TMEDepositState::TMEDepositState() {
    fEDepositIndex = fLocalNames.size();
    fLocalNames.push_back("EDeposit");
}

double ND::TMEDepositState::GetEDeposit() const {
    return GetThis().GetValue(fEDepositIndex);
}

void ND::TMEDepositState::SetEDeposit(double enr) {
    GetThis().SetValue(fEDepositIndex,enr);
}

double ND::TMEDepositState::GetEDepositVariance() const {
    return GetThis().GetCovarianceValue(fEDepositIndex,fEDepositIndex);
}

void ND::TMEDepositState::SetEDepositVariance(double var) {
    GetThis().SetCovarianceValue(fEDepositIndex,fEDepositIndex,var);
}

ND::TCorrValues ND::TMEDepositState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMPositionState::~TMPositionState() {}

ND::TMPositionState::TMPositionState() {
    fPositionIndex = fLocalNames.size();
    fLocalNames.push_back("X");
    fLocalNames.push_back("Y");
    fLocalNames.push_back("Z");
    fLocalNames.push_back("T");
}

TLorentzVector ND::TMPositionState::GetPosition() const {
    return TLorentzVector(GetThis().GetValue(fPositionIndex),
                          GetThis().GetValue(fPositionIndex+1),
                          GetThis().GetValue(fPositionIndex+2),
                          GetThis().GetValue(fPositionIndex+3));
}

void ND::TMPositionState::SetPosition(double x, double y,
                                      double z, double t) {
    GetThis().SetValue(fPositionIndex,x);
    GetThis().SetValue(fPositionIndex+1,y);
    GetThis().SetValue(fPositionIndex+2,z);
    GetThis().SetValue(fPositionIndex+3,t);
}

void ND::TMPositionState::SetPosition(const TLorentzVector& pos) {
    SetPosition(pos.X(), pos.Y(), pos.Z(), pos.T());
}

TLorentzVector ND::TMPositionState::GetPositionVariance() const {
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

void ND::TMPositionState::SetPositionVariance(double x, double y, 
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

void ND::TMPositionState::SetPositionCovariance(int i, int j, double v) {
    GetThis().SetCovarianceValue(fPositionIndex+i,fPositionIndex+j,v);
}

double ND::TMPositionState::GetPositionCovariance(int i, int j) {
    return GetThis().GetCovarianceValue(fPositionIndex+i,fPositionIndex+j);
}

ND::TCorrValues
ND::TMPositionState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMDirectionState::~TMDirectionState() {}

ND::TMDirectionState::TMDirectionState() {
    fDirectionIndex = fLocalNames.size();
    fLocalNames.push_back("DX");
    fLocalNames.push_back("DY");
    fLocalNames.push_back("DZ");
}

TVector3 ND::TMDirectionState::GetDirection() const {
    return TVector3(GetThis().GetValue(fDirectionIndex),
                    GetThis().GetValue(fDirectionIndex+1),
                    GetThis().GetValue(fDirectionIndex+2));
}

void ND::TMDirectionState::SetDirection(double x, double y,
                                      double z) {
    GetThis().SetValue(fDirectionIndex,x);
    GetThis().SetValue(fDirectionIndex+1,y);
    GetThis().SetValue(fDirectionIndex+2,z);
}

void ND::TMDirectionState::SetDirection(const TVector3& dir) {
    SetDirection(dir.X(), dir.Y(), dir.Z());
}

TVector3 ND::TMDirectionState::GetDirectionVariance() const {
    return TVector3(GetThis().GetCovarianceValue(fDirectionIndex,
                                                    fDirectionIndex),
                    GetThis().GetCovarianceValue(fDirectionIndex+1,
                                                    fDirectionIndex+1),
                    GetThis().GetCovarianceValue(fDirectionIndex+2,
                                                    fDirectionIndex+2));
}

void ND::TMDirectionState::SetDirectionVariance(double x, double y, 
                                                double z) {
    GetThis().SetCovarianceValue(fDirectionIndex,
                                    fDirectionIndex,x);
    GetThis().SetCovarianceValue(fDirectionIndex+1,
                                    fDirectionIndex+1,y);
    GetThis().SetCovarianceValue(fDirectionIndex+2,
                                    fDirectionIndex+2,z);
}

void ND::TMDirectionState::SetDirectionCovariance(int i, int j, double v) {
    GetThis().SetCovarianceValue(fDirectionIndex+i,fDirectionIndex+j,v);
}

double ND::TMDirectionState::GetDirectionCovariance(int i, int j) {
    return GetThis().GetCovarianceValue(fDirectionIndex+i,fDirectionIndex+j);
}

ND::TCorrValues 
ND::TMDirectionState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMMomentumState::~TMMomentumState() {}

ND::TMMomentumState::TMMomentumState() {
    fMomentumIndex = fLocalNames.size();
    fLocalNames.push_back("Momentum");
}

double ND::TMMomentumState::GetMomentum() const {
    return GetThis().GetValue(fMomentumIndex);
}

void ND::TMMomentumState::SetMomentum(double enr) {
    GetThis().SetValue(fMomentumIndex,enr);
}

double ND::TMMomentumState::GetMomentumVariance() const {
    return GetThis().GetCovarianceValue(fMomentumIndex,fMomentumIndex);
}

void ND::TMMomentumState::SetMomentumVariance(double var) {
    GetThis().SetCovarianceValue(fMomentumIndex,fMomentumIndex,var);
}

ND::TCorrValues ND::TMMomentumState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMChargeState::~TMChargeState() {}

ND::TMChargeState::TMChargeState() {
    fChargeIndex = fLocalNames.size();
    fLocalNames.push_back("Charge");
}

double ND::TMChargeState::GetCharge() const {
    return GetThis().GetValue(fChargeIndex);
}

void ND::TMChargeState::SetCharge(double enr) {
    GetThis().SetValue(fChargeIndex,enr);
}

double ND::TMChargeState::GetChargeVariance() const {
    return GetThis().GetCovarianceValue(fChargeIndex,fChargeIndex);
}

void ND::TMChargeState::SetChargeVariance(double var) {
    GetThis().SetCovarianceValue(fChargeIndex,fChargeIndex,var);
}

ND::TCorrValues ND::TMChargeState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMCurvatureState::~TMCurvatureState() {}

ND::TMCurvatureState::TMCurvatureState() {
    fCurvatureIndex = fLocalNames.size();
    fLocalNames.push_back("Curvature");
}

double ND::TMCurvatureState::GetCurvature() const {
    return GetThis().GetValue(fCurvatureIndex);
}

void ND::TMCurvatureState::SetCurvature(double enr) {
    GetThis().SetValue(fCurvatureIndex,enr);
}

double ND::TMCurvatureState::GetCurvatureVariance() const {
    return GetThis().GetCovarianceValue(fCurvatureIndex,
                                                fCurvatureIndex);
}

void ND::TMCurvatureState::SetCurvatureVariance(double var) {
    GetThis().SetCovarianceValue(fCurvatureIndex,fCurvatureIndex,var);
}

ND::TCorrValues ND::TMCurvatureState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMConeState::~TMConeState() {}

ND::TMConeState::TMConeState() {
    fConeIndex = fLocalNames.size();
    fLocalNames.push_back("C1");
    fLocalNames.push_back("C2");
}

TVector3 ND::TMConeState::GetCone() const {
    return TVector3(GetThis().GetValue(fConeIndex),
                    GetThis().GetValue(fConeIndex+1),
                    0);
}

void ND::TMConeState::SetCone(double x, double y) {
    GetThis().SetValue(fConeIndex,x);
    GetThis().SetValue(fConeIndex+1,y);
}

TVector3 ND::TMConeState::GetConeVariance() const {
    return TVector3(GetThis().GetCovarianceValue(fConeIndex,
                                                    fConeIndex),
                    GetThis().GetCovarianceValue(fConeIndex+1,
                                                    fConeIndex+1),
                    0);
}

void ND::TMConeState::SetConeVariance(double x, double y) {
    GetThis().SetCovarianceValue(fConeIndex,
                                         fConeIndex,x);
    GetThis().SetCovarianceValue(fConeIndex+1,
                                         fConeIndex+1,y);
}

ND::TCorrValues ND::TMConeState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
    TCorrValues values(TMConeState::GetSize());
    values.SetType("C1 C2 ");
    const TMConeState* coneState 
        = dynamic_cast<const TMConeState*>(GetPointer(proj));
    int base = 0;
    if (coneState) {
        const int offset = coneState->GetConeIndex();
        for (int i = 0; i < TMConeState::GetSize(); ++i) {
            values.SetValue(i+base,
                            coneState->GetThis().GetValue(i+offset));
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
ND::TMWidthState::~TMWidthState() {}

ND::TMWidthState::TMWidthState() {
    fWidthIndex = fLocalNames.size();
    fLocalNames.push_back("W1");
    fLocalNames.push_back("W2");
}


TVector3 ND::TMWidthState::GetWidth() const {
    return TVector3(GetThis().GetValue(fWidthIndex),
                    GetThis().GetValue(fWidthIndex+1),
                    0);
}

void ND::TMWidthState::SetWidth(double x, double y) {
    GetThis().SetValue(fWidthIndex,x);
    GetThis().SetValue(fWidthIndex+1,y);
}

TVector3 ND::TMWidthState::GetWidthVariance() const {
    return TVector3(GetThis().GetCovarianceValue(fWidthIndex,
                                                    fWidthIndex),
                    GetThis().GetCovarianceValue(fWidthIndex+1,
                                                    fWidthIndex+1),
                    0);
}

void ND::TMWidthState::SetWidthVariance(double x, double y) {
    GetThis().SetCovarianceValue(fWidthIndex,
                                         fWidthIndex,x);
    GetThis().SetCovarianceValue(fWidthIndex+1,
                                         fWidthIndex+1,y);
}

ND::TCorrValues ND::TMWidthState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMPositionDirectionState::~TMPositionDirectionState() {}

ND::TCorrValues ND::TMPositionDirectionState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
ND::TMPosDirCurvState::~TMPosDirCurvState() {}

ND::TCorrValues ND::TMPosDirCurvState::ProjectState(const ND::THandle<ND::TReconState>& proj) {
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
