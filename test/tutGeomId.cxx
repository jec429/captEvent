#include <iostream>
#include <map>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#include "TGeometryId.hxx"
#include "ND280GeomId.hxx"
#include "ND280GeomIdDef.hxx"

namespace tut {
    struct baseGeomId {
        baseGeomId() {
            // Run before each test.
        }
        ~baseGeomId() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseGeomId>::object testGeomId;
    test_group<baseGeomId> groupGeomId("GeomId");

    // Test P0D geometry identifiers.
    template<> template<>
    void testGeomId::test<1> () {
        ND::TGeometryId gid = ND::GeomId::P0D::Detector();
        
        ensure("P0D Detector IsP0D", ND::GeomId::P0D::IsP0D(gid));

        for (int i=0; i<4; ++i) {
            gid = ND::GeomId::P0D::SuperP0Dule(i);
            ensure("SuperP0Dule IsP0D", ND::GeomId::P0D::IsP0D(gid));
            ensure_equals("SuperP0Dule matches id",
                          ND::GeomId::P0D::GetSuperP0Dule(gid), i);
        }

        for (int i=0; i<40; ++i) {
            gid = ND::GeomId::P0D::P0Dule(i);
            ensure("P0Dule IsP0D", ND::GeomId::P0D::IsP0D(gid));
            ensure_equals("P0Dule matches id",
                          ND::GeomId::P0D::GetP0Dule(gid), i);
        }

        for (int i=0; i<25; ++i) {
            gid = ND::GeomId::P0D::Target(i);
            ensure("Target IsP0D", ND::GeomId::P0D::IsP0D(gid));
            ensure_equals("Target matches id",
                          ND::GeomId::P0D::GetTarget(gid), i);
        }

        for (int i=0; i<25; ++i) {
            gid = ND::GeomId::P0D::ECalRadiator(i);
            ensure("ECalRadiator IsP0D", ND::GeomId::P0D::IsP0D(gid));
            ensure_equals("ECalRadiator matches id",
                          ND::GeomId::P0D::GetECalRadiator(gid), i);
        }

        for (int i=0; i<25; ++i) {
            gid = ND::GeomId::P0D::TargetRadiator(i);
            ensure("TargetRadiator IsP0D", ND::GeomId::P0D::IsP0D(gid));
            ensure_equals("TargetRadiator matches id",
                          ND::GeomId::P0D::GetTargetRadiator(gid), i);
        }

        for (int p=0; p<40; ++p) {
            for (int xy = 0; xy < 2; ++xy) {
                for (int b=0; b<134; ++b) {
                    gid = ND::GeomId::P0D::Bar(p,xy,b);
                    ensure("Bar IsP0D", ND::GeomId::P0D::IsP0D(gid));
                    ensure_equals("Bar P0Dule matches id",
                                  ND::GeomId::P0D::GetBarP0Dule(gid), p);
                    ensure_equals("Bar Layer matches id",
                                  ND::GeomId::P0D::GetBarLayer(gid), xy);
                    ensure_equals("Bar Number matches id",
                                  ND::GeomId::P0D::GetBarNumber(gid), b);
                }
            }
        }
    }
    
