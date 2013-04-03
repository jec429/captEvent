#include "TReconPID.hxx"

ClassImp(CP::TReconPID);

CP::TReconPID::TReconPID() {
    fParticleId= kNotSet;
    fParticleWeight = 0;
    fState = new TPIDState;
    fNodes = new TReconNodeContainerImpl<CP::TPIDState>;
}


void CP::TReconPID::CopyTReconPID(const CP::TReconPID& pid){
    fNodes = new TReconNodeContainerImpl<CP::TPIDState>;
    
    fParticleId = pid.GetParticleId();
    fParticleWeight = pid.GetPIDWeight();
    
    // Copy the nodes 
    // Create new nodes with TPIDState's 
    CP::TReconNodeContainer::const_iterator in;
    for (in=pid.GetNodes().begin(); in!=pid.GetNodes().end();in++){
        CP::THandle<CP::TReconNode> node(new CP::TReconNode);
        CP::THandle<CP::TReconBase> object = (*in)->GetObject();
        node->SetObject(object);
        CP::THandle<CP::TPIDState> tstate = (*in)->GetState();
        if (tstate){
            CP::THandle<CP::TReconState> pstate(new CP::TPIDState(*tstate));
            node->SetState(pstate);
        }
        node->SetQuality((*in)->GetQuality());
        
        fNodes->push_back(node);
    }
    
    
    if (pid.GetState()){
        CP::THandle<CP::TPIDState> state = pid.GetState();  
        fState = new TPIDState(*state);
    }
    else { 
        fState = new TPIDState;
    }
}

CP::TReconPID::TReconPID(const CP::TReconPID& pid, int i)
    : CP::TReconBase(pid) {
    // integer input parameter is dummy. Just to use a diferent constructor
    // which does not copy the alternates
    CopyTReconPID(pid);
}


CP::TReconPID::TReconPID(const CP::TReconPID& pid)
    : CP::TReconBase(pid) {
    CopyTReconPID(pid);
    // copy the alternates
    CP::TReconObjectContainer::const_iterator it;
    for (it = pid.GetAlternates().begin();
         it != pid.GetAlternates().end(); ++it) {
        CP::THandle<CP::TReconPID> alter = *it;
        AddAlternate(alter->GetParticleId(),alter->GetPIDWeight());
    }
}


CP::TReconPID::TReconPID(CP::THandle<CP::TReconTrack> track)
    : CP::TReconBase(*track){

  fParticleId= kNotSet;
  fParticleWeight = 0;
  fNodes = new TReconNodeContainerImpl<CP::TPIDState>;


  // copy the state by converting it from TTrackState to TPIDState
  CP::THandle<CP::TTrackState> state = track->GetState();  
  if (state) {
      fState = new TPIDState(*state);
  }
  else { 
      fState = new TPIDState;
  }
  
  // Copy the nodes 
  // Create new nodes with TPIDState's 
  TReconNodeContainer& nodes = GetNodes();
  GetNodes().clear();

  for (CP::TReconNodeContainer::const_iterator in=track->GetNodes().begin(); 
       in!=track->GetNodes().end();
       ++in){
      CP::THandle<CP::TTrackState> tstate = (*in)->GetState();
      CP::THandle<CP::TReconBase> object = (*in)->GetObject();
      if (!tstate) {
          ND280Error("Track node without track state");
          continue;
      }
      CP::THandle<CP::TReconNode> node(new CP::TReconNode);
      CP::THandle<CP::TReconState> pstate(new CP::TPIDState(*tstate));
      node->SetObject(object);
      node->SetState(pstate);
      node->SetQuality((*in)->GetQuality());
      nodes.push_back(node);
  }

  // remove constituents, since the input object has been promoted 
  // to a TReconPID. This will have a unique constituent: the input object 
  TDatum* c = FindDatum("constituents");
  if (c){
    erase(c);
    delete c;
  }


  // add the track as constituent (notice that this is not really a
  // constituent but the promoted object)
  AddConstituent(track);
}


