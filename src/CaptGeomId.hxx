#ifndef ND280GeomId_hxx_seen
#define ND280GeomId_hxx_seen

#include "TGeometryId.hxx"

namespace CP {

    /// Define the geometry identifiers for elements in the ND280 geometry.
    namespace GeomId {

        /// Return the geometry id indication None.
        TGeometryId Empty();

        /// Get the sub-system type.
        int GetSubSystem(TGeometryId id);
        
        /// Define the P0D specific geometry identifiers.
        namespace P0D {
            /// Define a geometry identifier to the entire P0D detector.
            TGeometryId Detector();
        
            /// Check if the id is for the P0D.
            bool IsP0D(TGeometryId id);

            /// The geometry identifiers for the P0D super-P0Dules.  The
            /// super-P0Dules are number 0 to 3 from the upstream end of the
            /// P0D.
            ///   * 0 -- Upstream ECal
            ///   * 1 -- Upstream Water Target
            ///   * 2 -- Central Water Target
            ///   * 3 -- Central ECal.
            TGeometryId SuperP0Dule(int sP0Dule);

            /// If this is a super-P0Dule id, return the super-P0Dule number,
            /// otherwise return -1.
            int GetSuperP0Dule(TGeometryId id);

            /// The geometry identifiers for the P0D scintillator modules.
            /// The P0Dules are number 0 to 39 from the upstream end of the
            /// P0D.
            TGeometryId P0Dule(int p0dule);

            /// If this is a P0Dule id, return the P0Dule number, otherwise
            /// return -1.
            int GetP0Dule(TGeometryId id);

            /// The geometry identifiers for the P0D water targets.  The
            /// targets are numbered from 0 to 24 from the upstream end of the
            /// P0D.
            TGeometryId Target(int target);

            /// If this is a water target, return it's number, otherwise
            /// return -1.
            int GetTarget(TGeometryId id);

            /// The geometry identifiers for the P0D lead radiators.  The lead
            /// radiators are numbered from 0 to 13 from the upstream end of
            /// the P0D.
            TGeometryId ECalRadiator(int radiator);
            
            /// If this is an ECal radiator, return it's number, otherwise
            /// return -1.
            int GetECalRadiator(TGeometryId id);

            /// The geometry identifiers for the P0D brass radiators.  The
            /// brass radiators are numbered from 0 to 24 from the upstream
            /// end of the P0D.
            TGeometryId TargetRadiator(int radiator);
            
            /// If this is a target radiator, return it's number, otherwise
            /// return -1.
            int GetTargetRadiator(TGeometryId id);

            /// The geometry identifiers for p0d scintillators.  The P0Dules
            /// are numbered from 0 to 39 with 0 as the most upstream.  The
            /// layer is 0 (X), or 1 (Y), and the bar is from 0 to 125 (X) or
            /// 133 (Y).  Bar 0 has the most negative coordinate.
            TGeometryId Bar(int p0dule, int layer, int bar);

            /// Get the P0Dule number for an id.  If the provided geometry is
            /// not for a P0D bar, then this returns -1.  The P0Dule numbering
            /// runs from 0 to 39.
            int GetBarP0Dule(TGeometryId id);

            /// Get the P0D Bar Layer number from an id.  If the provided
            /// geometry id is not for a P0D bar, then this returns -1.  The
            /// P0Dule layers are numbered 0 (x) and 1 (y).
            int GetBarLayer(TGeometryId id);

            /// Get the P0D Bar Number from an id.  If the provided geometry
            /// id is not for a P0D bar, then this returns -1.  The Bars are
            /// numbered between 0 and 126 (or 134).  Bar Number zero hs the
            /// tip of the triangle facing upstream.  You can use
            /// \code
            /// bool pointsDownstream = (GetBarNumber(id)%2);
            /// \endcode
            /// to determine which way a bar is facing.
            int GetBarNumber(TGeometryId id);

        };

        /// Define the TPC specific identifiers.
        namespace TPC {
            /// Check to see if the id is for a TPC.
            bool IsTPC(TGeometryId id);

            /// The geometry identifier for the TPC modules.  This count
            /// starts from zero and runs from upstream to downstream.
            TGeometryId Module(int mod);