    // Test TPC geometry identifiers.
    template<> template<>
    void testGeomId::test<2> () {
        ND::TGeometryId gid;

        for (int i = 0; i<3; ++i) {
            gid = ND::GeomId::TPC::Module(i);
            ensure("Module IsTPC", ND::GeomId::TPC::IsTPC(gid));
            ensure_equals("Module matchs id",
                          ND::GeomId::TPC::GetModule(gid), i);
        }            

        for (int t=0; t<3; ++t) {
            for (int h = 0; h<2; ++h) {
                for (int m = 0; m<12; ++m) {
                    gid = ND::GeomId::TPC::MicroMega(t,h,m);
                    ensure("MM IsTPC", ND::GeomId::TPC::IsTPC(gid));
                    ensure_equals("MM TPC matchs id",
                                  ND::GeomId::TPC::GetMicroMegaTPC(gid), t);
                    ensure_equals("MM half matchs id",
                                  ND::GeomId::TPC::GetMicroMegaHalf(gid), h);
                    ensure_equals("MM Number matchs id",
                                  ND::GeomId::TPC::GetMicroMegaNumber(gid), m);
                }
            }
        }

        for (int t=0; t<3; ++t) {
            for (int h = 0; h<2; ++h) {
                for (int m = 0; m<12; ++m) {
                    for (int p=0; p<1728; ++p) {
                        gid = ND::GeomId::TPC::Pad(t,h,m,p);
                        ensure("Pad IsTPC", ND::GeomId::TPC::IsTPC(gid));
                        ensure_equals("Pad TPC matchs id",
                                      ND::GeomId::TPC::GetPadTPC(gid), t);
                        ensure_equals("Pad half matchs id",
                                      ND::GeomId::TPC::GetPadHalf(gid), h);
                        ensure_equals("Pad MM matchs id",
                                      ND::GeomId::TPC::GetPadMicroMega(gid),m);
                        ensure_equals("Pad Number matchs id",
                                      ND::GeomId::TPC::GetPadNumber(gid), p);
                    }
                }
            }
        }
    }
    
    // Test FGD geometry identifiers.
    template<> template<>
    void testGeomId::test<3> () {
        ND::TGeometryId gid;

        for (int i = 0; i<2; ++i) {
            gid = ND::GeomId::FGD::FGD(i);
            ensure("Module IsFGD", ND::GeomId::FGD::IsFGD(gid));
            ensure_equals("Module matchs id",
                          ND::GeomId::FGD::GetFGD(gid), i);
        }            

        for (int i=0; i<7; ++i) {
            gid = ND::GeomId::FGD::Target(i);
            ensure("Target IsFGD", ND::GeomId::FGD::IsFGD(gid));
            ensure_equals("Target matches id",
                          ND::GeomId::FGD::GetTarget(gid), i);
        }

        for (int f=0; f<2; ++f) {
            for (int m = 0; m < 14; ++m) {
                for (int xy=0; xy<2; ++xy) {
                    gid = ND::GeomId::FGD::Layer(f,m,xy);
                    ensure("Layer IsFGD", ND::GeomId::FGD::IsFGD(gid));
                    ensure_equals("Layer FGD matches id",
                                  ND::GeomId::FGD::GetLayerFGD(gid), f);
                    ensure_equals("Layer Module matches id",
                                  ND::GeomId::FGD::GetLayerModule(gid), m);
                    ensure_equals("Layer Number matches id",
                                  ND::GeomId::FGD::GetLayerNumber(gid), xy);
                }
            }
        }

        for (int f=0; f<2; ++f) {
            for (int m = 0; m < 14; ++m) {
                for (int xy=0; xy<2; ++xy) {
                    for (int b=0; b<100; ++b) {
                        gid = ND::GeomId::FGD::Bar(f,m,xy,b);
                        ensure("Bar IsFGD", ND::GeomId::FGD::IsFGD(gid));
                        ensure_equals("Bar FGD matches id",
                                      ND::GeomId::FGD::GetBarFGD(gid), f);
                        ensure_equals("Bar Module matches id",
                                      ND::GeomId::FGD::GetBarModule(gid), m);
                        ensure_equals("Bar Layer matches id",
                                      ND::GeomId::FGD::GetBarLayer(gid), xy);
                        ensure_equals("Bar Number matches id",
                                      ND::GeomId::FGD::GetBarNumber(gid), b);
                    }
                }
            }
        }
    }

