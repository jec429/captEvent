#ifndef TAlgorithm_hxx_seen
#define TAlgorithm_hxx_seen

#include <string>
#include <TTask.h>
#include <TGeoManager.h>

#include "TEvent.hxx"
#include "THitSelection.hxx"
#include "TAlgorithmTag.hxx"
#include "TAlgorithmResult.hxx"
#include "TEventFolder.hxx"
#include "ECore.hxx"

namespace CP {
    class TAlgorithm;

    /// An exception thrown if an algorithm expecting an event is run without
    /// a current event available.
    EXCEPTION(EAlgorithmWithoutEvent,ECore);
}

/// Base class for reconstruction algorithms.  It contains the basic
/// information about the "flavor" and version.  Specific algorithms should
/// inherit the class and returned results using a TAlgorithmResult object.
/// Inherited classes must implement the
///
/// \code
/// THandle<TAlgorithmResult> Process(const TAlgorithmResult& input)
/// \endcode
///
/// which is a pure virtual member of this class.  The user should use
/// TAlgorithm::CreateResult() to construct an empty result that is returned
/// by the Process method.  This convenience function will set the algorithm
/// result name, and fill the bookkeeping information.
///
/// By convention, the algorithm should fill the TAlgorithmResult with some
/// standard TReconObjectContainers and THitSelections.  See the
/// TAlgorithmResult documentation for more information.
///
/// TAlgorithmResult objects are often created by one TAlgorithm, and then
/// used as input to the next TAlgorithm.  In that case, the TAlgorithm will
/// use the last container of objects, and hit selection for it's input.
///
class CP::TAlgorithm : public TNamed {
public:
    TAlgorithm(const char* name, const char* title="An Algorithm");
    virtual ~TAlgorithm();
    
    /// This method returns information about the compiled version 
    virtual const std::string& GetVersion() const {return fVersion;}
    
    /// The routine that does the actual work.  This must be implemented by
    /// any derived class.  By convention, the derived class should only name
    /// the parameters that are actually used.  For instance, a derived class
    /// that only uses the first TAlgorithmResult would be declared as
    /// \code
    /// THandle<CP::TAlgorithmResult> 
    /// Process(const CP::TAlgorithmResult& input,
    ///      const CP::TAlgorithmResult& input1 = CP::TAlgorithmResult::Empty,
    ///      const CP::TAlgorithmResult& input2 = CP::TAlgorithmResult::Empty);
    /// \endcode
    /// and defined as
    /// \code
    /// THandle<CP::TAlgorithmResult> 
    /// Process(const CP::TAlgorithmResult& input,
    ///         const CP::TAlgorithmResult&,
    ///         const CP::TAlgorithmResult&) { ... }
    /// \endcode
    virtual THandle<CP::TAlgorithmResult> 
    Process(const CP::TAlgorithmResult& input,
            const CP::TAlgorithmResult& input1 = CP::TAlgorithmResult::Empty,
            const CP::TAlgorithmResult& input2 = CP::TAlgorithmResult::Empty
        ) = 0;

    /// Reset algorithm containers, but keep do not change the
    /// parameters by default
    virtual void Clear(Option_t* option);

    /// Get the algorithm tag.
    TAlgorithmTag GetTag() const;

protected:
    /// Set the version of the algorithm.  This should be set in the derived
    /// classes constructor.
    void SetVersion(const char* v);

    /// A convenient way to get the event being processed by the algorithm.
    /// The returned event is only valid during a single call to Process so
    /// the pointer can not be saved in a field.
    CP::TEvent& GetEvent() const;
    
    /// A convenient way to get the correct geometry for the current event.
    /// If there isn't a valid geometry available, this will return NULL.  As
    /// a side effect, the gGeoManager global variable is set.
    TGeoManager* GetGeom() const;
  
    /// A convenient way to create an empty algorithm result to be returned by
    /// a derived class.
    THandle<CP::TAlgorithmResult> CreateResult() {
        return THandle<TAlgorithmResult>(new TAlgorithmResult(*this));
    }

private:
    /// A version string that must be set in the constructor with SetVersion().
    std::string fVersion;

    ClassDef(TAlgorithm, 1);
};
#endif