            /// Get the TPC module number (0 to 2).  This returns  -1 if the id
            /// is invalid.
            int GetModule(TGeometryId id);

            /// The geometry identifier for TPC1.
            TGeometryId TPC1();
            
            /// The geometry identifier for TPC2.
            TGeometryId TPC2();
            
            /// The geometry identifier for TPC3.
            TGeometryId TPC3();
            
            /// @{ Check to see if the id is for a TPCn.
            bool IsTPC1(TGeometryId id);
            bool IsTPC2(TGeometryId id);
            bool IsTPC3(TGeometryId id);
            /// @}

            /// The geometry identifier for the micromegas.  The TPCs are
            /// numbered 0 to 2 from upstream to downstream.  The half is 0
            /// for -x, and 1 for +x.  The micromegas run from 0 to 15;
            TGeometryId MicroMega(int tpc, int  half, int mm);

            /// @{ Get the micromega TPC, half, or number.  This returns  -1 if
            /// the id is invalid.
            int GetMicroMegaTPC(TGeometryId id);
            int GetMicroMegaHalf(TGeometryId id);
            int GetMicroMegaNumber(TGeometryId id);
            /// @}

            /// Check if a TGeometryId is a MicroMega
            bool IsMicroMega(TGeometryId id);

            /// The geometry identifier for the micromega pads.  The TPCs are
            /// numbered 0 to 2 from upstream to downstream.  The half is 0
            /// for -x, and 1 for +x.  The micromegas run from 0 to 15; The
            /// pad runs from 0 to 1727.
            TGeometryId Pad(int tpc, int half, int mm, int pad);

            /// @{ Get the pad TPC, half, micro-mega or number.  This returns 
            /// -1 if the id is invalid.
            int GetPadTPC(TGeometryId id);
            int GetPadHalf(TGeometryId id);
            int GetPadMicroMega(TGeometryId id);
            int GetPadNumber(TGeometryId id);
            /// @}

            /// Check if a TGeometryId is a PAD
            bool IsPad(TGeometryId id);
        };

        /// Define the FGD specific geometry identifiers.
        namespace FGD {
            /// Check if this is an FGD id.
            bool IsFGD(TGeometryId id);

            /// The geometry identifier for the FGD modules.  This count
            /// starts from zero and runs from upstream to downstream.  0 is
            /// the upstream FGD (FGD1).  1 is the downstream FGD (FGD2 -- The
            /// water target FGD).
            TGeometryId FGD(int mod);

            /// Get the FGD number.  This returns  -1 if the id is invalid.
            int GetFGD(TGeometryId id);

            /// The geometry identifier for FGD1.
            TGeometryId FGD1();
            
            /// The geometry identifier for FGD2.  FGD2 has the water targets.
            TGeometryId FGD2();

            /// @{ Check if the id is for FGDn.  This returns  -1 if the id is
            /// invalid.
            bool IsFGD1(TGeometryId id);
            bool IsFGD2(TGeometryId id);
            /// @}

            /// The geometry identifier for the targets.
            TGeometryId Target(int i);

            /// Get the FGD target number.  This returns  -1 if the id is
            /// invalid.
            int GetTarget(TGeometryId id);

            /// A layer within the FGD.  The fgd is either 0 or 1.  The module
            /// is the glued plane of X and Y scintillator bars.  The layer is
            /// 0 (x) or 1 (y).
            TGeometryId Layer(int fgd, int module, int layer);

            /// {@ Get the number of the fgd, module or layer.  This returns -1
            /// if the id is invalid.
            int GetLayerFGD(TGeometryId id);
            int GetLayerModule(TGeometryId id);
            int GetLayerNumber(TGeometryId id);
            /// @}

            /// The geometry identifiers for FGD scintillators.  The FGD
            /// indicates which FGD the bar is in (0: FGD1, 1: FGD2).  The
            /// module gives the glued plane of X and Y bars in the FGD.  The
            /// layer (x or y) indicates which orientation the bars have (0: X,
            /// 1: Y).  The bar is the number of bar in the layer.
            TGeometryId Bar(int fgd, int module, int layer, int bar);