    // Test DSECal geometry identifiers.
    template<> template<>
    void testGeomId::test<4> () {
        ND::TGeometryId gid = ND::GeomId::DSECal::Detector();
        
        ensure("IsDSECal", ND::GeomId::DSECal::IsDSECal(gid));
        
        for (int i=0; i<64; ++i) {
            gid = ND::GeomId::DSECal::Layer(i);
            ensure("Layer IsDSECal", ND::GeomId::DSECal::IsDSECal(gid));
            ensure_equals("Layer matches id",
                          ND::GeomId::DSECal::GetLayer(gid), i);
        }

        for (int i=0; i<64; ++i) {
            gid = ND::GeomId::DSECal::Radiator(i);
            ensure("Radiator IsDSECal", ND::GeomId::DSECal::IsDSECal(gid));
            ensure_equals("Radiator matches id",
                          ND::GeomId::DSECal::GetRadiator(gid), i);
        }

        for (int i=0; i<64; ++i) {
            for (int b=0; b<64; ++b) {
                gid = ND::GeomId::DSECal::Bar(i,b);
                ensure("Layer IsDSECal", ND::GeomId::DSECal::IsDSECal(gid));
                ensure_equals("DSECal Bar Layer matches id",
                              ND::GeomId::DSECal::GetBarLayer(gid), i);
                ensure_equals("DSECal Bar Number matches id",
                              ND::GeomId::DSECal::GetBarNumber(gid), b);
            }
        }

    }

