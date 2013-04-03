//////////////////////////////////////////////////////////////
// $Id: ECore.hxx,v 1.5 2010/02/01 17:39:16 mcgrew Exp $
//
#ifndef ECore_HXX_SEEN
#define ECore_HXX_SEEN

#include <exception>
#include <string>

namespace CP {
    class ECore;
}

/// The root exception for all exceptions explicitly thrown by the oaCore
/// library.  All exceptions declared by this library should be derived from
/// this class, and not std::exception.  New exceptions can be derived from
/// ECore by creating a new class
///
/// \code
/// class EoaChild :public ECore {
/// public:
///     EoaChild() {AppendWhat("EoaChild");};
/// };
/// \endcode
///
/// But the prefered way to create derived exception classes is to use the
/// EXCEPTION macro.  This is used as follows 
/// 
/// \code
/// EXCEPTION(EChild,ECore);
/// EXCEPTION(EGrandChild,EChild);
/// ...
/// try {
///   throw EChild();
/// }
/// catch (ECore& ex) {
///   std::cout << ex.what << std::endl;
/// }
/// \endcode
///
/// This can add a backtrace at the point of the exception.  The number of
/// backtrace frames to be shown is controlled with the
/// CP::ECore::gBacktraceSymbols static variable.
/// 
class CP::ECore :public std::exception {
    /// What exception generated this object.
    char fWhat[2048];

public:
    /// The number of backtrace symbols to add to the "what" string.
    static unsigned int gBacktraceSymbols;

    ECore();
    virtual ~ECore() throw() {}

    /// Inherited from exception to return the name of the exception as a null
    /// terminated string.
    const char* what(void) const throw() {return fWhat;}

    /// Used in constructors of classes which inherit ECore to add text to
    /// the What string.  Here is a comprehensive example of how to construct
    /// an exception derived from ECore:
    /// \verbatim
    /// class EoaChild : public ECore {
    /// public: 
    ///   EoaChild() {AppendWhat("EoaChild");}
    /// };
    /// \endverbatim
    void AppendWhat(const char* child);
};

/// A macro to build an exception class __name that is derived from __parent.
/// The __parent class must be derived from ECore which provides the
/// AppendWhat method.  This macro builds an exception class that can be used:
/// \verbatim
/// EXCEPTION(EChild,ECore);
/// ...
/// try {
///   throw EChild();
/// }
/// catch (ECore& ex) {
///   std::cout << ex.what << std::endl;
/// }
/// \endverbatim
#ifndef EXCEPTION
#define EXCEPTION(__name,__parent)           \
    class __name : public __parent {            \
    public:                                     \
    __name() {AppendWhat(#__name);}             \
    }
#endif
#endif