            /// {@ Get the number of the fgd, module, layer, or bar.  This
            /// returns -1 if the id is invalid.
            int GetBarFGD(TGeometryId id);
            int GetBarModule(TGeometryId id);
            int GetBarLayer(TGeometryId id);
            int GetBarNumber(TGeometryId id);
            /// @}
        };

        /// Define the geometry identifiers for a generic (uk) ecal.
        namespace ECal {
            /// The geometry identifier for the tracker ecal modules.  The
            /// ecal must be either CP::GeomId::Def::kDSECal,
            /// CP::GeomId::Def::kPECal, or CP::GeomId::Def::kTECal.  The clam
            /// is 0 for south (-x) or 1 for north (+x).  The module is 0 for
            /// bottom, 1 for side, and 2 for top.
            TGeometryId Module(int ecal, int clam, int module);
            
            /// The geometry identifier for barrel ECal container modules
            /// The ecal must be CP::GeomId::Def::kTECal
            /// Clams and modules are in in Module()
            TGeometryId Container(int ecal, int clam, int module);

            /// The layer within the ecal.
            TGeometryId Layer(int ecal, int clam, int module, int layer);

            /// The geometry identifier for the radiators
            TGeometryId Radiator(int ecal, int clam, int module, int radiator);

            /// The geometry identifiers for the scintillators.
            TGeometryId Bar(int ecal, int clam, 
                            int module, int layer, int bar);

            /// @{ Access the information for an ECal Module.
            int GetModuleECal(TGeometryId id);
            int GetModuleClam(TGeometryId id);
            int GetModuleNumber(TGeometryId id);
            /// @}

            /// @{ Get the layer number.  This returns -1 if the id is
            /// invalid.
            int GetLayerECal(TGeometryId id);
            int GetLayerClam(TGeometryId id);
            int GetLayerModule(TGeometryId id);
            int GetLayerNumber(TGeometryId id);
            /// @} 

            /// @{ Get the radiator number.  This returns -1 if the id is
            /// invalid.
            int GetRadiatorECal(TGeometryId id);
            int GetRadiatorClam(TGeometryId id);
            int GetRadiatorModule(TGeometryId id);
            int GetRadiatorNumber(TGeometryId id);
            /// @} 

            /// @{ Get the layer or bar number.  This returns -1 if the id is
            /// invalid.
            int GetBarECal(TGeometryId id);
            int GetBarClam(TGeometryId id);
            int GetBarModule(TGeometryId id);
            int GetBarLayer(TGeometryId id);
            int GetBarNumber(TGeometryId id);
            /// @}
        };

        /// Define the DSECal specific geometry identifiers.
        namespace DSECal {
            /// Check if the id corresponds to the DSECal.
            bool IsDSECal(TGeometryId id);

            /// The geometry identifier for the downstream ecal detector.
            TGeometryId Detector();
            
            /// The layer within the ecal.
            TGeometryId Layer(int layer);
            
            /// Get the DSECal layer number.  This returns -1 if the id is
            /// invalid.
            int GetLayer(TGeometryId id);

            /// The geometry identifiers for DSECal scintillators.
            TGeometryId Bar(int layer, int bar);

            /// @{ Get the layer or bar number.  This returns -1 if the id is
            /// invalid.
            int GetBarLayer(TGeometryId id);
            int GetBarNumber(TGeometryId id);
            /// @}

            /// The geometry identifier for the radiators
            TGeometryId Radiator(int radiator);

            /// Get the DSECal radiator number.  This returns -1 if the id is
            /// invalid.
            int GetRadiator(TGeometryId id);
        };

        /// Define the TECal specific geometry identifiers.
        namespace TECal {
            /// Check if the id corresponds to the TECal.
            bool IsTECal(TGeometryId id);

            /// The geometry identifier for the tracker ecal modules
            TGeometryId Module(int clam, int module);

            /// The layer within the ecal.
            TGeometryId Layer(int clam, int module, int layer);

            /// The geometry identifier for the radiators
            TGeometryId Radiator(int clam, int module, int radiator);

            /// The geometry identifiers for the scintillators.
            TGeometryId Bar(int clam, int module, int layer, int bar);

            /// @{ Access the information for an ECal Module.
            int GetModuleClam(TGeometryId id);
            int GetModuleNumber(TGeometryId id);
            /// @}