CP::TReconPID::TReconPID(CP::THandle<CP::TReconShower> shower)
  : CP::TReconBase(*shower) {
  fParticleId= kNotSet;
  fParticleWeight = 0;
  fNodes = new TReconNodeContainerImpl<CP::TPIDState>;

  // copy the state by converting it from TShowerState to TPIDState
  CP::THandle<CP::TShowerState> state = shower->GetState();  
  if (state) {
    fState = new TPIDState(*state);
  }
  else {
    fState = new TPIDState;
  }

  // Copy the nodes 
  // Create new nodes with TPIDState's 
  TReconNodeContainer& nodes = GetNodes();
  GetNodes().clear();
  if (shower->GetNodes().size()>0) {
      for (CP::TReconNodeContainer::const_iterator in
               = shower->GetNodes().begin();
           in!=shower->GetNodes().end();
           in++) {
          CP::THandle<CP::TShowerState> tstate = (*in)->GetState();
          CP::THandle<CP::TReconBase> object = (*in)->GetObject();
          if (!tstate) {
              ND280Error("Shower node without shower state");
              continue;
          }
          CP::THandle<CP::TReconNode> node(new CP::TReconNode);
          CP::THandle<CP::TReconState> pstate(new CP::TPIDState(*tstate));
          node->SetObject(object);
          node->SetState(pstate);
          node->SetQuality((*in)->GetQuality());
          nodes.push_back(node);
      }
  }
  else {
      CP::THandle<CP::TShowerState> tstate = shower->GetState();
      CP::THandle<CP::TReconBase> object = shower;
      if (!tstate) {
          ND280Error("Shower node without shower state");
          
      }
      else {
          CP::THandle<CP::TReconNode> node(new CP::TReconNode);
          CP::THandle<CP::TReconState> pstate(new CP::TPIDState(*tstate));
          node->SetObject(object);
          node->SetState(pstate);
          node->SetQuality(shower->GetQuality());
          nodes.push_back(node);
      }
  }

  // remove constituents, since the input object has been promoted 
  // to a TReconPID. This will have a unique constituent: the input object 
  TDatum* c = FindDatum("constituents");
  if (c){
    erase(c);
    delete c;
  }


  // add the shower as constituent (notice that this is not really a
  // constituent but the promoted object)
  AddConstituent(shower);

}

CP::TReconPID::~TReconPID() {}

TLorentzVector CP::TReconPID::GetPosition() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TPIDState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPosition();
}

TLorentzVector CP::TReconPID::GetPositionVariance() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TPIDState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPositionVariance();
}

bool CP::TReconPID::IsXPID() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.X())) return false;
    return true;
}

bool CP::TReconPID::IsYPID() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Y())) return false;
    return true;
}

bool CP::TReconPID::IsZPID() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Z())) return false;
    return true;
}

int CP::TReconPID::GetDimensions() const{
    TLorentzVector var = GetPositionVariance();
    int dim = 0;
    if (IsXPID()) ++dim;
    if (IsYPID()) ++dim;
    if (IsZPID()) ++dim;
    return dim;
}

TVector3 CP::TReconPID::GetDirection() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TPIDState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetDirection();
}

double CP::TReconPID::GetMomentum() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TPIDState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetMomentum();
}

double CP::TReconPID::GetCharge() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TPIDState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetCharge();
}


void CP::TReconPID::AddAlternate(ParticleId id, double weight) {
  // erase a previous alternate if it has the same id
  CP::TReconObjectContainer::iterator it;
  for (it = fAlternatives.begin(); it != fAlternatives.end(); it++){
    CP::THandle<CP::TReconPID> pid = *it;
    if ( pid->GetParticleId() == id){ 
      fAlternatives.erase(it);
      break;
    }
  }

  // create a new TReconPID copying the main TReconPID. Don't copy the
  // alternates.  is there any other solution ?
  CP::THandle<CP::TReconPID> alter(new CP::TReconPID(*this,1));

  // set the new information
  alter->SetParticleId(id);
  alter->SetPIDWeight(weight);

  AddAlternate(alter);

}

std::string CP::TReconPID::ConvertParticleId() const {
    return ConvertParticleId(fParticleId);
}

std::string CP::TReconPID::ConvertParticleId(CP::TReconPID::ParticleId id){
    std::string s("");

    if (id == kNotSet)     s= "Not Set";
    if (id == kOther)      s= "Other";
    if (id == kShower)     s= "Shower";
    if (id == kEM)         s= "EM";
    if (id == kElectron)   s= "Electron";
    if (id == kGamma)      s= "Gamma";
    if (id == kHadronic)   s= "Hadronic";
    if (id == kPiZero)     s= "Pi0";
    if (id == kLightTrack) s= "Light track";
    if (id == kMuon)       s= "Muon";
    if (id == kPion)       s= "Pion";
    if (id == kHeavyTrack) s= "Heavy track";
    if (id == kProton)     s= "Proton";
    if (id == kKaon)       s= "Kaon";

    return s;
}

void CP::TReconPID::ls(Option_t *opt) const {
    CP::TReconBase::ls_base(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Particle Id: " << ConvertParticleId() 
              << "     Weight: " << GetPIDWeight() 
              << std::endl;

    if (fState) {
        TROOT::IncreaseDirLevel();
        fState->ls(opt);
        TROOT::DecreaseDirLevel();
    }

    TROOT::IncreaseDirLevel();
    fAlternatives.ls(opt);
    TROOT::DecreaseDirLevel();

    if (fNodes) {
        TROOT::IncreaseDirLevel();
        fNodes->ls(opt);
        TROOT::DecreaseDirLevel();
    }

    TROOT::IncreaseDirLevel();
    for (const_iterator v = begin(); v != end(); ++v) {
        (*v)->ls(opt);
    };
    TROOT::DecreaseDirLevel();

    TROOT::DecreaseDirLevel();
}
