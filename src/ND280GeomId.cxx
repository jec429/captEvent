#include "TND280Log.hxx"
#include "TGeometryId.hxx"
#include "ND280GeomId.hxx"
#include "ND280GeomIdDef.hxx"

namespace {
    // Provide access to the protected setter and getter methods.
    class TSettableGeometryId: public ND::TGeometryId {
    public:
        TSettableGeometryId() {}
        explicit TSettableGeometryId(const ND::TGeometryId& id)
            : ND::TGeometryId(id) {}
        void SetField(int v, int m, int b) {
            ND::TGeometryId::SetFieldSafe(v,m,b);
        }
        int GetField(int m, int b) {
            return ND::TGeometryId::GetFieldSafe(m,b);
        }
    };
}

ND::TGeometryId ND::GeomId::Empty() {
    return TGeometryId();
}

int ND::GeomId::GetSubSystem(TGeometryId i) {
    TSettableGeometryId id(i);
    return id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB);
}

////////////////////////////////////////////////////////////
// P0D Geometry Identifiers.
////////////////////////////////////////////////////////////

bool ND::GeomId::P0D::IsP0D(TGeometryId i) {
    TSettableGeometryId id(i);
    return (ND::GeomId::Def::kP0D 
            == id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                           ND::GeomId::Def::kDetectorIdLSB));
}

