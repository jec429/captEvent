#include "TReconShower.hxx"

ClassImp(CP::TReconShower);

CP::TReconShower::TReconShower() {
    fState = new TShowerState;
    fNodes = new TReconNodeContainerImpl<CP::TShowerState>;
}

CP::TReconShower::TReconShower(const CP::TReconShower& shower)
    : CP::TReconBase(shower) {
    
    fNodes = new TReconNodeContainerImpl<CP::TShowerState>;
    
    // Copy the nodes 
    // Create new nodes with TShowerState's 
    CP::TReconNodeContainer::const_iterator in;
    for (in=shower.GetNodes().begin(); in!=shower.GetNodes().end();in++){
        CP::THandle<CP::TReconNode> node(new CP::TReconNode);
        CP::THandle<CP::TReconBase> object = (*in)->GetObject();
        node->SetObject(object);
        CP::THandle<CP::TShowerState> tstate = (*in)->GetState();
        if (tstate){
            CP::THandle<CP::TReconState> pstate(new CP::TShowerState(*tstate));
            node->SetState(pstate);
        }
        node->SetQuality((*in)->GetQuality());
        
        fNodes->push_back(node);
    }
    
    
    if (shower.GetState()) {
        CP::THandle<CP::TShowerState> state = shower.GetState();  
        fState = new TShowerState(*state);
    }
    else {
        fState = new TShowerState;
    }
}

CP::TReconShower::~TReconShower() {}

double CP::TReconShower::GetEDeposit() const {
    THandle<CP::TShowerState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetEDeposit();
}
    
TLorentzVector CP::TReconShower::GetPosition() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TShowerState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPosition();
}


TLorentzVector CP::TReconShower::GetPositionVariance() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TShowerState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPositionVariance();
}

bool CP::TReconShower::IsXShower() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.X())) return false;
    return true;
}

bool CP::TReconShower::IsYShower() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Y())) return false;
    return true;
}

bool CP::TReconShower::IsZShower() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Z())) return false;
    return true;
}

int CP::TReconShower::GetDimensions() const{
    TLorentzVector var = GetPositionVariance();
    int dim = 0;
    if (IsXShower()) ++dim;
    if (IsYShower()) ++dim;
    if (IsZShower()) ++dim;
    return dim;
}

TVector3 CP::TReconShower::GetDirection() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TShowerState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetDirection();
}

double CP::TReconShower::GetConeAngle() const {
    THandle<CP::TShowerState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetCone();
}