            /// @{ Get the layer number.  This returns -1 if the id is
            /// invalid.
            int GetLayerClam(TGeometryId id);
            int GetLayerModule(TGeometryId id);
            int GetLayerNumber(TGeometryId id);
            /// @} 

            /// @{ Get the radiator number.  This returns -1 if the id is
            /// invalid.
            int GetRadiatorClam(TGeometryId id);
            int GetRadiatorModule(TGeometryId id);
            int GetRadiatorNumber(TGeometryId id);
            /// @} 

            /// @{ Get the layer or bar number.  This returns -1 if the id is
            /// invalid.
            int GetBarClam(TGeometryId id);
            int GetBarModule(TGeometryId id);
            int GetBarLayer(TGeometryId id);
            int GetBarNumber(TGeometryId id);
            /// @}
        };

        /// Define the PECal specific geometry identifiers.
        namespace PECal {
            /// Check if the id corresponds to the PECal.
            bool IsPECal(TGeometryId id);

            /// The geometry identifier for the tracker ecal modules
            TGeometryId Module(int clam, int module);

            /// The layer within the ecal.
            TGeometryId Layer(int clam, int module, int layer);

            /// The geometry identifiers for the scintillators.
            TGeometryId Bar(int clam, int module, int layer, int bar);

            /// The geometry identifier for the radiators
            TGeometryId Radiator(int clam, int module, int radiator);

            /// @{ Access the information for an ECal Module.
            int GetModuleClam(TGeometryId id);
            int GetModuleNumber(TGeometryId id);
            /// @}

            /// @{ Get the layer number.  This returns -1 if the id is
            /// invalid.
            int GetLayerClam(TGeometryId id);
            int GetLayerModule(TGeometryId id);
            int GetLayerNumber(TGeometryId id);
            /// @} 

            /// @{ Get the radiator number.  This returns -1 if the id is
            /// invalid.
            int GetRadiatorClam(TGeometryId id);
            int GetRadiatorModule(TGeometryId id);
            int GetRadiatorNumber(TGeometryId id);
            /// @} 

            /// @{ Get the layer or bar number.  This returns -1 if the id is
            /// invalid.
            int GetBarClam(TGeometryId id);
            int GetBarModule(TGeometryId id);
            int GetBarLayer(TGeometryId id);
            int GetBarNumber(TGeometryId id);
            /// @}
        };

        /// Define the SMRD specific geometry identifiers.
        namespace SMRD {
            /// Test if this id is for the SMRD.
            bool IsSMRD(TGeometryId id);

            /// The geometry identifier for a SMRD module.
            TGeometryId Module(int clam, int yoke, int layer, int slot);
            
            /// The geometry identifer for a SMRD bar.
            TGeometryId Bar(int clam, int yoke, int layer, int slot, int bar);

            /// @{ Access the information for a Module.  This returns -1 if
            /// the id is invalid.
            int GetModuleClam(TGeometryId id);
            int GetModuleYoke(TGeometryId id);
            int GetModuleLayer(TGeometryId id);
            int GetModuleSlot(TGeometryId id);
            /// @}

            /// @{ Get the bar information.  This returns -1 if the id is
            /// invalid.
            int GetBarClam(TGeometryId id);
            int GetBarYoke(TGeometryId id);
            int GetBarLayer(TGeometryId id);
            int GetBarSlot(TGeometryId id);
            int GetBarNumber(TGeometryId id);
            /// @}
        };

        /// Define the INGRID specific geometry identifiers.
        namespace INGRID {
            /// The geometry of a generical scintillator.  The object type is
            /// either kIngridModule = 0, or kIngridVeto = 1.
            TGeometryId Scintillator(int objType, int obj, int trk, 
                                     int proj, int scinti);

            /// The geometry of a module scintillator.
            TGeometryId ModScintillator(int mod, int trk,
                                        int proj, int scinti);

            /// The geometry of a vertical veto scintillator.
            TGeometryId VertVetoScintillator(int veto, int scinti);

            /// The geometry of an horizontal veto scintillator.
            TGeometryId HorzVetoScintillator(int veto, int scinti);
        };
    };
};
#endif