ND::TGeometryId ND::GeomId::P0D::Detector() {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kP0D,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::P0D::kGlobal,
                ND::GeomId::Def::P0D::kSeqIdMSB,
                ND::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::P0D::Global::kP0D,
                ND::GeomId::Def::P0D::Global::kSeqIdMSB,
                ND::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(0,
                ND::GeomId::Def::P0D::Global::kFieldMSB,
                ND::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

ND::TGeometryId ND::GeomId::P0D::SuperP0Dule(int sP0Dule) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kP0D,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::P0D::kGlobal, 
                ND::GeomId::Def::P0D::kSeqIdMSB, 
                ND::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::P0D::Global::kSP0Dule,
                ND::GeomId::Def::P0D::Global::kSeqIdMSB,
                ND::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(sP0Dule,
                ND::GeomId::Def::P0D::Global::kFieldMSB,
                ND::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int ND::GeomId::P0D::GetSuperP0Dule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kGlobal 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::Global::kSP0Dule 
        != id.GetField(ND::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Global::kFieldMSB,
                       ND::GeomId::Def::P0D::Global::kFieldLSB);
}


ND::TGeometryId ND::GeomId::P0D::P0Dule(int p0dule) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kP0D,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::P0D::kGlobal, 
                ND::GeomId::Def::P0D::kSeqIdMSB,
                ND::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::P0D::Global::kP0Dule,
                ND::GeomId::Def::P0D::Global::kSeqIdMSB,
                ND::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(p0dule,
                ND::GeomId::Def::P0D::Global::kFieldMSB,
                ND::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int ND::GeomId::P0D::GetP0Dule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kGlobal 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::Global::kP0Dule 
        != id.GetField(ND::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Global::kFieldMSB,
                       ND::GeomId::Def::P0D::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::P0D::Target(int target) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kP0D,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::P0D::kGlobal,
                ND::GeomId::Def::P0D::kSeqIdMSB, 
                ND::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::P0D::Global::kTarget,
                ND::GeomId::Def::P0D::Global::kSeqIdMSB,
                ND::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(target,
                ND::GeomId::Def::P0D::Global::kFieldMSB,
                ND::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int ND::GeomId::P0D::GetTarget(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kGlobal 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::Global::kTarget 
        != id.GetField(ND::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Global::kFieldMSB,
                       ND::GeomId::Def::P0D::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::P0D::ECalRadiator(int radiator) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kP0D,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::P0D::kGlobal, 
                ND::GeomId::Def::P0D::kSeqIdMSB, 
                ND::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::P0D::Global::kECalRadiator,
                ND::GeomId::Def::P0D::Global::kSeqIdMSB,
                ND::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(radiator,
                ND::GeomId::Def::P0D::Global::kFieldMSB,
                ND::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int ND::GeomId::P0D::GetECalRadiator(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kGlobal 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::Global::kECalRadiator
        != id.GetField(ND::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Global::kFieldMSB,
                       ND::GeomId::Def::P0D::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::P0D::TargetRadiator(int targetRadiator) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kP0D,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::P0D::kGlobal, 
                ND::GeomId::Def::P0D::kSeqIdMSB, 
                ND::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::P0D::Global::kTargetRadiator,
                ND::GeomId::Def::P0D::Global::kSeqIdMSB,
                ND::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(targetRadiator,
                ND::GeomId::Def::P0D::Global::kFieldMSB,
                ND::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int ND::GeomId::P0D::GetTargetRadiator(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kGlobal 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::Global::kTargetRadiator
        != id.GetField(ND::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Global::kFieldMSB,
                       ND::GeomId::Def::P0D::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::P0D::Bar(int P0Dule, 
                                     int layer,
                                     int bar) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kP0D,
                ND::GeomId::Def::kDetectorIdMSB,
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::P0D::kScintillator,
                ND::GeomId::Def::P0D::kSeqIdMSB,
                ND::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(0,
                ND::GeomId::Def::P0D::Bar::kSP0DuleMSB,
                ND::GeomId::Def::P0D::Bar::kSP0DuleLSB);
    id.SetField(P0Dule,
                ND::GeomId::Def::P0D::Bar::kP0DuleMSB,
                ND::GeomId::Def::P0D::Bar::kP0DuleLSB);
    id.SetField(layer,
                ND::GeomId::Def::P0D::Bar::kLayerMSB,
                ND::GeomId::Def::P0D::Bar::kLayerLSB);
    id.SetField(bar,
                ND::GeomId::Def::P0D::Bar::kBarMSB,
                ND::GeomId::Def::P0D::Bar::kBarLSB);
    return id;
}

int ND::GeomId::P0D::GetBarP0Dule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kScintillator 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Bar::kP0DuleMSB,
                       ND::GeomId::Def::P0D::Bar::kP0DuleLSB);
}

int ND::GeomId::P0D::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kScintillator 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Bar::kLayerMSB,
                       ND::GeomId::Def::P0D::Bar::kLayerLSB);
}

int ND::GeomId::P0D::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kP0D 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::P0D::kScintillator 
        != id.GetField(ND::GeomId::Def::P0D::kSeqIdMSB, 
                       ND::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::P0D::Bar::kBarMSB,
                       ND::GeomId::Def::P0D::Bar::kBarLSB);
}

////////////////////////////////////////////////////////////
// TPC Geometry Identifiers
////////////////////////////////////////////////////////////

bool ND::GeomId::TPC::IsTPC(TGeometryId i) {
    TSettableGeometryId id(i);
    return (ND::GeomId::Def::kTPC 
            == id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                           ND::GeomId::Def::kDetectorIdLSB));
}

ND::TGeometryId ND::GeomId::TPC::Module(int tpc) {
    TSettableGeometryId id;
    if (tpc<0 || tpc>2) {
        ND280Warn("TPC module out of range [0,2]: " << tpc);
        return id;
    }
    id.SetField(ND::GeomId::Def::kTPC,
                ND::GeomId::Def::kDetectorIdMSB,
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::TPC::kGlobal,
                ND::GeomId::Def::TPC::kSeqIdMSB,
                ND::GeomId::Def::TPC::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::TPC::Global::kTPC,
                ND::GeomId::Def::TPC::Global::kSeqIdMSB,
                ND::GeomId::Def::TPC::Global::kSeqIdLSB);
    id.SetField(tpc,
                ND::GeomId::Def::TPC::Global::kFieldMSB,
                ND::GeomId::Def::TPC::Global::kFieldLSB);
    return id;
}

int ND::GeomId::TPC::GetModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kTPC 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::TPC::kGlobal 
        != id.GetField(ND::GeomId::Def::TPC::kSeqIdMSB, 
                       ND::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::TPC::Global::kTPC
        != id.GetField(ND::GeomId::Def::TPC::Global::kSeqIdMSB, 
                       ND::GeomId::Def::TPC::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::TPC::Global::kFieldMSB,
                       ND::GeomId::Def::TPC::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::TPC::TPC1() {return Module(0);}
ND::TGeometryId ND::GeomId::TPC::TPC2() {return Module(1);}
ND::TGeometryId ND::GeomId::TPC::TPC3() {return Module(2);}

bool ND::GeomId::TPC::IsTPC1(TGeometryId i) {
    return (ND::GeomId::TPC::GetModule(i) == 0);
}

bool ND::GeomId::TPC::IsTPC2(TGeometryId i) {
    return (ND::GeomId::TPC::GetModule(i) == 1);
}

bool ND::GeomId::TPC::IsTPC3(TGeometryId i) {
    return (ND::GeomId::TPC::GetModule(i) == 2);
}

ND::TGeometryId ND::GeomId::TPC::MicroMega(int tpc, int half, int mm) {
    TSettableGeometryId id;
    if (tpc<0 || tpc>2) {
        ND280Warn("TPC module out of range [0,2]: " << tpc);
        return id;
    }
    if (half<0 || half>1) {
        ND280Warn("TPC half out of range [0,1]: " << half);
        return id;
    }
    if (mm<0 || mm>11) {
        ND280Warn("TPC micromega out of range [0,11]: " << mm);
        return id;
    }
    id.SetField(ND::GeomId::Def::kTPC,
                ND::GeomId::Def::kDetectorIdMSB,
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::TPC::kPad,
                ND::GeomId::Def::TPC::kSeqIdMSB,
                ND::GeomId::Def::TPC::kSeqIdLSB);
    id.SetField(tpc,
                ND::GeomId::Def::TPC::Pad::kTPCMSB,
                ND::GeomId::Def::TPC::Pad::kTPCLSB);
    id.SetField(half,
                ND::GeomId::Def::TPC::Pad::kHalfMSB,
                ND::GeomId::Def::TPC::Pad::kHalfLSB);
    id.SetField(mm,
                ND::GeomId::Def::TPC::Pad::kMMegaMSB,
                ND::GeomId::Def::TPC::Pad::kMMegaLSB);
    id.SetField(0,
                ND::GeomId::Def::TPC::Pad::kPadFlagMSB,
                ND::GeomId::Def::TPC::Pad::kPadFlagLSB);
    id.SetField(0,
                ND::GeomId::Def::TPC::Pad::kPadMSB,
                ND::GeomId::Def::TPC::Pad::kPadLSB);
    return id;
}

bool ND::GeomId::TPC::IsMicroMega(ND::TGeometryId i) {
    TSettableGeometryId id(i);
    int detector = id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                               ND::GeomId::Def::kDetectorIdLSB);
    if (detector != ND::GeomId::Def::kTPC) return false;
    /// This may be a pad.
    int seqId = id.GetField(ND::GeomId::Def::TPC::kSeqIdMSB,
                            ND::GeomId::Def::TPC::kSeqIdLSB);
    if (seqId != ND::GeomId::Def::TPC::kPad) return false;
    int padFlag = id.GetField(ND::GeomId::Def::TPC::Pad::kPadFlagMSB,
                              ND::GeomId::Def::TPC::Pad::kPadFlagLSB);
    if (padFlag) return false;
    return true;
}

ND::TGeometryId ND::GeomId::TPC::Pad(int tpc, int half, int mm, int pad) {
    TSettableGeometryId id;
    if (tpc<0 || tpc>2) {
        ND280Warn("TPC module out of range [0,2]: " << tpc);
        return id;
    }
    if (half<0 || half>1) {
        ND280Warn("TPC half out of range [0,1]: " << half);
        return id;
    }
    if (mm<0 || mm>11) {
        ND280Warn("TPC micromega out of range [0,11]: " << mm);
        return id;
    }
    if (pad<0 || pad>1727) {
        ND280Warn("TPC pad out of range [0,1727]: " << pad);
        return id;
    }
    id.SetField(ND::GeomId::Def::kTPC,
                ND::GeomId::Def::kDetectorIdMSB,
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::TPC::kPad,
                ND::GeomId::Def::TPC::kSeqIdMSB,
                ND::GeomId::Def::TPC::kSeqIdLSB);
    id.SetField(tpc,
                ND::GeomId::Def::TPC::Pad::kTPCMSB,
                ND::GeomId::Def::TPC::Pad::kTPCLSB);
    id.SetField(half,
                ND::GeomId::Def::TPC::Pad::kHalfMSB,
                ND::GeomId::Def::TPC::Pad::kHalfLSB);
    id.SetField(mm,
                ND::GeomId::Def::TPC::Pad::kMMegaMSB,
                ND::GeomId::Def::TPC::Pad::kMMegaLSB);
    id.SetField(1,
                ND::GeomId::Def::TPC::Pad::kPadFlagMSB,
                ND::GeomId::Def::TPC::Pad::kPadFlagLSB);
    id.SetField(pad,
                ND::GeomId::Def::TPC::Pad::kPadMSB,
                ND::GeomId::Def::TPC::Pad::kPadLSB);
    return id;
}

int ND::GeomId::TPC::GetMicroMegaTPC(TGeometryId i) {
    return ND::GeomId::TPC::GetPadTPC(i);
}

int ND::GeomId::TPC::GetPadTPC(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kTPC 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::TPC::kPad 
        != id.GetField(ND::GeomId::Def::TPC::kSeqIdMSB, 
                       ND::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::TPC::Pad::kTPCMSB, 
                       ND::GeomId::Def::TPC::Pad::kTPCLSB);
}

int ND::GeomId::TPC::GetMicroMegaHalf(TGeometryId i) {
    return ND::GeomId::TPC::GetPadHalf(i);
}

int ND::GeomId::TPC::GetPadHalf(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kTPC 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::TPC::kPad 
        != id.GetField(ND::GeomId::Def::TPC::kSeqIdMSB, 
                       ND::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::TPC::Pad::kHalfMSB, 
                       ND::GeomId::Def::TPC::Pad::kHalfLSB);
}

int ND::GeomId::TPC::GetMicroMegaNumber(TGeometryId i) {
    return ND::GeomId::TPC::GetPadMicroMega(i);
}

int ND::GeomId::TPC::GetPadMicroMega(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kTPC 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::TPC::kPad 
        != id.GetField(ND::GeomId::Def::TPC::kSeqIdMSB, 
                       ND::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::TPC::Pad::kMMegaMSB, 
                       ND::GeomId::Def::TPC::Pad::kMMegaLSB);
}

int ND::GeomId::TPC::GetPadNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kTPC 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::TPC::kPad 
        != id.GetField(ND::GeomId::Def::TPC::kSeqIdMSB, 
                       ND::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    if (1 != id.GetField(ND::GeomId::Def::TPC::Pad::kPadFlagMSB, 
                         ND::GeomId::Def::TPC::Pad::kPadFlagLSB)) return -1;
    return id.GetField(ND::GeomId::Def::TPC::Pad::kPadMSB, 
                       ND::GeomId::Def::TPC::Pad::kPadLSB);
}

bool ND::GeomId::TPC::IsPad(ND::TGeometryId i) {
    TSettableGeometryId id(i);
    int detector = id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                               ND::GeomId::Def::kDetectorIdLSB);
    if (detector != ND::GeomId::Def::kTPC) return false;
    /// This may be a pad.
    int seqId = id.GetField(ND::GeomId::Def::TPC::kSeqIdMSB,
                            ND::GeomId::Def::TPC::kSeqIdLSB);
    if (seqId != ND::GeomId::Def::TPC::kPad) return false;
    int padFlag = id.GetField(ND::GeomId::Def::TPC::Pad::kPadFlagMSB,
                              ND::GeomId::Def::TPC::Pad::kPadFlagLSB);
    if (!padFlag) return false;
    return true;
}

////////////////////////////////////////////////////////////
// FGD Geometry Identifiers.
////////////////////////////////////////////////////////////
bool ND::GeomId::FGD::IsFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    return (ND::GeomId::Def::kFGD
            == id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                           ND::GeomId::Def::kDetectorIdLSB));
}

ND::TGeometryId ND::GeomId::FGD::FGD(int fgd) {
    TSettableGeometryId id;
    if (fgd<0 || fgd>1) {
        ND280Warn("FGD out of range [0,1]: " << fgd);
        return id;
    }
    id.SetField(ND::GeomId::Def::kFGD,
                ND::GeomId::Def::kDetectorIdMSB,
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::FGD::kGlobal,
                ND::GeomId::Def::FGD::kSeqIdMSB,
                ND::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::FGD::Global::kFGD,
                ND::GeomId::Def::FGD::Global::kSeqIdMSB,
                ND::GeomId::Def::FGD::Global::kSeqIdLSB);
    id.SetField(fgd,
                ND::GeomId::Def::FGD::Global::kFieldMSB,
                ND::GeomId::Def::FGD::Global::kFieldLSB);
    return id;
}

ND::TGeometryId ND::GeomId::FGD::FGD1() {return FGD(0);}
ND::TGeometryId ND::GeomId::FGD::FGD2() {return FGD(1);}

int ND::GeomId::FGD::GetFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kGlobal 
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::Global::kFGD
        != id.GetField(ND::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Global::kFieldMSB,
                       ND::GeomId::Def::FGD::Global::kFieldLSB);
}

bool ND::GeomId::FGD::IsFGD1(TGeometryId id) {
    return (ND::GeomId::FGD::GetFGD(id) == 0);
}

bool ND::GeomId::FGD::IsFGD2(TGeometryId id) {
    return (ND::GeomId::FGD::GetFGD(id) == 1);
}

ND::TGeometryId ND::GeomId::FGD::Bar(int fgd, 
                                     int module,
                                     int layer,
                                     int bar) {
    TSettableGeometryId id;
    if (fgd<0 || fgd>1) {
        ND280Warn("FGD out of range [0,1]: " << fgd);
        return id;
    }
    id.SetField(ND::GeomId::Def::kFGD,
                ND::GeomId::Def::kDetectorIdMSB,
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::FGD::kScintillator,
                ND::GeomId::Def::FGD::kSeqIdMSB,
                ND::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(fgd,
                ND::GeomId::Def::FGD::Bar::kModuleMSB,
                ND::GeomId::Def::FGD::Bar::kModuleLSB);
    id.SetField(module,
                ND::GeomId::Def::FGD::Bar::kPlaneMSB,
                ND::GeomId::Def::FGD::Bar::kPlaneLSB);
    id.SetField(layer,
                ND::GeomId::Def::FGD::Bar::kLayerMSB,
                ND::GeomId::Def::FGD::Bar::kLayerLSB);
    id.SetField(bar,
                ND::GeomId::Def::FGD::Bar::kBarMSB,
                ND::GeomId::Def::FGD::Bar::kBarLSB);
    return id;
}

int ND::GeomId::FGD::GetBarFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kScintillator
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Bar::kModuleMSB,
                       ND::GeomId::Def::FGD::Bar::kModuleLSB);
}

int ND::GeomId::FGD::GetBarModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kScintillator
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Bar::kPlaneMSB,
                       ND::GeomId::Def::FGD::Bar::kPlaneLSB);
}