    // Test TECal geometry identifiers.
    template<> template<>
    void testGeomId::test<5> () {
        ND::TGeometryId gid;
        
        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                gid = ND::GeomId::TECal::Module(c,m);
                ensure("IsTECal", ND::GeomId::TECal::IsTECal(gid));
            }
        }

        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                for (int i=0; i<64; ++i) {
                    gid = ND::GeomId::TECal::Layer(c,m,i);
                    ensure("Layer IsTECal", ND::GeomId::TECal::IsTECal(gid));
                    ensure_equals("TECal Layer Clam matches id",
                                  ND::GeomId::TECal::GetLayerClam(gid), c);
                    ensure_equals("TECal Layer Module matches id",
                                  ND::GeomId::TECal::GetLayerModule(gid), m);
                    ensure_equals("TECal Layer matches id",
                                  ND::GeomId::TECal::GetLayerNumber(gid), i);
                }
            }
        }

        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                for (int i=0; i<64; ++i) {
                    gid = ND::GeomId::TECal::Radiator(c,m,i);
                    ensure("Radiator IsTECal", ND::GeomId::TECal::IsTECal(gid));
                    ensure_equals("TECal Radiator Clam matches id",
                                  ND::GeomId::TECal::GetRadiatorClam(gid), c);
                    ensure_equals("TECal Radiator Module matches id",
                                  ND::GeomId::TECal::GetRadiatorModule(gid), m);
                    ensure_equals("TECal Radiator matches id",
                                  ND::GeomId::TECal::GetRadiatorNumber(gid), i);
                }
            }
        }

        
        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                for (int i=0; i<64; ++i) {
                    for (int b=0; b<64; ++b) {
                        gid = ND::GeomId::TECal::Bar(c,m,i,b);
                        ensure("Layer IsTECal",
                               ND::GeomId::TECal::IsTECal(gid));
                        ensure_equals("TECal Bar Clam matches id",
                                      ND::GeomId::TECal::GetBarClam(gid), c);
                        ensure_equals("TECal Bar Module matches id",
                                      ND::GeomId::TECal::GetBarModule(gid), m);
                        ensure_equals("TECal Bar Layer matches id",
                                      ND::GeomId::TECal::GetBarLayer(gid), i);
                        ensure_equals("TECal Bar Number matches id",
                                      ND::GeomId::TECal::GetBarNumber(gid), b);
                    }
                }
            }
        }
    }

    // Test PECal geometry identifiers.
    template<> template<>
    void testGeomId::test<6> () {
        ND::TGeometryId gid;
        
        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                gid = ND::GeomId::PECal::Module(c,m);
                ensure("IsPECal", ND::GeomId::PECal::IsPECal(gid));
            }
        }

        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                for (int i=0; i<64; ++i) {
                    gid = ND::GeomId::PECal::Layer(c,m,i);
                    ensure("Layer IsPECal", ND::GeomId::PECal::IsPECal(gid));
                    ensure_equals("PECal Layer Clam matches id",
                                  ND::GeomId::PECal::GetLayerClam(gid), c);
                    ensure_equals("PECal Layer Module matches id",
                                  ND::GeomId::PECal::GetLayerModule(gid), m);
                    ensure_equals("PECal Layer matches id",
                                  ND::GeomId::PECal::GetLayerNumber(gid), i);
                }
            }
        }

        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                for (int i=0; i<64; ++i) {
                    gid = ND::GeomId::PECal::Radiator(c,m,i);
                    ensure("Radiator IsPECal", ND::GeomId::PECal::IsPECal(gid));
                    ensure_equals("PECal Radiator Clam matches id",
                                  ND::GeomId::PECal::GetRadiatorClam(gid), c);
                    ensure_equals("PECal Radiator Module matches id",
                                  ND::GeomId::PECal::GetRadiatorModule(gid), m);
                    ensure_equals("PECal Radiator matches id",
                                  ND::GeomId::PECal::GetRadiatorNumber(gid), i);
                }
            }
        }

        
        for (int c = 0; c<2; ++c) {
            for (int m = 0; m<3; ++m) {
                for (int i=0; i<64; ++i) {
                    for (int b=0; b<64; ++b) {
                        gid = ND::GeomId::PECal::Bar(c,m,i,b);
                        ensure("Layer IsPECal",
                               ND::GeomId::PECal::IsPECal(gid));
                        ensure_equals("PECal Bar Clam matches id",
                                      ND::GeomId::PECal::GetBarClam(gid), c);
                        ensure_equals("PECal Bar Module matches id",
                                      ND::GeomId::PECal::GetBarModule(gid), m);
                        ensure_equals("PECal Bar Layer matches id",
                                      ND::GeomId::PECal::GetBarLayer(gid), i);
                        ensure_equals("PECal Bar Number matches id",
                                      ND::GeomId::PECal::GetBarNumber(gid), b);
                    }
                }
            }
        }
    }

    // Test SMRD geometry identifiers.
    template<> template<>
    void testGeomId::test<7> () {
        ND::TGeometryId gid;

        for (int c = 0; c<2; ++c) {
            for (int y=0; y<8; ++y) {
                for (int ll = 0; ll < 16; ++ll) {
                    for (int s = 0; s<8; ++s) {
                        gid = ND::GeomId::SMRD::Module(c,y,ll,s);
                        ensure("Module IsSMRD",
                               ND::GeomId::SMRD::IsSMRD(gid));
                        ensure_equals("SMRD Module Clam matches id",
                                      ND::GeomId::SMRD::GetModuleClam(gid), c);
                        ensure_equals("SMRD Module Yoke matches id",
                                      ND::GeomId::SMRD::GetModuleYoke(gid), y);
                        ensure_equals("SMRD Module Layer matches id",
                                      ND::GeomId::SMRD::GetModuleLayer(gid),ll);
                        ensure_equals("SMRD Module Slot matches id",
                                      ND::GeomId::SMRD::GetModuleSlot(gid), s);
                    }
                }
            }
        }

        for (int c = 0; c<2; ++c) {
            for (int y=0; y<8; ++y) {
                for (int ll = 0; ll < 16; ++ll) {
                    for (int s = 0; s<8; ++s) {
                        for (int b = 0; b<4; ++b) {
                            gid = ND::GeomId::SMRD::Bar(c,y,ll,s,b);
                            ensure("Bar IsSMRD",
                                   ND::GeomId::SMRD::IsSMRD(gid));
                            ensure_equals("SMRD Bar Clam matches id",
                                          ND::GeomId::SMRD::GetBarClam(gid), c);
                            ensure_equals("SMRD Bar Yoke matches id",
                                          ND::GeomId::SMRD::GetBarYoke(gid), y);
                            ensure_equals("SMRD Bar Layer matches id",
                                          ND::GeomId::SMRD::GetBarLayer(gid),
                                          ll);
                            ensure_equals("SMRD Bar Slot matches id",
                                          ND::GeomId::SMRD::GetBarSlot(gid), s);
                            ensure_equals("SMRD Bar Number matches id",
                                          ND::GeomId::SMRD
                                          ::GetBarNumber(gid),b);
                        }
                    }
                }
            }
        }
    }
};

