#ifndef TReconCluster_hxx_seen
#define TReconCluster_hxx_seen

#include <TMatrixT.h>
#include <TMatrixTSym.h>

#include "THandle.hxx"
#include "TReconBase.hxx"
#include "TClusterState.hxx"
#include "ECore.hxx"

namespace CP {
    class TReconCluster;

    EXCEPTION(EReconCluster,ECore);
    EXCEPTION(EMomentsSize,EReconCluster);
}

/// Represent an extended energy deposition centered at a position (i.e. a
/// spheroidal blob of energy).  This type of energy deposit is described by
/// the amount of energy, and the central position of the deposit
/// (deposit,position,time) in an CP::TClusterState object.  The specific hits
/// associated with the cluster are available through the GetHits method.  The
/// moments of the distribution can be retrieved using the GetMoments()
/// method.
/// 
/// The CP::TReconCluster class is intended to describe the geometry of the
/// energy deposition in a detector, and not make the association with a
/// particular particle identification.  Assignment of particle types to the
/// detector measurements is done in the CP::TReconPID class.
class CP::TReconCluster: public CP::TReconBase {
public: 
    typedef TMatrixTSym<float> MomentMatrix;

    TReconCluster();

    /// copy constructor
    TReconCluster(const CP::TReconCluster& cluster);

    virtual ~TReconCluster();

    /// Return a handle to the state.
    CP::THandle<CP::TClusterState> GetState() const {
        return GetReconState();
    }

    /// Get the energy deposited in the cluster.
    double GetEDeposit() const;

    /// Get the variance of the energy deposited in the cluster.
    double GetEDepositVariance() const;

    /// Get the cluster position.
    TLorentzVector GetPosition() const;

    /// Get the track starting position uncertainty.
    TLorentzVector GetPositionVariance() const;

    /// Get the number of (non-free) spacial dimensions 
    int GetDimensions() const;

    /// Check if this cluster has X information.
    bool IsXCluster() const;

    /// Check if this cluster has Y information.
    bool IsYCluster() const;

    /// Check if this cluster has Z information.
    bool IsZCluster() const;

    /// Get the moments of the charge distribution.
    const MomentMatrix& GetMoments() const;
    
    /// Set the moments of the charge distribution.
    void SetMoments(double xx, double yy, double zz, 
                    double xy, double xz, double yz);

    /// Set the moments of the charge distribution.  The input matrix is
    /// assumed to be symmetric and only the upper half of the elements are
    /// accessed.
    void SetMoments(const TMatrixT<double>& moments);
    
    /// Fill the CP::TReconCluster and CP::TClusterState objects from hits
    /// between the begin and end iterators.  The position is set to average
    /// hit position (the hit position uncertainty is used), and the energy
    /// deposit is the sum of the hit charges.  The algorithm name is set to
    /// the first argument.
    ///
    /// \note Since FillFromHits is typically used with 3D hit objects
    /// (i.e. TReconHit), care must be taken in the reconstruction to make
    /// sure that the charge is properly shared among the hits.  For instance,
    /// if a particular hit contributes to several 3D hits (the usual case),
    /// its charge must be distributed over all of those hits.
    ///
    /// \note If FillFromHits is used with 2D hits (i.e. U, V, and X hits),
    /// the resulting covariances, and energy deposits will have been
    /// calculated using incorrect assumptions.
    template <typename T> 
    void FillFromHits(const char* name, T begin,T end) {
        // Set the algorithm name.
        fAlgorithm = std::string(name);
        
        // Add a copy of the hits to the cluster.
        if (end == begin) return;
        CP::THitSelection* hits = new THitSelection("clusterHits");
        std::copy(begin, end, std::back_inserter(*hits));
        AddHits(hits);
        
        // Update the cluster fields based on the hits.
        UpdateFromHits();
    }


    /// A convenience method to fill the cluster from a CP::THitSelection.
    /// The hits are copied into the the cluster, so this does not take owner
    /// ship of the original CP::THitSelection.
    void FillFromHits(const char* name, const CP::THitSelection& hits) {
        FillFromHits(name, hits.begin(), hits.end());
    }

    /// List the results of in the cluster.
    virtual void ls(Option_t* opt = "") const; 

    /// A convenience routine to return the long axis of the cluster.  The
    /// vector points in the direction of the long axis, and the magnitude is
    /// equal to the moment along the axis.
    const TVector3& GetLongAxis();

    /// A convenience routine to return the major axis perpendicular to the
    /// long axis of the cluster.  The vector points in the direction of the
    /// major axis, and the magnitude is equal to the moment along the axis.
    const TVector3& GetMajorAxis();

    /// A convenience routine to return the minor axis perpendicular to the
    /// long axis of the cluster.  The vector points in the direction of the
    /// minor axis, and the magnitude is equal to the moment along the axis.
    const TVector3& GetMinorAxis();

    /// A convenience routine to return the distance to the furthest hit from
    /// the cluster position along the long axis of the cluster.
    double GetLongExtent();

    /// A convenience routine to return the distance to the furthest hit from
    /// the cluster position along the major axis of the cluster.
    double GetMajorExtent();

    /// A convenience routine to return the distance to the furthest hit from
    /// the cluster position along the minor axis of the cluster.
    double GetMinorExtent();

private:

    /// Fill all of the fields of the cluster based on the hits.
    void UpdateFromHits();

    /// The moments for this cluster.
    MomentMatrix fMoments;
    
    /// Fill the temporary fields.
    bool fTemporariesInitialized; //! Don't Save
    void FillTemporaries();

    /// The cached length of the cluster calculated from the eigenvectors of
    /// the moments.
    TVector3 fLongAxis; //! Don't Save

    /// The cached major axis (perpendicular to the length) calculated from
    /// the eigenvectors of the moments.
    TVector3 fMajorAxis; //! Don't Save

    /// The cached major axis (perpendicular to the length) calculated from
    /// the eigenvectors of the moments.
    TVector3 fMinorAxis; //! Don't Save

    ClassDef(TReconCluster,1);
};
#endif