int ND::GeomId::FGD::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kScintillator
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Bar::kLayerMSB,
                       ND::GeomId::Def::FGD::Bar::kLayerLSB);
}

int ND::GeomId::FGD::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kScintillator
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Bar::kBarMSB,
                       ND::GeomId::Def::FGD::Bar::kBarLSB);
}

ND::TGeometryId ND::GeomId::FGD::Target(int target) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kFGD,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::FGD::kGlobal,
                ND::GeomId::Def::FGD::kSeqIdMSB, 
                ND::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::FGD::Global::kTarget,
                ND::GeomId::Def::FGD::Global::kSeqIdMSB,
                ND::GeomId::Def::FGD::Global::kSeqIdLSB);
    id.SetField(target,
                ND::GeomId::Def::FGD::Global::kFieldMSB,
                ND::GeomId::Def::FGD::Global::kFieldLSB);
    return id;
}

int ND::GeomId::FGD::GetTarget(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kGlobal 
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::Global::kTarget
        != id.GetField(ND::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Global::kFieldMSB,
                       ND::GeomId::Def::FGD::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::FGD::Layer(int fgd, 
                                       int module,
                                       int layer) {
    TSettableGeometryId id;
    if (fgd<0 || fgd>1) {
        ND280Warn("FGD out of range [0,1]: " << fgd);
        return id;
    }
    id.SetField(ND::GeomId::Def::kFGD,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::FGD::kGlobal,
                ND::GeomId::Def::FGD::kSeqIdMSB, 
                ND::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::FGD::Global::kLayer,
                ND::GeomId::Def::FGD::Global::kSeqIdMSB,
                ND::GeomId::Def::FGD::Global::kSeqIdLSB);
    id.SetField(fgd,
                ND::GeomId::Def::FGD::Global::Layer::kFGDMSB,
                ND::GeomId::Def::FGD::Global::Layer::kFGDLSB);
    id.SetField(layer,
                ND::GeomId::Def::FGD::Global::Layer::kLayerMSB,
                ND::GeomId::Def::FGD::Global::Layer::kLayerLSB);
    id.SetField(module,
                ND::GeomId::Def::FGD::Global::Layer::kModuleMSB,
                ND::GeomId::Def::FGD::Global::Layer::kModuleLSB);
    return id;
}

int ND::GeomId::FGD::GetLayerFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kGlobal 
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::Global::kLayer
        != id.GetField(ND::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Global::Layer::kFGDMSB,
                       ND::GeomId::Def::FGD::Global::Layer::kFGDLSB);
}

int ND::GeomId::FGD::GetLayerModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kGlobal 
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::Global::kLayer
        != id.GetField(ND::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Global::Layer::kModuleMSB,
                       ND::GeomId::Def::FGD::Global::Layer::kModuleLSB);
}

