#ifndef TInputManager_hxx_seen
#define TInputManager_hxx_seen

#include "ECore.hxx"

#include <string>
#include <vector>

namespace CP {
    class TManager;
    class TVInputFile;
    class TInputManager;
    class TVInputBuilder;

    /// An exception from the data base.
    EXCEPTION(ENoInputBuilder, ECore);

}

/// A class to build a particular type of input reader.  The input builders
/// are used by the TInputManager to create new TVInputFile derived objects
/// which understand specific input formats.  Not all of the readers are
/// implemented in captEvent (some reside in optional libraries).  As an
/// example, the TRootInput reader is in captEvent, but the TUBooneInput
/// reader is going to be found in captTrans.
///
/// An object from each TVInputBuilder daughter class should be added to the
/// TInputManager using the TInputManager::Register() method.  The standard
/// idiom in the builder declaration file is:
///
/// \code
/// namespace {
///   class TMyOwnInputRegistration {
///    public:
///        TMyOwnInputRegistration() {
///            CP::TManager::Get().Input().Register(new TMyOwnInputBuilder());
///        }
///    };
///    TMyOwnInputRegistration registrationObject;
/// }
/// \endcode
///
/// which will automatically register TMyOwnInputBuilder to the list of known
/// builders when the library containing TMyOwnInputBuilder is loaded.
///
class CP::TVInputBuilder {
    friend CP::TInputManager;

public:

    /// Create a new builder.  The builder name is used by TInputManager to
    /// find the right builder to return.  It should be limited to letters and
    /// numbers.  All the character up to, but not including, the first "("
    /// will be used by the TInputManager to find the builder
    TVInputBuilder(const char* name, const char* documentation);
    virtual ~TVInputBuilder();

    /// Build a new TVInputFile derived object.  The building of the
    /// TVInputFile object can be controled using arguments that have been set
    /// using the SetArguments() method.  The arguments are builder specific,
    /// and a builder is free to ignore any values set using SetArguments.
    virtual CP::TVInputFile* Open(const char* fileName) const = 0;

    /// Get the name of the builder
    std::string GetName() const {return fName;}

    /// Get the documentation for the builder
    std::string GetDocumentation() const {return fDocumentation;}

protected:
    /// Set the current arguments for the builder.  These are available when
    /// the file is being opened.  This can be "const" since the fArguments
    /// field is a mutable field.
    void SetArguments(const char* arguments) const {fArguments = arguments;}

    /// Get the current arguments for the builder.  This should be used by the
    /// builder when it's opening the file.  The arguments will be the full
    /// string passed to the TInputManager::Build() method.  For instance, the
    /// following lines
    ///
    /// \code
    /// CP::TManager::Get().Input().Builder("aBuilder(abc,def)")
    /// \endcode
    ///
    /// Will get the builder name "aBuilder" with the return value of
    /// GetArguments() being "aBuilder(abc,def)"
    std::string GetArguments() const {return fArguments;}

private:
    /// The name of the builder.
    std::string fName;

    /// The documentation.
    std::string fDocumentation;

    /// The builder arguments.
    mutable std::string fArguments;

};

/// This class manages the available TVInputFile derived classes, this is a
/// singleton managed by TManager.  The event loop will use Create to make the
/// actual reader.
class CP::TInputManager {
    friend class CP::TManager;
public:
    virtual ~TInputManager();

    /// Register a new TVInputFile derived class.  Classes that are registered
    /// in this way can be used to read files with the event loop.  The method
    /// takes ownership of the builder.
    void Register(TVInputBuilder* builder);

    /// Create a registered TVInputFIle derived class.  The new input class
    /// will belong to the caller (and it is the caller's responsibility to
    /// delete the object).  This will return NULL if the reader name is
    /// unknown.
    const TVInputBuilder& Builder(const char* name);

    /// Get a builder by index
    const TVInputBuilder& Builder(int i);

    /// Get the number of builders
    int BuilderCount() const;

private:

    /// Construct a new TGeomIdManager.  This is private since it should only
    /// be constructed by the friend class TManager.
    TInputManager();

    /// A vector of builders that can be used.
    std::vector<const TVInputBuilder*> fBuilders;
};
#endif
