#include "TReconVertex.hxx"

ClassImp(CP::TReconVertex);

CP::TReconVertex::TReconVertex() {
    fState = new TVertexState;
    fNodes = new TReconNodeContainerImpl<CP::TVertexState>;
}

CP::TReconVertex::~TReconVertex() {}

TLorentzVector CP::TReconVertex::GetPosition() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TVertexState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPosition();
}

TLorentzVector CP::TReconVertex::GetPositionVariance() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TVertexState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPositionVariance();
}

bool CP::TReconVertex::IsXVertex() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.X())) return false;
    return true;
}

bool CP::TReconVertex::IsYVertex() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Y())) return false;
    return true;
}

bool CP::TReconVertex::IsZVertex() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Z())) return false;
    return true;
}

int CP::TReconVertex::GetDimensions() const{
    TLorentzVector var = GetPositionVariance();
    int dim = 0;
    if (IsXVertex()) ++dim;
    if (IsYVertex()) ++dim;
    if (IsZVertex()) ++dim;
    return dim;
}