int ND::GeomId::FGD::GetLayerNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kFGD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::kGlobal 
        != id.GetField(ND::GeomId::Def::FGD::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::FGD::Global::kLayer
        != id.GetField(ND::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(ND::GeomId::Def::FGD::Global::Layer::kLayerMSB,
                       ND::GeomId::Def::FGD::Global::Layer::kLayerLSB);
}

////////////////////////////////////////////////////////////
// ECal Geometry Identifiers.
////////////////////////////////////////////////////////////

ND::TGeometryId ND::GeomId::ECal::Module(int ecal, int clam, int module) {
    TSettableGeometryId id;
    if (ecal == ND::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kPECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("P0D ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("P0D ECal with invalid module: " << module);
            return id;
        }
    }
    id.SetField(ecal,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::ECal::kGlobal,
                ND::GeomId::Def::ECal::kSeqIdMSB,
                ND::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                ND::GeomId::Def::ECal::kModuleMSB,
                ND::GeomId::Def::ECal::kModuleLSB);

    id.SetField(ND::GeomId::Def::ECal::Global::kECal,
                ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                ND::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(0,
                ND::GeomId::Def::ECal::Global::kFieldMSB,
                ND::GeomId::Def::ECal::Global::kFieldLSB);

    return id;
}

ND::TGeometryId ND::GeomId::ECal::Container(int ecal, int clam, int module) {
    TSettableGeometryId id;
    if (ecal ==  ND::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal container with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal container with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kPECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("P0D ECal container with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("P0D ECal container with invalid module: " << module);
            return id;
        }
    }
    else {
        ND280Error("Container for non-contained ECal: " << ecal);
        return id;
    }
    
    id.SetField(ecal,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::ECal::kGlobal,
                ND::GeomId::Def::ECal::kSeqIdMSB,
                ND::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                ND::GeomId::Def::ECal::kModuleMSB,
                ND::GeomId::Def::ECal::kModuleLSB);

    id.SetField(ND::GeomId::Def::ECal::Global::kContainer,
                ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                ND::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(0,
                ND::GeomId::Def::ECal::Global::kFieldMSB,
                ND::GeomId::Def::ECal::Global::kFieldLSB);

    return id;
}
    

int ND::GeomId::ECal::GetModuleECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = ND::GeomId::GetSubSystem(id);
    if (det == ND::GeomId::Def::kDSECal) return det;
    else if (det == ND::GeomId::Def::kTECal) return det;
    else if (det == ND::GeomId::Def::kPECal) return det;
    else return -1;
}

int ND::GeomId::ECal::GetModuleNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetModuleECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kECal
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) / 2;
}

