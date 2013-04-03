#include <iostream>
#include <tut.h>
#include <cmath>

#include "TCorrValues.hxx"

namespace tut {
    struct baseTCorrValues {
        baseTCorrValues() {
            // Run before each test.
        }
        ~baseTCorrValues() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTCorrValues>::object testTCorrValues;
    test_group<baseTCorrValues> groupTCorrValues("TCorrValues");

    // Test the default constructor and destructor.
    template<> template<>
    void testTCorrValues::test<1> () {
        CP::TCorrValues v;
        ensure_equals("Correlated values dimension", 0, v.GetDimensions());
    }

    // Test the constructor with a dimension.
    template<> template<>
    void testTCorrValues::test<2> () {
        CP::TCorrValues v(2);
        CP::TCorrValues w(20);
        ensure_equals("Correlated values dimension", 2, v.GetDimensions());
        ensure_equals("Correlated values dimension", 20, w.GetDimensions());
        ensure_equals("Correlated values vector dimension",w.GetDimensions(), 
                      w.GetValues().GetNoElements());
        ensure_equals("Correlated values matrix row dimension",w.GetDimensions(), 
                      w.GetCovariance().GetNrows());
        ensure_equals("Correlated values matrix column dimension",w.GetDimensions(), 
                      w.GetCovariance().GetNcols());
    }

    // Test the copy constructor.
    template<> template<>
    void testTCorrValues::test<3> () {
        CP::TCorrValues w(20);
        CP::TCorrValues x(w);
        CP::TCorrValues y;
        y = w;
        ensure_equals("Correlated values dimension", 20, w.GetDimensions());
        ensure_equals("Correlated values copied dimension", x.GetDimensions(), 
                      w.GetDimensions());
        ensure_equals("Correlated values assigned dimension", y.GetDimensions(),
                      w.GetDimensions());
    }

    // Test the vector constructor.
    template<> template<>
    void testTCorrValues::test<4> () {
        TVectorD vec(10);
        CP::TCorrValues v(vec);
        ensure_equals("Vector dimension", 10, vec.GetNoElements());
        ensure_equals("Correlated values dimension", vec.GetNoElements(),
                      v.GetDimensions());
        ensure_equals("Correlated values vector dimension",v.GetDimensions(), 
                      v.GetValues().GetNoElements());
        ensure_equals("Correlated values matrix row dimension",v.GetDimensions(), 
                      v.GetCovariance().GetNrows());
        ensure_equals("Correlated values matrix column dimension",v.GetDimensions(), 
                      v.GetCovariance().GetNcols());
    }

    // Test the construction of a hash value.
    template<> template<>
    void testTCorrValues::test<5> () {
        CP::TCorrValues v(3);
        CP::TCorrValues w(3);
        const char* val = "A type value";

        ensure_equals("Default type hash value", v.GetTypeHash(), (unsigned)0);
        v.SetType(val);
        ensure("Specified type hash is not equal to zero", 
               v.GetTypeHash() != 0);
        w.SetType(val);
        ensure_equals("Type hash is the same.",
                      v.GetTypeHash(), w.GetTypeHash());
    }

    // Test an invalid covariance matrix
    template<> template<>
    void testTCorrValues::test<6> () {
        CP::TCorrValues v(4);

        TMatrixTSym<float> hess(v.GetDimensions());

        hess = v.GetHessian();
        for (int row = 0; row < hess.GetNrows(); ++row) {
            for (int col = 0; col < hess.GetNcols(); ++col) {
                ensure_distance("Hessian is zero for zero covariance",
                                hess(row,col), 
                                (CP::TCorrValues::Element) 0.0, 
                                (CP::TCorrValues::Element) 0.00001);
            }
        }

        v.SetCovarianceValue(0,0,1.0);
        v.SetCovarianceValue(1,1,1.0);
        v.SetCovarianceValue(2,2,1.0);
        v.SetCovarianceValue(3,3,1.0);
        hess = v.GetHessian();
        for (int row = 0; row < hess.GetNrows(); ++row) {
            for (int col = 0; col < hess.GetNcols(); ++col) {
                if (row == col) {
                    ensure_distance("Hessian is 1 for 1 covariance",
                                    hess(row,col), 
                                    (CP::TCorrValues::Element) 1.0, 
                                    (CP::TCorrValues::Element) 0.00001);
                    continue;
                }
                ensure_distance("Hessian is zero for zero covariance",
                                hess(row,col), 
                                (CP::TCorrValues::Element) 0.0, 
                                std::numeric_limits<CP::TCorrValues::Element>::epsilon());
            }
        }
    }
    
