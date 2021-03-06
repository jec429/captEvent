#include <limits>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "TReconCluster.hxx"
#include "TReconNode.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TReconCluster);

CP::TReconCluster::TReconCluster() 
    : fMoments(3), fTemporariesInitialized(false) {
    fState = new TClusterState;
    fNodes = new TReconNodeContainerImpl<CP::TClusterState>;
}

CP::TReconCluster::TReconCluster(const CP::TReconCluster& cluster)
    : CP::TReconBase(cluster), fMoments(3), fTemporariesInitialized(false) {
    fNodes = new TReconNodeContainerImpl<CP::TClusterState>;
    
    // Copy the nodes.  Create new nodes with TClusterState's
    CP::TReconNodeContainer::const_iterator in;
    for (in=cluster.GetNodes().begin(); in!=cluster.GetNodes().end();in++){
        CP::THandle<CP::TReconNode> node(new CP::TReconNode);
        CP::THandle<CP::TReconBase> object = (*in)->GetObject();
        node->SetObject(object);
        CP::THandle<CP::TClusterState> tstate = (*in)->GetState();
        if (tstate){
            CP::THandle<CP::TReconState> pstate(new CP::TClusterState(*tstate));
            node->SetState(pstate);
        }
        node->SetQuality((*in)->GetQuality());
        
        fNodes->push_back(node);
    }
    
    
    if (cluster.GetState()){
        CP::THandle<CP::TClusterState> state = cluster.GetState();  
        fState = new TClusterState(*state);
    }
    else {
        fState = new TClusterState;
    }
}

CP::TReconCluster::~TReconCluster() {}

double CP::TReconCluster::GetEDeposit() const {
    // I'm being a bit pedantic and casting to the base mix-in class.  This
    // could just as well cast to a TClusterState.
    const TMEDepositState* state 
        = dynamic_cast<const TMEDepositState*>(fState);
    if (!state) throw EMissingField();
    return state->GetEDeposit();
}
    
double CP::TReconCluster::GetEDepositVariance() const {
    // I'm being a bit pedantic and casting to the base mix-in class.  This
    // could just as well cast to a TClusterState.
    const TMEDepositState* state 
        = dynamic_cast<const TMEDepositState*>(fState);
    if (!state) throw EMissingField();
    return state->GetEDepositVariance();
}
    
TLorentzVector CP::TReconCluster::GetPosition() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TClusterState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPosition();
}

TLorentzVector CP::TReconCluster::GetPositionVariance() const {
    // This is the preferred way to access a state field.  
    THandle<CP::TClusterState> state = GetState();
    if (!state) throw EMissingField();
    return state->GetPositionVariance();
}

bool CP::TReconCluster::IsXCluster() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.X())) return false;
    return true;
}

bool CP::TReconCluster::IsYCluster() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Y())) return false;
    return true;
}

bool CP::TReconCluster::IsZCluster() const {
    TLorentzVector var = GetPositionVariance();
    if (CP::TCorrValues::IsFree(var.Z())) return false;
    return true;
}

int CP::TReconCluster::GetDimensions() const{
    TLorentzVector var = GetPositionVariance();
    int dim = 0;
    if (IsXCluster()) ++dim;
    if (IsYCluster()) ++dim;
    if (IsZCluster()) ++dim;
    return dim;
}

const CP::TReconCluster::MomentMatrix& CP::TReconCluster::GetMoments() const {
    return fMoments;
}

void CP::TReconCluster::SetMoments(double xx, double yy, double zz,
                                   double xy, double xz, double yz) {
    fTemporariesInitialized = false;
    fMoments(0,0) = xx;
    fMoments(1,1) = yy;
    fMoments(2,2) = zz;
    fMoments(0,1) = xy;
    fMoments(1,0) = xy;
    fMoments(0,2) = xz;
    fMoments(2,0) = xz;
    fMoments(1,2) = yz;
    fMoments(2,1) = yz;
}

void CP::TReconCluster::SetMoments(const TMatrixT<double>& moments) {
    if (moments.GetNrows() != fMoments.GetNrows()) throw EMomentsSize();
    if (moments.GetNcols() != fMoments.GetNcols()) throw EMomentsSize();
    fTemporariesInitialized = false;
    for (int row=0; row<3; ++row) {
        for (int col=0; col<3; ++col) {
            fMoments(row,col) = moments(row,col);
        }
    }
}