int ND::GeomId::ECal::GetModuleClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetModuleECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kECal
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) % 2;
}

ND::TGeometryId ND::GeomId::ECal::Layer(int ecal, int clam, 
                                        int module, int layer) {
    TSettableGeometryId id;
    if (ecal == ND::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kPECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("P0D ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("P0D ECal with invalid module: " << module);
            return id;
        }
    }
    id.SetField(ecal,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::ECal::kGlobal,
                ND::GeomId::Def::ECal::kSeqIdMSB,
                ND::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                ND::GeomId::Def::ECal::kModuleMSB,
                ND::GeomId::Def::ECal::kModuleLSB);
    id.SetField(ND::GeomId::Def::ECal::Global::kLayer,
                ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                ND::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(layer,
                ND::GeomId::Def::ECal::Global::kFieldMSB,
                ND::GeomId::Def::ECal::Global::kFieldLSB);
    return id;
}

int ND::GeomId::ECal::GetLayerECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = ND::GeomId::GetSubSystem(id);
    if (det == ND::GeomId::Def::kDSECal) return det;
    else if (det == ND::GeomId::Def::kTECal) return det;
    else if (det == ND::GeomId::Def::kPECal) return det;
    else return -1;
}

int ND::GeomId::ECal::GetLayerClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetLayerECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kLayer
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) % 2;
}

int ND::GeomId::ECal::GetLayerModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetLayerECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kLayer
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) / 2;
}

int ND::GeomId::ECal::GetLayerNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetLayerECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kLayer
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::Global::kFieldMSB,
                       ND::GeomId::Def::ECal::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::ECal::Radiator(int ecal, int clam,
                                           int module, int rad) {
    TSettableGeometryId id;
    if (ecal == ND::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kPECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("P0D ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("P0D ECal with invalid module: " << module);
            return id;
        }
    }
    id.SetField(ecal,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::ECal::kGlobal,
                ND::GeomId::Def::ECal::kSeqIdMSB,
                ND::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                ND::GeomId::Def::ECal::kModuleMSB,
                ND::GeomId::Def::ECal::kModuleLSB);
    id.SetField(ND::GeomId::Def::ECal::Global::kRadiator,
                ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                ND::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(rad,
                ND::GeomId::Def::ECal::Global::kFieldMSB,
                ND::GeomId::Def::ECal::Global::kFieldLSB);
    return id;
}

int ND::GeomId::ECal::GetRadiatorECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = ND::GeomId::GetSubSystem(id);
    if (det == ND::GeomId::Def::kDSECal) return det;
    else if (det == ND::GeomId::Def::kTECal) return det;
    else if (det == ND::GeomId::Def::kPECal) return det;
    else return -1;
}

int ND::GeomId::ECal::GetRadiatorClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetRadiatorECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kRadiator
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) % 2;
}

int ND::GeomId::ECal::GetRadiatorModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetRadiatorECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kRadiator
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) / 2;
}

int ND::GeomId::ECal::GetRadiatorNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetRadiatorECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kGlobal
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::ECal::Global::kRadiator
        != id.GetField(ND::GeomId::Def::ECal::Global::kSeqIdMSB,
                       ND::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::Global::kFieldMSB,
                       ND::GeomId::Def::ECal::Global::kFieldLSB);
}

