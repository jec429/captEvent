#ifndef TPackageSet_hxx_seen
#define TPackageSet_hxx_seen
/////////////////////////////////////////////////////////////////////
// $Id: TPackageSet.hxx,v 1.1 2008/07/23 22:10:10 mcgrew Exp $
// 
// Define a container for the package versions used in an executable.

#include "TPackageVersion.hxx"
#include <set>

namespace CP {
    /// The set of package versions used in the currently running executable.
    typedef std::set < TPackageVersion * > TPackageSet;
}

#endif