void CP::TReconCluster::UpdateFromHits() {
    fTemporariesInitialized = false;
    // Make sure there is a hit container.
    CP::THandle<CP::THitSelection> hits = GetHits();
    if (!hits) return;

    // Make sure the hit container isn't empty.
    CP::THitSelection::const_iterator beg = hits->begin();
    CP::THitSelection::const_iterator end = hits->end();
    if (end-beg < 1) return;

    fStatus = CP::TReconBase::kSuccess;
    fQuality = 1.0;
    fNDOF = std::max(1,int(end-beg-1));
    CP::THandle<CP::TClusterState> state = GetState();
    
    int dim = state->GetDimensions();
    TVectorT<double> vals(dim);
    TVectorT<double> sigs(dim);
    TVectorT<double> rms(dim);

    // Save the index into the state for each of the values.
    int eDep = state->GetEDepositIndex();
    int posX = state->GetXIndex();
    int posY = state->GetYIndex();
    int posZ = state->GetZIndex();
    int posT = state->GetTIndex();

    // Find the energy deposit and the average position.
    TVectorT<double> stateValues(dim);
    TVectorT<double> stateNorms(dim);
    for (CP::THitSelection::const_iterator h = beg;
         h != end;
         ++h) {
        vals(eDep) = (*h)->GetCharge();
        sigs(eDep) = 1.0;

        vals(posX) = (*h)->GetPosition().X();
        sigs(posX) = (*h)->GetUncertainty().X();

        vals(posY) = (*h)->GetPosition().Y();
        sigs(posY) = (*h)->GetUncertainty().Y();

        vals(posZ) = (*h)->GetPosition().Z();
        sigs(posZ) = (*h)->GetUncertainty().Z();

        vals(posT) = (*h)->GetTime();
        sigs(posT) = (*h)->GetTimeUncertainty();

        for (int i=0; i<dim; ++i) {
            stateValues(i) += vals(i)/(sigs(i)*sigs(i));
            stateNorms(i) += 1/(sigs(i)*sigs(i));
        }
    }
        
    stateNorms(0) = 1.0;
    for (int i=0; i<dim; ++i) {
        if (stateNorms(i) > 0) stateValues(i) /= stateNorms(i);
    }

    // Estimate the covariance of cluster state values.
    TMatrixTSym<double> stateCov(dim);
    // This counts the weight for each bin of the covariance.
    TMatrixTSym<double> weights(dim);
    // This counts the number of degrees of freedom contributing to each bin.
    TMatrixTSym<double> dof(dim);
    for (CP::THitSelection::const_iterator h = beg;
         h != end;
         ++h) {
        vals(eDep) = (*h)->GetCharge() - stateValues(eDep);
        sigs(eDep) = std::sqrt((*h)->GetCharge());
        rms(eDep) = 0.0;

        vals(posX) = (*h)->GetPosition().X() - stateValues(posX);
        sigs(posX) = (*h)->GetUncertainty().X();
        rms(posX) = (*h)->GetRMS().X();

        vals(posY) = (*h)->GetPosition().Y() - stateValues(posY);
        sigs(posY) = (*h)->GetUncertainty().Y();
        rms(posY) = (*h)->GetRMS().Y();
        
        vals(posZ) = (*h)->GetPosition().Z() - stateValues(posZ);
        sigs(posZ) = (*h)->GetUncertainty().Z();
        rms(posZ) = (*h)->GetRMS().Z();

        vals(posT) = (*h)->GetTime() - stateValues(posT);
        sigs(posT) = (*h)->GetTimeUncertainty();
        rms(posT) = (*h)->GetTimeRMS();

        for (int row = 0; row<dim; ++row) {
            for (int col = row; col<dim; ++col) {
                double weight = 1.0/(sigs(row)*sigs(col));
                stateCov(row,col) += weight*vals(row)*vals(col);
                weights(row,col) += weight;
                double degrees 
                    = 4*rms(row)*rms(col)/(12*sigs(row)*sigs(col));
                if (row == posT && col == posT) degrees = 1.0;
                dof(row,col) += degrees;
            }
        }
    }

    // Turn the "stateCov" variable into the RMS.
    for (int row = 0; row<dim; ++row) {
        for (int col = row; col<dim; ++col) {
            if (weights(row,col)>0) stateCov(row,col) /= weights(row,col);
            else stateCov(row,col) = 0.0;
            stateCov(col,row) = stateCov(row,col);
        }
    }

    // Turn the RMS into a covariance of the mean (This is almost a repeat of
    // turning the value into an RMS.
    for (int row = 0; row<dim; ++row) {
        for (int col = row; col<dim; ++col) {
            if (dof(row,col)>0.9) stateCov(row,col) /= std::sqrt(dof(row,col));
            else if (row==col) stateCov(row,col) = CP::TCorrValues::kFreeValue;
            else stateCov(row,col) = 0.0;
            stateCov(col,row) = stateCov(row,col);
        }
    }

    // Add the correction for finite size of the hits.
    TVectorT<double> hitWeights(dim);
    for (CP::THitSelection::const_iterator h = beg;
         h != end;
         ++h) {
        sigs(eDep) = std::sqrt((*h)->GetCharge());
        sigs(posX) = (*h)->GetUncertainty().X();
        sigs(posY) = (*h)->GetUncertainty().Y();
        sigs(posZ) = (*h)->GetUncertainty().Z();
        sigs(posT) = (*h)->GetTimeUncertainty();

        for (int idx = 0; idx<dim; ++idx) {
            double weight = 1.0/(sigs(idx)*sigs(idx));
            hitWeights(idx) += weight;
        }
    }
    
    for (int idx = 0; idx<dim; ++idx) {
        if (hitWeights(idx)<1E-8) continue;
        stateCov(idx,idx) += 1.0/hitWeights(idx);
    }
    
    // Fix the variance of the deposited energy.  This assumes it's Poisson
    // distributed.
    stateCov(eDep,eDep) = stateValues(eDep);

    // Set the state value and covariance.
    for (int row=0; row<dim; ++row) {
        state->SetValue(row,stateValues(row));
        for (int col=0; col<dim; ++col) {
            state->SetCovarianceValue(row,col,stateCov(row,col));
            state->SetCovarianceValue(col,row,stateCov(row,col));
        }
    }

    // Find the moments of the cluster.  This could be done as part of the
    // covariance calculation, but is seperated so it's easier to see what is
    // going on.
    TMatrixTSym<double> moments(3);
    TMatrixTSym<double> chargeSum(3);
    TVector3 center(state->GetValue(posX), 
                    state->GetValue(posY), 
                    state->GetValue(posZ));

    // Calculate the charge weighted "sum of squared differences".
    for (CP::THitSelection::const_iterator h = beg;
         h != end;
         ++h) {
        TVector3 diff = (*h)->GetPosition() - center;
        for (int row = 0; row<3; ++row) {
            for (int col = row; col<3; ++col) {
                moments(row,col) += diff(row)*diff(col)*(*h)->GetCharge();
                // Correct for the finite size of the hit.
                if (row==col) {
                    double r = (*h)->GetRMS()[row];
                    moments(row,col) += r*r*(*h)->GetCharge();
                }
                chargeSum(row,col) += (*h)->GetCharge();
            }
        }
    }

    // Turn this into the average square difference (i.e. the moments)
    for (int row = 0; row<3; ++row) {
        for (int col = row; col<3; ++col) {
            // No divide by zero please!
            if (chargeSum(row,col) > 1E-6) {
                moments(row,col) = moments(row,col)/chargeSum(row,col);
            } 
            else if (row == col) {
                // There were no measurements on this axis so spread the
                // charge over the entire range of the detector 
                // (i.e. +-"10 m + epsilon")
                moments(row,col) = 1E+9;
            }
            else {
                // There were no measurements so the aren't any off-axis
                // correlations.
                moments(row,col) = 0;
            }
            // Keep it symmetric.
            moments(col,row) = moments(row,col);
        }
    }

    SetMoments(moments);
}