ND::TGeometryId ND::GeomId::ECal::Bar(int ecal, int clam,
                                      int module, int layer, int bar) {
    TSettableGeometryId id;
    if (ecal == ND::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == ND::GeomId::Def::kPECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("P0D ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("P0D ECal with invalid module: " << module);
            return id;
        }
    }
    id.SetField(ecal,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::ECal::kScintillator,
                ND::GeomId::Def::ECal::kSeqIdMSB,
                ND::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                ND::GeomId::Def::ECal::kModuleMSB,
                ND::GeomId::Def::ECal::kModuleLSB);
    id.SetField(layer,
                ND::GeomId::Def::ECal::Bar::kLayerMSB,
                ND::GeomId::Def::ECal::Bar::kLayerLSB);
    id.SetField(bar,
                ND::GeomId::Def::ECal::Bar::kBarMSB,
                ND::GeomId::Def::ECal::Bar::kBarLSB);
    return id;
}

int ND::GeomId::ECal::GetBarECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = ND::GeomId::GetSubSystem(id);
    if (det == ND::GeomId::Def::kDSECal) return det;
    else if (det == ND::GeomId::Def::kTECal) return det;
    else if (det == ND::GeomId::Def::kPECal) return det;
    else return -1;
}

int ND::GeomId::ECal::GetBarClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kScintillator
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) % 2;
}

int ND::GeomId::ECal::GetBarModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kScintillator
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::kModuleMSB,
                       ND::GeomId::Def::ECal::kModuleLSB) / 2;
}

int ND::GeomId::ECal::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kScintillator
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::Bar::kLayerMSB,
                       ND::GeomId::Def::ECal::Bar::kLayerLSB);
}

int ND::GeomId::ECal::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (ND::GeomId::Def::ECal::kScintillator
        != id.GetField(ND::GeomId::Def::ECal::kSeqIdMSB,
                       ND::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::ECal::Bar::kBarMSB,
                       ND::GeomId::Def::ECal::Bar::kBarLSB);
}

////////////////////////////////////////////////////////////
// Downstream ECal Geometry Identifiers.
////////////////////////////////////////////////////////////
bool ND::GeomId::DSECal::IsDSECal(TGeometryId i) {
    TSettableGeometryId id(i);
    return (ND::GeomId::Def::kDSECal
            == id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                           ND::GeomId::Def::kDetectorIdLSB));
}

ND::TGeometryId ND::GeomId::DSECal::Detector() {
    return ND::GeomId::ECal::Module(ND::GeomId::Def::kDSECal,0,0);
}

ND::TGeometryId ND::GeomId::DSECal::Layer(int layer) {
    return ND::GeomId::ECal::Layer(ND::GeomId::Def::kDSECal,0,0,layer);
}

int ND::GeomId::DSECal::GetLayer(TGeometryId i) {
    if (!ND::GeomId::DSECal::IsDSECal(i)) return -1;
    return ND::GeomId::ECal::GetLayerNumber(i);
}

ND::TGeometryId ND::GeomId::DSECal::Radiator(int rad) {
    return ND::GeomId::ECal::Radiator(ND::GeomId::Def::kDSECal,0,0,rad);
}

int ND::GeomId::DSECal::GetRadiator(TGeometryId i) {
    if (!ND::GeomId::DSECal::IsDSECal(i)) return -1;
    return ND::GeomId::ECal::GetRadiatorNumber(i);
}

ND::TGeometryId ND::GeomId::DSECal::Bar(int layer, int bar) {
    return ND::GeomId::ECal::Bar(ND::GeomId::Def::kDSECal,
                                 0,0,layer,bar);
}

int ND::GeomId::DSECal::GetBarLayer(TGeometryId i) {
    if (!ND::GeomId::DSECal::IsDSECal(i)) return -1;
    return ND::GeomId::ECal::GetBarLayer(i);
}

int ND::GeomId::DSECal::GetBarNumber(TGeometryId i) {
    if (!ND::GeomId::DSECal::IsDSECal(i)) return -1;
    return ND::GeomId::ECal::GetBarNumber(i);
}

////////////////////////////////////////////////////////////
// Tracker ECal Geometry Identifiers.
////////////////////////////////////////////////////////////
bool ND::GeomId::TECal::IsTECal(TGeometryId i) {
    TSettableGeometryId id(i);
    return (ND::GeomId::Def::kTECal
            == id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                           ND::GeomId::Def::kDetectorIdLSB));
}

ND::TGeometryId ND::GeomId::TECal::Module(int clam, int module) {
    return ND::GeomId::ECal::Module(ND::GeomId::Def::kTECal,
                                    clam,module);
}

ND::TGeometryId ND::GeomId::TECal::Layer(int clam, int module, int layer) {
    return ND::GeomId::ECal::Layer(ND::GeomId::Def::kTECal,
                                   clam,module,layer);
}

ND::TGeometryId ND::GeomId::TECal::Radiator(int clam, int module, int rad) {
    return ND::GeomId::ECal::Radiator(ND::GeomId::Def::kTECal,
                                      clam,module,rad);
}

ND::TGeometryId ND::GeomId::TECal::Bar(int clam, int mod, int layer, int bar) {
    return ND::GeomId::ECal::Bar(ND::GeomId::Def::kTECal,
                                      clam,mod,layer,bar);
}

int ND::GeomId::TECal::GetModuleNumber(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetModuleNumber(i);
}

int ND::GeomId::TECal::GetModuleClam(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetModuleClam(i);
}

int ND::GeomId::TECal::GetLayerNumber(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetLayerNumber(i);
}

int ND::GeomId::TECal::GetLayerClam(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetLayerClam(i);
}

