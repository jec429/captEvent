#include <iostream>
#include <tut.h>

#include "TPackageSet.hxx"
#include "TPackageVersion.hxx"
#include "TManager.hxx"

namespace tut {
    struct basePackageVersion {
        basePackageVersion() {
            // Run before each test.
        }
        ~basePackageVersion() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<basePackageVersion>::object testPackageVersion;
    test_group<basePackageVersion> groupPackageVersion("PackageVersion");

    // Test the default constructor and destructor.
    template<> template<>
    void testPackageVersion::test<1> () {
        CP::TManager::Get().PackageSet();
    }
};