    // Test the default covariance matrix has all parameters free.  Also check
    // that SetFree and SetFixed work properly.
    template<> template<>
    void testTCorrValues::test<7> () {
        CP::TCorrValues v(3);

        for (int i=0; i<v.GetDimensions(); ++i) {
            ensure("Parameter is free", v.IsFree(i));
        }
    }

    // Check that SetFree and SetFixed work properly.
    template<> template<>
    void testTCorrValues::test<8> () {
        CP::TCorrValues v(3);
        v.SetFixed(1);
        ensure("Parameter 0 is free", v.IsFree(0));
        ensure("Parameter 1 is fixed", v.IsFixed(1));
        ensure("Parameter 2 is free", v.IsFree(2));

        v.SetFixed(0);
        v.SetFree(1);
        v.SetFixed(2);
        ensure("Parameter 0 is free", v.IsFixed(0));
        ensure("Parameter 1 is free", v.IsFree(1));
        ensure("Parameter 2 is free", v.IsFixed(2));
    }

    /// Check that the Hessian*Covariance is the identity when all covariance
    /// parameters are free.
    template<> template<>
    void testTCorrValues::test<9> () {
        CP::TCorrValues v(3);

        v.SetFree(0);
        v.SetFixed(1);
        v.SetFree(2);

        TMatrixTSym<float> h(v.GetDimensions());
        TMatrixTSym<float> c(v.GetDimensions());

        TMatrixD u(v.GetDimensions(),v.GetDimensions());
        u = v.GetHessian()*v.GetCovariance();

        for (int i=0; i<v.GetDimensions(); ++i) {
            for (int j=0; j<v.GetDimensions(); ++j) {
                if (i==j) {
                    ensure_distance("Identity diagonals are 1", 
                                    u(i,j), 1.0, 0.00001);
                    continue;
                }
                ensure_distance("Identiy off-diagonals are zero",
                                u(i,j),0.0,0.00001);
            }
        }
    }

    /// Test the covariance value getters and setters.
    template<> template<>
    void testTCorrValues::test<10> () {
        CP::TCorrValues v(3);
        
        v.SetCovarianceValue(0,0,1.0);
        v.SetCovarianceValue(1,1,2.0);
        v.SetCovarianceValue(2,2,3.0);

        ensure_distance("TCorrValues variance",
                        v.GetCovarianceValue(0,0), 1.0, 0.0000001);
        ensure_distance("TCorrValues variance",
                        v.GetCovarianceValue(1,1), 2.0, 0.0000001);
        ensure_distance("TCorrValues variance",
                        v.GetCovarianceValue(2,2), 3.0, 0.0000001);

    }


    /// Test the one-dimensional constructor.
    template<> template<>
    void testTCorrValues::test<11> () {
        CP::TCorrValues v(3.0,2.0);
        
        ensure_distance("TCorrValues value",
                        v.GetValue(), 3.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        v.GetUncertainty(), 2.0, 0.0000001);
        ensure_distance("TCorrValues variance",
                        v.GetCovarianceValue(0,0), 4.0, 0.0000001);
    }

    /// Test the addition of one dimensional correlated values.
    template<> template<>
    void testTCorrValues::test<12> () {
        CP::TCorrValues u(3.0,2.0);
        CP::TCorrValues v(4.0,3.0);
        
        CP::TCorrValues x(1);
        x = u + v;
        ensure_distance("TCorrValues value",
                        x.GetValue(), 7.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        x.GetUncertainty(), std::sqrt(2*2+3*3), 0.0000001);

        CP::TCorrValues y = u + v;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 7.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), std::sqrt(2*2+3*3), 0.0000001);