void CP::TReconCluster::ls(Option_t *opt) const {
    CP::TReconBase::ls_base(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();

    if (fState) {
        TROOT::IncreaseDirLevel();
        fState->ls(opt);
        TROOT::DecreaseDirLevel();
    }

    std::ios::fmtflags save = std::cout.flags();
    for (int i = 0; i<3; ++i) {
        TROOT::IndentLevel();
        if (i == 0) std::cout << "Position Cov: ";
        else        std::cout << "              ";
        THandle<CP::TClusterState> state = GetState();
        for (int j = 0; j<3; ++j) {
            std::cout << std::setw(12) << state->GetPositionCovariance(i,j);
        }
        std::cout << std::setw(0) << std::endl;
    }
    for (int i = 0; i<3; ++i) {
        TROOT::IndentLevel();
        if (i == 0) std::cout << "Moments:      ";
        else        std::cout << "              ";
        for (int j = 0; j<3; ++j) {
            std::cout << std::setw(12) << fMoments(i,j);
        }
        std::cout << std::setw(0) << std::endl;
    }
    std::cout.flags(save);

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

void CP::TReconCluster::FillTemporaries() const {
    if (fTemporariesInitialized) return;
    fTemporariesInitialized = true;

    TVectorF eigenValues;
    TMatrixF eigenVectors(GetMoments().EigenVectors(eigenValues));

    // Long axis comes first.
    fLongAxis.SetXYZ(eigenVectors(0,0), eigenVectors(1,0), eigenVectors(2,0));
    fLongAxis = std::sqrt(eigenValues(0))*fLongAxis;

    // Major axis comes second.
    fMajorAxis.SetXYZ(eigenVectors(0,1), eigenVectors(1,1), eigenVectors(2,1));
    fMajorAxis = std::sqrt(eigenValues(1))*fMajorAxis;

    // Minor axis comes last.
    fMinorAxis.SetXYZ(eigenVectors(0,2), eigenVectors(1,2), eigenVectors(2,2));
    fMinorAxis = std::sqrt(eigenValues(2))*fMinorAxis;

    const double epsilon = 1E-6;
    if (fLongAxis.X() < -epsilon) {
        fLongAxis = -fLongAxis;
    }
    else if (fLongAxis.X() < epsilon && fLongAxis.Y() < -epsilon) {
        fLongAxis = -fLongAxis;
    }
    else if (fLongAxis.Y() < epsilon && fLongAxis.Z() < 0.0) {
        fLongAxis = -fLongAxis;
    }

    if (fMajorAxis.Y() < -epsilon) {
        fMajorAxis = -fMajorAxis;
    }
    else if (fMajorAxis.Y() < epsilon && fMajorAxis.Z() < -epsilon) {
        fMajorAxis = -fMajorAxis;
    }
    else if (fMajorAxis.Z() < epsilon && fMajorAxis.X() < 0.0) {
        fMajorAxis = -fMajorAxis;
    }

    TVector3 temp = fLongAxis.Cross(fMajorAxis);
    if (temp*fMinorAxis < 0) {
        fMinorAxis = -fMinorAxis;
    }

}

const TVector3& CP::TReconCluster::GetLongAxis() const {
    FillTemporaries();
    return fLongAxis;
}

const TVector3& CP::TReconCluster::GetMajorAxis() const {
    FillTemporaries();
    return fMajorAxis;
}

const TVector3& CP::TReconCluster::GetMinorAxis() const {
    FillTemporaries();
    return fMinorAxis;
}

double CP::TReconCluster::GetLongExtent() const {
    FillTemporaries();

    double maxLen = 0.0;
    CP::THandle<CP::THitSelection> hits = GetHits();
    for (CP::THitSelection::iterator h = hits->begin(); 
         h != hits->end(); ++h) {
        TVector3 diff = (*h)->GetPosition() - GetPosition().Vect();
        maxLen = std::max(maxLen,
                          (*h)->GetRMS().Mag() + std::abs(diff*fLongAxis));
    }
    maxLen /= fLongAxis.Mag();
    return maxLen;

}

double CP::TReconCluster::GetMajorExtent() const {
    FillTemporaries();

    double maxLen = 0.0;
    CP::THandle<CP::THitSelection> hits = GetHits();
    for (CP::THitSelection::iterator h = hits->begin(); 
         h != hits->end(); ++h) {
        TVector3 diff = (*h)->GetPosition() - GetPosition().Vect();
        maxLen = std::max(maxLen, 
                          (*h)->GetRMS().Mag() + std::abs(diff*fMajorAxis));
    }
    maxLen /= fMajorAxis.Mag();
    return maxLen;

}

double CP::TReconCluster::GetMinorExtent() const {
    FillTemporaries();

    double maxLen = 0.0;
    CP::THandle<CP::THitSelection> hits = GetHits();
    for (CP::THitSelection::iterator h = hits->begin(); 
         h != hits->end(); ++h) {
        TVector3 diff = (*h)->GetPosition() - GetPosition().Vect();
        maxLen = std::max(maxLen,
                          (*h)->GetRMS().Mag() + std::abs(diff*fMinorAxis));
    }
    maxLen /= fMinorAxis.Mag();
    return maxLen;

}
