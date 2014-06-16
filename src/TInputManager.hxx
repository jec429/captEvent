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

/// A class to build a particular type of input reader.  This is added to the
/// TInputManager::Register method to allow the event loop to access the
/// available TVInputFile classes.  Not all of the readers are implemented in
/// captEvent (some reside in optional libraries).
class CP::TVInputBuilder {
public:

    /// Add a new builder.
    TVInputBuilder(const char* name, const char* documentation);
    virtual ~TVInputBuilder();

    /// Build a new TVInputFile derived object.
    virtual CP::TVInputFile* Open(const char* fileName) const = 0;

    /// Get the name of the builder
    std::string GetName() const {return fName;}

    /// Get the documentation for the builder
    std::string GetDocumentation() const {return fDocumentation;}

private:
    /// The name of the builder.
    std::string fName;

    /// The documentation.
    std::string fDocumentation;
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