        y = u - v;
        ensure_distance("TCorrValues value",
                        y.GetValue(), -1.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), std::sqrt(2*2+3*3), 0.0000001);

        y = u + 1.0;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 4.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);

        y = 1.0 + u;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 4.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);

        y = u - 1.0;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 2.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);

        y = 1.0 - u;
        ensure_distance("TCorrValues value",
                        y.GetValue(), -2.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);
    }

    /// Test the addition of one dimensional correlated values.
    template<> template<>
    void testTCorrValues::test<13> () {
        CP::TCorrValues u(2);
        CP::TCorrValues v(2);

        u.SetValue(0,3.0);
        u.SetCovarianceValue(0,0,4.0);
        u.SetValue(1,3.0);
        u.SetCovarianceValue(1,1,4.0);

        v.SetValue(0,4.0);
        v.SetCovarianceValue(0,0,9.0);
        v.SetValue(1,4.0);
        v.SetCovarianceValue(1,1,9.0);

        CP::TCorrValues y = u + v;

        for (int i=0; i<2; ++i) {
            ensure_distance("TCorrValues value",
                            y.GetValue(i), 7.0, 0.0000001);
            ensure_distance("TCorrValues uncertainty",
                            y.GetUncertainty(i), std::sqrt(2*2+3*3), 0.0000001);
        }

        y = u - v;
        for (int i=0; i<2; ++i) {
            ensure_distance("TCorrValues value",
                            y.GetValue(i), -1.0, 0.0000001);
            ensure_distance("TCorrValues uncertainty",
                            y.GetUncertainty(i), std::sqrt(2*2+3*3), 0.0000001);
        }

        y = u + 1.0;
        for (int i=0; i<2; ++i) {
            ensure_distance("TCorrValues value",
                            y.GetValue(i), 4.0, 0.0000001);
            ensure_distance("TCorrValues uncertainty",
                            y.GetUncertainty(i), 2.0, 0.0000001);
        }

        y = 1.0 + u;
        for (int i=0; i<2; ++i) {
            ensure_distance("TCorrValues value",
                            y.GetValue(i), 4.0, 0.0000001);
            ensure_distance("TCorrValues uncertainty",
                            y.GetUncertainty(i), 2.0, 0.0000001);
        }

        y = u - 1.0;
        for (int i=0; i<2; ++i) {
            ensure_distance("TCorrValues value",
                            y.GetValue(i), 2.0, 0.0000001);
            ensure_distance("TCorrValues uncertainty",
                            y.GetUncertainty(i), 2.0, 0.0000001);
        }
        
        y = 1.0 - u;
        for (int i=0; i<2; ++i) {
            ensure_distance("TCorrValues value",
                            y.GetValue(i), -2.0, 0.0000001);
            ensure_distance("TCorrValues uncertainty",
                            y.GetUncertainty(i), 2.0, 0.0000001);
        }
    }

    /// Test the addition of one dimensional correlated values.
    template<> template<>
    void testTCorrValues::test<14> () {
        CP::TCorrValues u(3.0,2.0);
        CP::TCorrValues v(4.0,3.0);
        
        CP::TCorrValues x(1);
        x = u + v;
        ensure_distance("TCorrValues value",
                        x.GetValue(), 7.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        x.GetUncertainty(), std::sqrt(2*2+3*3), 0.0000001);

        CP::TCorrValues y = u + v;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 7.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), std::sqrt(2*2+3*3), 0.0000001);

        y = u - v;
        ensure_distance("TCorrValues value",
                        y.GetValue(), -1.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), std::sqrt(2*2+3*3), 0.0000001);

        y = u + 1.0;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 4.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);

        y = 1.0 + u;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 4.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);

        y = u - 1.0;
        ensure_distance("TCorrValues value",
                        y.GetValue(), 2.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);

        y = 1.0 - u;
        ensure_distance("TCorrValues value",
                        y.GetValue(), -2.0, 0.0000001);
        ensure_distance("TCorrValues uncertainty",
                        y.GetUncertainty(), 2.0, 0.0000001);
    }


};