int ND::GeomId::TECal::GetLayerModule(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetLayerModule(i);
}

int ND::GeomId::TECal::GetRadiatorNumber(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetRadiatorNumber(i);
}

int ND::GeomId::TECal::GetRadiatorClam(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetRadiatorClam(i);
}

int ND::GeomId::TECal::GetRadiatorModule(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetRadiatorModule(i);
}

int ND::GeomId::TECal::GetBarNumber(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetBarNumber(i);
}

int ND::GeomId::TECal::GetBarLayer(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetBarLayer(i);
}

int ND::GeomId::TECal::GetBarClam(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetBarClam(i);
}

int ND::GeomId::TECal::GetBarModule(TGeometryId i) {
    if (!ND::GeomId::TECal::IsTECal(i)) return -1;
    return ND::GeomId::ECal::GetBarModule(i);
}

////////////////////////////////////////////////////////////
// P0D ECal Geometry Identifiers.
////////////////////////////////////////////////////////////
bool ND::GeomId::PECal::IsPECal(TGeometryId i) {
    TSettableGeometryId id(i);
    return (ND::GeomId::Def::kPECal
            == id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                           ND::GeomId::Def::kDetectorIdLSB));
}

ND::TGeometryId ND::GeomId::PECal::Module(int clam, int module) {
    return ND::GeomId::ECal::Module(ND::GeomId::Def::kPECal,
                                    clam,module);
}

ND::TGeometryId ND::GeomId::PECal::Layer(int clam, int module, int layer) {
    return ND::GeomId::ECal::Layer(ND::GeomId::Def::kPECal,
                                   clam,module,layer);
}

ND::TGeometryId ND::GeomId::PECal::Radiator(int clam, int module, int rad) {
    return ND::GeomId::ECal::Radiator(ND::GeomId::Def::kPECal,
                                      clam,module,rad);
}

ND::TGeometryId ND::GeomId::PECal::Bar(int clam, int mod, int layer, int bar) {
    return ND::GeomId::ECal::Bar(ND::GeomId::Def::kPECal,
                                      clam,mod,layer,bar);
}

int ND::GeomId::PECal::GetModuleNumber(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetModuleNumber(i);
}

int ND::GeomId::PECal::GetModuleClam(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetModuleClam(i);
}

int ND::GeomId::PECal::GetLayerNumber(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetLayerNumber(i);
}

int ND::GeomId::PECal::GetLayerClam(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetLayerClam(i);
}

int ND::GeomId::PECal::GetLayerModule(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetLayerModule(i);
}

int ND::GeomId::PECal::GetRadiatorNumber(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetRadiatorNumber(i);
}

int ND::GeomId::PECal::GetRadiatorClam(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetRadiatorClam(i);
}

int ND::GeomId::PECal::GetRadiatorModule(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetRadiatorModule(i);
}

int ND::GeomId::PECal::GetBarNumber(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetBarNumber(i);
}

int ND::GeomId::PECal::GetBarLayer(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetBarLayer(i);
}

int ND::GeomId::PECal::GetBarClam(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetBarClam(i);
}

int ND::GeomId::PECal::GetBarModule(TGeometryId i) {
    if (!ND::GeomId::PECal::IsPECal(i)) return -1;
    return ND::GeomId::ECal::GetBarModule(i);
}

////////////////////////////////////////////////////////////
// SMRD Geometry Identifiers.
////////////////////////////////////////////////////////////
bool ND::GeomId::SMRD::IsSMRD(TGeometryId i) {
    TSettableGeometryId id(i);
    return (ND::GeomId::Def::kSMRD 
            == id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                           ND::GeomId::Def::kDetectorIdLSB));
}

ND::TGeometryId ND::GeomId::SMRD::Module(int clam, int yoke, 
                                         int layer, int slot) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kSMRD,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::SMRD::kGlobal, 
                ND::GeomId::Def::SMRD::kSeqIdMSB, 
                ND::GeomId::Def::SMRD::kSeqIdLSB);
    id.SetField(ND::GeomId::Def::SMRD::Global::kModule,
                ND::GeomId::Def::SMRD::Global::kSeqIdMSB,
                ND::GeomId::Def::SMRD::Global::kSeqIdLSB);
    id.SetField(clam,
                ND::GeomId::Def::SMRD::Global::Module::kClamMSB,
                ND::GeomId::Def::SMRD::Global::Module::kClamLSB);
    id.SetField(yoke,
                ND::GeomId::Def::SMRD::Global::Module::kYokeMSB,
                ND::GeomId::Def::SMRD::Global::Module::kYokeLSB);
    id.SetField(layer,
                ND::GeomId::Def::SMRD::Global::Module::kLayerMSB,
                ND::GeomId::Def::SMRD::Global::Module::kLayerLSB);
    id.SetField(slot,
                ND::GeomId::Def::SMRD::Global::Module::kSlotMSB,
                ND::GeomId::Def::SMRD::Global::Module::kSlotLSB);
    return id;
}

int ND::GeomId::SMRD::GetModuleClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kGlobal
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::Global::kModule
        != id.GetField(ND::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(ND::GeomId::Def::SMRD::Global::Module::kClamMSB, 
                       ND::GeomId::Def::SMRD::Global::Module::kClamLSB);
}

