#include <iostream>
#include <string>

#include <tut.h>

#include "HEPUnits.hxx"
#include "TTPCPadManager.hxx"
#include "TOADatabase.hxx"

namespace tut {

    struct baseTTPCPadManager {
        baseTTPCPadManager() {
            // Run before each test.
        }
        ~baseTTPCPadManager() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTTPCPadManager>::object testTTPCPadManager;
    test_group<baseTTPCPadManager> groupTTPCPadManager("TTPCPadManager");

    // Test the default constructor and destructor.
    template<> template<>
    void testTTPCPadManager::test<1> () {
        CP::TTPCPadManager& pads = CP::TOADatabase::Get().TPCPads();
        std::string name = pads.GetName();
        ensure_equals("TPC pad manager name is correct", 
                      name, "ND280TPCPadManager");
    }

    // Test the pad to row/column translation.
    template<> template<>
    void testTTPCPadManager::test<2> () {
        CP::TTPCPadManager& pads = CP::TOADatabase::Get().TPCPads();
        for (int pad = 0; pad<pads.GetPadCount(); ++pad) {
            int row = pads.PadToRow(pad);
            int column = pads.PadToColumn(pad);
            int p = pads.RowAndColumnToPad(row, column);
            ensure_greaterthan("Valid pad number", p, -1);
            ensure_lessthan("Valid pad number", p, pads.GetPadCount());
            ensure_equals("Pad number matches row and column", p, pad);
        }
    }

    // Test the default local XY max and min.
    template<> template<>
    void testTTPCPadManager::test<3> () {
        CP::TTPCPadManager& pads = CP::TOADatabase::Get().TPCPads();
        ensure_distance("Default local X maximum and minimum are equal",
                        -pads.GetLocalXMin(), 
                        pads.GetLocalXMax(), 
                        0.1*unit::mm);
        ensure_distance("Default local Y maximum and minimum are equal",
                        -pads.GetLocalYMin(), 
                        pads.GetLocalYMax(), 
                        0.1*unit::mm);
    }

    // Test the pad to local X and local Y translation.
    template<> template<>
    void testTTPCPadManager::test<4> () {
        CP::TTPCPadManager& pads = CP::TOADatabase::Get().TPCPads();
        for (int pad = 0; pad<pads.GetPadCount(); ++pad) {
            double localX = pads.PadToLocalX(pad);
            double localY = pads.PadToLocalY(pad);
            int row = pads.LocalYToRow(localY);
            int column = pads.LocalXToColumn(localX);
            int p = pads.RowAndColumnToPad(row, column);
            ensure_lessthan("Valid local X", localX, pads.GetLocalXMax()+0.1);
            ensure_lessthan("Valid local Y", localY, pads.GetLocalYMax()+0.1);
            ensure_greaterthan("Valid local X", 
                               localX, pads.GetLocalXMin()-0.1);
            ensure_greaterthan("Valid local Y",
                               localY, pads.GetLocalYMin()-0.1);
            ensure_greaterthan("Valid pad number", p, -1);
            ensure_lessthan("Valid pad number", p, pads.GetPadCount());
            ensure_equals("Pad number matches row and column", p, pad);
        }
    }
};