int ND::GeomId::SMRD::GetModuleYoke(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kGlobal
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::Global::kModule
        != id.GetField(ND::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    

    return id.GetField(ND::GeomId::Def::SMRD::Global::Module::kYokeMSB, 
                       ND::GeomId::Def::SMRD::Global::Module::kYokeLSB);
}

int ND::GeomId::SMRD::GetModuleLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kGlobal
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::Global::kModule
        != id.GetField(ND::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    

    return id.GetField(ND::GeomId::Def::SMRD::Global::Module::kLayerMSB, 
                       ND::GeomId::Def::SMRD::Global::Module::kLayerLSB);
}

int ND::GeomId::SMRD::GetModuleSlot(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kGlobal
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::Global::kModule
        != id.GetField(ND::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    

    return id.GetField(ND::GeomId::Def::SMRD::Global::Module::kSlotMSB, 
                       ND::GeomId::Def::SMRD::Global::Module::kSlotLSB);
}

ND::TGeometryId ND::GeomId::SMRD::Bar(int clam, int yoke, 
                                      int layer, int slot, int bar) {
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kSMRD,
                ND::GeomId::Def::kDetectorIdMSB, 
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(ND::GeomId::Def::SMRD::kScintillator, 
                ND::GeomId::Def::SMRD::kSeqIdMSB, 
                ND::GeomId::Def::SMRD::kSeqIdLSB);
    id.SetField(clam,
                ND::GeomId::Def::SMRD::Bar::kClamMSB,
                ND::GeomId::Def::SMRD::Bar::kClamLSB);
    id.SetField(yoke,
                ND::GeomId::Def::SMRD::Bar::kYokeMSB,
                ND::GeomId::Def::SMRD::Bar::kYokeLSB);
    id.SetField(layer,
                ND::GeomId::Def::SMRD::Bar::kLayerMSB,
                ND::GeomId::Def::SMRD::Bar::kLayerLSB);
    id.SetField(slot,
                ND::GeomId::Def::SMRD::Bar::kSlotMSB,
                ND::GeomId::Def::SMRD::Bar::kSlotLSB);
    id.SetField(bar,
                ND::GeomId::Def::SMRD::Bar::kBarMSB,
                ND::GeomId::Def::SMRD::Bar::kBarLSB);
    return id;
}

int ND::GeomId::SMRD::GetBarClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kScintillator
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(ND::GeomId::Def::SMRD::Bar::kClamMSB, 
                       ND::GeomId::Def::SMRD::Bar::kClamLSB);
}

int ND::GeomId::SMRD::GetBarYoke(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kScintillator
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(ND::GeomId::Def::SMRD::Bar::kYokeMSB, 
                       ND::GeomId::Def::SMRD::Bar::kYokeLSB);
}

int ND::GeomId::SMRD::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kScintillator
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(ND::GeomId::Def::SMRD::Bar::kLayerMSB, 
                       ND::GeomId::Def::SMRD::Bar::kLayerLSB);
}

int ND::GeomId::SMRD::GetBarSlot(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kScintillator
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(ND::GeomId::Def::SMRD::Bar::kSlotMSB, 
                       ND::GeomId::Def::SMRD::Bar::kSlotLSB);
}

int ND::GeomId::SMRD::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (ND::GeomId::Def::kSMRD 
        != id.GetField(ND::GeomId::Def::kDetectorIdMSB,
                       ND::GeomId::Def::kDetectorIdLSB)) return -1;
    if (ND::GeomId::Def::SMRD::kScintillator
        != id.GetField(ND::GeomId::Def::SMRD::kSeqIdMSB, 
                       ND::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(ND::GeomId::Def::SMRD::Bar::kBarMSB, 
                       ND::GeomId::Def::SMRD::Bar::kBarLSB);
}

////////////////////////////////////////////////////////////
// INGRID Geometry Identifiers.
////////////////////////////////////////////////////////////

ND::TGeometryId ND::GeomId::INGRID::Scintillator(
    int objType, int obj, int trk, int proj, int scinti){
    TSettableGeometryId id;
    id.SetField(ND::GeomId::Def::kINGRID,
                ND::GeomId::Def::kDetectorIdMSB,
                ND::GeomId::Def::kDetectorIdLSB);
    id.SetField(objType,
                ND::GeomId::Def::INGRID::kIngridObjIDMSB,
                ND::GeomId::Def::INGRID::kIngridObjIDLSB);
    id.SetField(obj,
                ND::GeomId::Def::INGRID::kIngridModNumMSB,
                ND::GeomId::Def::INGRID::kIngridModNumLSB);
    id.SetField(trk,
                ND::GeomId::Def::INGRID::kIngridTrkNumMSB,
                ND::GeomId::Def::INGRID::kIngridTrkNumLSB);
    id.SetField(proj,
                ND::GeomId::Def::INGRID::kIngridProjMSB,
                ND::GeomId::Def::INGRID::kIngridProjLSB);
    id.SetField(scinti,
                ND::GeomId::Def::INGRID::kIngridSciNumMSB,
                ND::GeomId::Def::INGRID::kIngridSciNumLSB);
    return id;
}

ND::TGeometryId ND::GeomId::INGRID::ModScintillator(
    int obj, int trk, int proj, int scinti){
    return ND::GeomId::INGRID::Scintillator(
        ND::GeomId::Def::INGRID::kIngridModule, obj, trk, proj, scinti);
}

ND::TGeometryId ND::GeomId::INGRID::VertVetoScintillator(int obj, int scinti){
    return ND::GeomId::INGRID::Scintillator(
        ND::GeomId::Def::INGRID::kIngridVeto, obj, 0,
        ND::GeomId::Def::INGRID::kVertical, scinti);
}

ND::TGeometryId ND::GeomId::INGRID::HorzVetoScintillator(int obj, int scinti){
    return ND::GeomId::INGRID::Scintillator(
        ND::GeomId::Def::INGRID::kIngridVeto, obj, 0,
        ND::GeomId::Def::INGRID::kHorizontal, scinti);
}
