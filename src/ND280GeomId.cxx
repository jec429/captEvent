#include "TND280Log.hxx"
#include "TGeometryId.hxx"
#include "ND280GeomId.hxx"
#include "ND280GeomIdDef.hxx"

namespace {
    // Provide access to the protected setter and getter methods.
    class TSettableGeometryId: public CP::TGeometryId {
    public:
        TSettableGeometryId() {}
        explicit TSettableGeometryId(const CP::TGeometryId& id)
            : CP::TGeometryId(id) {}
        void SetField(int v, int m, int b) {
            CP::TGeometryId::SetFieldSafe(v,m,b);
        }
        int GetField(int m, int b) {
            return CP::TGeometryId::GetFieldSafe(m,b);
        }
    };
}

CP::TGeometryId CP::GeomId::Empty() {
    return TGeometryId();
}

int CP::GeomId::GetSubSystem(TGeometryId i) {
    TSettableGeometryId id(i);
    return id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB);
}

////////////////////////////////////////////////////////////
// P0D Geometry Identifiers.
////////////////////////////////////////////////////////////

bool CP::GeomId::P0D::IsP0D(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kP0D 
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::P0D::Detector() {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kP0D,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::P0D::kGlobal,
                CP::GeomId::Def::P0D::kSeqIdMSB,
                CP::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::P0D::Global::kP0D,
                CP::GeomId::Def::P0D::Global::kSeqIdMSB,
                CP::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::P0D::Global::kFieldMSB,
                CP::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

CP::TGeometryId CP::GeomId::P0D::SuperP0Dule(int sP0Dule) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kP0D,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::P0D::kGlobal, 
                CP::GeomId::Def::P0D::kSeqIdMSB, 
                CP::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::P0D::Global::kSP0Dule,
                CP::GeomId::Def::P0D::Global::kSeqIdMSB,
                CP::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(sP0Dule,
                CP::GeomId::Def::P0D::Global::kFieldMSB,
                CP::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int CP::GeomId::P0D::GetSuperP0Dule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kGlobal 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::Global::kSP0Dule 
        != id.GetField(CP::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Global::kFieldMSB,
                       CP::GeomId::Def::P0D::Global::kFieldLSB);
}


CP::TGeometryId CP::GeomId::P0D::P0Dule(int p0dule) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kP0D,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::P0D::kGlobal, 
                CP::GeomId::Def::P0D::kSeqIdMSB,
                CP::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::P0D::Global::kP0Dule,
                CP::GeomId::Def::P0D::Global::kSeqIdMSB,
                CP::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(p0dule,
                CP::GeomId::Def::P0D::Global::kFieldMSB,
                CP::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int CP::GeomId::P0D::GetP0Dule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kGlobal 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::Global::kP0Dule 
        != id.GetField(CP::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Global::kFieldMSB,
                       CP::GeomId::Def::P0D::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::P0D::Target(int target) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kP0D,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::P0D::kGlobal,
                CP::GeomId::Def::P0D::kSeqIdMSB, 
                CP::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::P0D::Global::kTarget,
                CP::GeomId::Def::P0D::Global::kSeqIdMSB,
                CP::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(target,
                CP::GeomId::Def::P0D::Global::kFieldMSB,
                CP::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int CP::GeomId::P0D::GetTarget(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kGlobal 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::Global::kTarget 
        != id.GetField(CP::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Global::kFieldMSB,
                       CP::GeomId::Def::P0D::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::P0D::ECalRadiator(int radiator) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kP0D,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::P0D::kGlobal, 
                CP::GeomId::Def::P0D::kSeqIdMSB, 
                CP::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::P0D::Global::kECalRadiator,
                CP::GeomId::Def::P0D::Global::kSeqIdMSB,
                CP::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(radiator,
                CP::GeomId::Def::P0D::Global::kFieldMSB,
                CP::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int CP::GeomId::P0D::GetECalRadiator(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kGlobal 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::Global::kECalRadiator
        != id.GetField(CP::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Global::kFieldMSB,
                       CP::GeomId::Def::P0D::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::P0D::TargetRadiator(int targetRadiator) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kP0D,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::P0D::kGlobal, 
                CP::GeomId::Def::P0D::kSeqIdMSB, 
                CP::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::P0D::Global::kTargetRadiator,
                CP::GeomId::Def::P0D::Global::kSeqIdMSB,
                CP::GeomId::Def::P0D::Global::kSeqIdLSB);
    id.SetField(targetRadiator,
                CP::GeomId::Def::P0D::Global::kFieldMSB,
                CP::GeomId::Def::P0D::Global::kFieldLSB);
    return id;
}

int CP::GeomId::P0D::GetTargetRadiator(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kGlobal 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::Global::kTargetRadiator
        != id.GetField(CP::GeomId::Def::P0D::Global::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Global::kFieldMSB,
                       CP::GeomId::Def::P0D::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::P0D::Bar(int P0Dule, 
                                     int layer,
                                     int bar) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kP0D,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::P0D::kScintillator,
                CP::GeomId::Def::P0D::kSeqIdMSB,
                CP::GeomId::Def::P0D::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::P0D::Bar::kSP0DuleMSB,
                CP::GeomId::Def::P0D::Bar::kSP0DuleLSB);
    id.SetField(P0Dule,
                CP::GeomId::Def::P0D::Bar::kP0DuleMSB,
                CP::GeomId::Def::P0D::Bar::kP0DuleLSB);
    id.SetField(layer,
                CP::GeomId::Def::P0D::Bar::kLayerMSB,
                CP::GeomId::Def::P0D::Bar::kLayerLSB);
    id.SetField(bar,
                CP::GeomId::Def::P0D::Bar::kBarMSB,
                CP::GeomId::Def::P0D::Bar::kBarLSB);
    return id;
}

int CP::GeomId::P0D::GetBarP0Dule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kScintillator 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Bar::kP0DuleMSB,
                       CP::GeomId::Def::P0D::Bar::kP0DuleLSB);
}

int CP::GeomId::P0D::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kScintillator 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Bar::kLayerMSB,
                       CP::GeomId::Def::P0D::Bar::kLayerLSB);
}

int CP::GeomId::P0D::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kP0D 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::P0D::kScintillator 
        != id.GetField(CP::GeomId::Def::P0D::kSeqIdMSB, 
                       CP::GeomId::Def::P0D::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::P0D::Bar::kBarMSB,
                       CP::GeomId::Def::P0D::Bar::kBarLSB);
}

////////////////////////////////////////////////////////////
// TPC Geometry Identifiers
////////////////////////////////////////////////////////////

bool CP::GeomId::TPC::IsTPC(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kTPC 
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::TPC::Module(int tpc) {
    TSettableGeometryId id;
    if (tpc<0 || tpc>2) {
        ND280Warn("TPC module out of range [0,2]: " << tpc);
        return id;
    }
    id.SetField(CP::GeomId::Def::kTPC,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::TPC::kGlobal,
                CP::GeomId::Def::TPC::kSeqIdMSB,
                CP::GeomId::Def::TPC::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::TPC::Global::kTPC,
                CP::GeomId::Def::TPC::Global::kSeqIdMSB,
                CP::GeomId::Def::TPC::Global::kSeqIdLSB);
    id.SetField(tpc,
                CP::GeomId::Def::TPC::Global::kFieldMSB,
                CP::GeomId::Def::TPC::Global::kFieldLSB);
    return id;
}

int CP::GeomId::TPC::GetModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kTPC 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::TPC::kGlobal 
        != id.GetField(CP::GeomId::Def::TPC::kSeqIdMSB, 
                       CP::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::TPC::Global::kTPC
        != id.GetField(CP::GeomId::Def::TPC::Global::kSeqIdMSB, 
                       CP::GeomId::Def::TPC::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::TPC::Global::kFieldMSB,
                       CP::GeomId::Def::TPC::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::TPC::TPC1() {return Module(0);}
CP::TGeometryId CP::GeomId::TPC::TPC2() {return Module(1);}
CP::TGeometryId CP::GeomId::TPC::TPC3() {return Module(2);}

bool CP::GeomId::TPC::IsTPC1(TGeometryId i) {
    return (CP::GeomId::TPC::GetModule(i) == 0);
}

bool CP::GeomId::TPC::IsTPC2(TGeometryId i) {
    return (CP::GeomId::TPC::GetModule(i) == 1);
}

bool CP::GeomId::TPC::IsTPC3(TGeometryId i) {
    return (CP::GeomId::TPC::GetModule(i) == 2);
}

CP::TGeometryId CP::GeomId::TPC::MicroMega(int tpc, int half, int mm) {
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
    id.SetField(CP::GeomId::Def::kTPC,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::TPC::kPad,
                CP::GeomId::Def::TPC::kSeqIdMSB,
                CP::GeomId::Def::TPC::kSeqIdLSB);
    id.SetField(tpc,
                CP::GeomId::Def::TPC::Pad::kTPCMSB,
                CP::GeomId::Def::TPC::Pad::kTPCLSB);
    id.SetField(half,
                CP::GeomId::Def::TPC::Pad::kHalfMSB,
                CP::GeomId::Def::TPC::Pad::kHalfLSB);
    id.SetField(mm,
                CP::GeomId::Def::TPC::Pad::kMMegaMSB,
                CP::GeomId::Def::TPC::Pad::kMMegaLSB);
    id.SetField(0,
                CP::GeomId::Def::TPC::Pad::kPadFlagMSB,
                CP::GeomId::Def::TPC::Pad::kPadFlagLSB);
    id.SetField(0,
                CP::GeomId::Def::TPC::Pad::kPadMSB,
                CP::GeomId::Def::TPC::Pad::kPadLSB);
    return id;
}

bool CP::GeomId::TPC::IsMicroMega(CP::TGeometryId i) {
    TSettableGeometryId id(i);
    int detector = id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                               CP::GeomId::Def::kDetectorIdLSB);
    if (detector != CP::GeomId::Def::kTPC) return false;
    /// This may be a pad.
    int seqId = id.GetField(CP::GeomId::Def::TPC::kSeqIdMSB,
                            CP::GeomId::Def::TPC::kSeqIdLSB);
    if (seqId != CP::GeomId::Def::TPC::kPad) return false;
    int padFlag = id.GetField(CP::GeomId::Def::TPC::Pad::kPadFlagMSB,
                              CP::GeomId::Def::TPC::Pad::kPadFlagLSB);
    if (padFlag) return false;
    return true;
}

CP::TGeometryId CP::GeomId::TPC::Pad(int tpc, int half, int mm, int pad) {
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
    id.SetField(CP::GeomId::Def::kTPC,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::TPC::kPad,
                CP::GeomId::Def::TPC::kSeqIdMSB,
                CP::GeomId::Def::TPC::kSeqIdLSB);
    id.SetField(tpc,
                CP::GeomId::Def::TPC::Pad::kTPCMSB,
                CP::GeomId::Def::TPC::Pad::kTPCLSB);
    id.SetField(half,
                CP::GeomId::Def::TPC::Pad::kHalfMSB,
                CP::GeomId::Def::TPC::Pad::kHalfLSB);
    id.SetField(mm,
                CP::GeomId::Def::TPC::Pad::kMMegaMSB,
                CP::GeomId::Def::TPC::Pad::kMMegaLSB);
    id.SetField(1,
                CP::GeomId::Def::TPC::Pad::kPadFlagMSB,
                CP::GeomId::Def::TPC::Pad::kPadFlagLSB);
    id.SetField(pad,
                CP::GeomId::Def::TPC::Pad::kPadMSB,
                CP::GeomId::Def::TPC::Pad::kPadLSB);
    return id;
}

int CP::GeomId::TPC::GetMicroMegaTPC(TGeometryId i) {
    return CP::GeomId::TPC::GetPadTPC(i);
}

int CP::GeomId::TPC::GetPadTPC(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kTPC 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::TPC::kPad 
        != id.GetField(CP::GeomId::Def::TPC::kSeqIdMSB, 
                       CP::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::TPC::Pad::kTPCMSB, 
                       CP::GeomId::Def::TPC::Pad::kTPCLSB);
}

int CP::GeomId::TPC::GetMicroMegaHalf(TGeometryId i) {
    return CP::GeomId::TPC::GetPadHalf(i);
}

int CP::GeomId::TPC::GetPadHalf(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kTPC 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::TPC::kPad 
        != id.GetField(CP::GeomId::Def::TPC::kSeqIdMSB, 
                       CP::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::TPC::Pad::kHalfMSB, 
                       CP::GeomId::Def::TPC::Pad::kHalfLSB);
}

int CP::GeomId::TPC::GetMicroMegaNumber(TGeometryId i) {
    return CP::GeomId::TPC::GetPadMicroMega(i);
}

int CP::GeomId::TPC::GetPadMicroMega(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kTPC 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::TPC::kPad 
        != id.GetField(CP::GeomId::Def::TPC::kSeqIdMSB, 
                       CP::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::TPC::Pad::kMMegaMSB, 
                       CP::GeomId::Def::TPC::Pad::kMMegaLSB);
}

int CP::GeomId::TPC::GetPadNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kTPC 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::TPC::kPad 
        != id.GetField(CP::GeomId::Def::TPC::kSeqIdMSB, 
                       CP::GeomId::Def::TPC::kSeqIdLSB)) return -1;
    if (1 != id.GetField(CP::GeomId::Def::TPC::Pad::kPadFlagMSB, 
                         CP::GeomId::Def::TPC::Pad::kPadFlagLSB)) return -1;
    return id.GetField(CP::GeomId::Def::TPC::Pad::kPadMSB, 
                       CP::GeomId::Def::TPC::Pad::kPadLSB);
}

bool CP::GeomId::TPC::IsPad(CP::TGeometryId i) {
    TSettableGeometryId id(i);
    int detector = id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                               CP::GeomId::Def::kDetectorIdLSB);
    if (detector != CP::GeomId::Def::kTPC) return false;
    /// This may be a pad.
    int seqId = id.GetField(CP::GeomId::Def::TPC::kSeqIdMSB,
                            CP::GeomId::Def::TPC::kSeqIdLSB);
    if (seqId != CP::GeomId::Def::TPC::kPad) return false;
    int padFlag = id.GetField(CP::GeomId::Def::TPC::Pad::kPadFlagMSB,
                              CP::GeomId::Def::TPC::Pad::kPadFlagLSB);
    if (!padFlag) return false;
    return true;
}

////////////////////////////////////////////////////////////
// FGD Geometry Identifiers.
////////////////////////////////////////////////////////////
bool CP::GeomId::FGD::IsFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kFGD
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::FGD::FGD(int fgd) {
    TSettableGeometryId id;
    if (fgd<0 || fgd>1) {
        ND280Warn("FGD out of range [0,1]: " << fgd);
        return id;
    }
    id.SetField(CP::GeomId::Def::kFGD,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::FGD::kGlobal,
                CP::GeomId::Def::FGD::kSeqIdMSB,
                CP::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::FGD::Global::kFGD,
                CP::GeomId::Def::FGD::Global::kSeqIdMSB,
                CP::GeomId::Def::FGD::Global::kSeqIdLSB);
    id.SetField(fgd,
                CP::GeomId::Def::FGD::Global::kFieldMSB,
                CP::GeomId::Def::FGD::Global::kFieldLSB);
    return id;
}

CP::TGeometryId CP::GeomId::FGD::FGD1() {return FGD(0);}
CP::TGeometryId CP::GeomId::FGD::FGD2() {return FGD(1);}

int CP::GeomId::FGD::GetFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kGlobal 
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::Global::kFGD
        != id.GetField(CP::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Global::kFieldMSB,
                       CP::GeomId::Def::FGD::Global::kFieldLSB);
}

bool CP::GeomId::FGD::IsFGD1(TGeometryId id) {
    return (CP::GeomId::FGD::GetFGD(id) == 0);
}

bool CP::GeomId::FGD::IsFGD2(TGeometryId id) {
    return (CP::GeomId::FGD::GetFGD(id) == 1);
}

CP::TGeometryId CP::GeomId::FGD::Bar(int fgd, 
                                     int module,
                                     int layer,
                                     int bar) {
    TSettableGeometryId id;
    if (fgd<0 || fgd>1) {
        ND280Warn("FGD out of range [0,1]: " << fgd);
        return id;
    }
    id.SetField(CP::GeomId::Def::kFGD,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::FGD::kScintillator,
                CP::GeomId::Def::FGD::kSeqIdMSB,
                CP::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(fgd,
                CP::GeomId::Def::FGD::Bar::kModuleMSB,
                CP::GeomId::Def::FGD::Bar::kModuleLSB);
    id.SetField(module,
                CP::GeomId::Def::FGD::Bar::kPlaneMSB,
                CP::GeomId::Def::FGD::Bar::kPlaneLSB);
    id.SetField(layer,
                CP::GeomId::Def::FGD::Bar::kLayerMSB,
                CP::GeomId::Def::FGD::Bar::kLayerLSB);
    id.SetField(bar,
                CP::GeomId::Def::FGD::Bar::kBarMSB,
                CP::GeomId::Def::FGD::Bar::kBarLSB);
    return id;
}

int CP::GeomId::FGD::GetBarFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kScintillator
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Bar::kModuleMSB,
                       CP::GeomId::Def::FGD::Bar::kModuleLSB);
}

int CP::GeomId::FGD::GetBarModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kScintillator
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Bar::kPlaneMSB,
                       CP::GeomId::Def::FGD::Bar::kPlaneLSB);
}

int CP::GeomId::FGD::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kScintillator
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Bar::kLayerMSB,
                       CP::GeomId::Def::FGD::Bar::kLayerLSB);
}

int CP::GeomId::FGD::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kScintillator
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Bar::kBarMSB,
                       CP::GeomId::Def::FGD::Bar::kBarLSB);
}

CP::TGeometryId CP::GeomId::FGD::Target(int target) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kFGD,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::FGD::kGlobal,
                CP::GeomId::Def::FGD::kSeqIdMSB, 
                CP::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::FGD::Global::kTarget,
                CP::GeomId::Def::FGD::Global::kSeqIdMSB,
                CP::GeomId::Def::FGD::Global::kSeqIdLSB);
    id.SetField(target,
                CP::GeomId::Def::FGD::Global::kFieldMSB,
                CP::GeomId::Def::FGD::Global::kFieldLSB);
    return id;
}

int CP::GeomId::FGD::GetTarget(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kGlobal 
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::Global::kTarget
        != id.GetField(CP::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Global::kFieldMSB,
                       CP::GeomId::Def::FGD::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::FGD::Layer(int fgd, 
                                       int module,
                                       int layer) {
    TSettableGeometryId id;
    if (fgd<0 || fgd>1) {
        ND280Warn("FGD out of range [0,1]: " << fgd);
        return id;
    }
    id.SetField(CP::GeomId::Def::kFGD,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::FGD::kGlobal,
                CP::GeomId::Def::FGD::kSeqIdMSB, 
                CP::GeomId::Def::FGD::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::FGD::Global::kLayer,
                CP::GeomId::Def::FGD::Global::kSeqIdMSB,
                CP::GeomId::Def::FGD::Global::kSeqIdLSB);
    id.SetField(fgd,
                CP::GeomId::Def::FGD::Global::Layer::kFGDMSB,
                CP::GeomId::Def::FGD::Global::Layer::kFGDLSB);
    id.SetField(layer,
                CP::GeomId::Def::FGD::Global::Layer::kLayerMSB,
                CP::GeomId::Def::FGD::Global::Layer::kLayerLSB);
    id.SetField(module,
                CP::GeomId::Def::FGD::Global::Layer::kModuleMSB,
                CP::GeomId::Def::FGD::Global::Layer::kModuleLSB);
    return id;
}

int CP::GeomId::FGD::GetLayerFGD(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kGlobal 
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::Global::kLayer
        != id.GetField(CP::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Global::Layer::kFGDMSB,
                       CP::GeomId::Def::FGD::Global::Layer::kFGDLSB);
}

int CP::GeomId::FGD::GetLayerModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kGlobal 
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::Global::kLayer
        != id.GetField(CP::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Global::Layer::kModuleMSB,
                       CP::GeomId::Def::FGD::Global::Layer::kModuleLSB);
}

int CP::GeomId::FGD::GetLayerNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kFGD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::kGlobal 
        != id.GetField(CP::GeomId::Def::FGD::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::FGD::Global::kLayer
        != id.GetField(CP::GeomId::Def::FGD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::FGD::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::FGD::Global::Layer::kLayerMSB,
                       CP::GeomId::Def::FGD::Global::Layer::kLayerLSB);
}

////////////////////////////////////////////////////////////
// ECal Geometry Identifiers.
////////////////////////////////////////////////////////////

CP::TGeometryId CP::GeomId::ECal::Module(int ecal, int clam, int module) {
    TSettableGeometryId id;
    if (ecal == CP::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kPECal) {
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
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::ECal::kGlobal,
                CP::GeomId::Def::ECal::kSeqIdMSB,
                CP::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                CP::GeomId::Def::ECal::kModuleMSB,
                CP::GeomId::Def::ECal::kModuleLSB);

    id.SetField(CP::GeomId::Def::ECal::Global::kECal,
                CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                CP::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::ECal::Global::kFieldMSB,
                CP::GeomId::Def::ECal::Global::kFieldLSB);

    return id;
}

CP::TGeometryId CP::GeomId::ECal::Container(int ecal, int clam, int module) {
    TSettableGeometryId id;
    if (ecal ==  CP::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal container with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal container with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kPECal) {
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
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::ECal::kGlobal,
                CP::GeomId::Def::ECal::kSeqIdMSB,
                CP::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                CP::GeomId::Def::ECal::kModuleMSB,
                CP::GeomId::Def::ECal::kModuleLSB);

    id.SetField(CP::GeomId::Def::ECal::Global::kContainer,
                CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                CP::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::ECal::Global::kFieldMSB,
                CP::GeomId::Def::ECal::Global::kFieldLSB);

    return id;
}
    

int CP::GeomId::ECal::GetModuleECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = CP::GeomId::GetSubSystem(id);
    if (det == CP::GeomId::Def::kDSECal) return det;
    else if (det == CP::GeomId::Def::kTECal) return det;
    else if (det == CP::GeomId::Def::kPECal) return det;
    else return -1;
}

int CP::GeomId::ECal::GetModuleNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetModuleECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kECal
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) / 2;
}

int CP::GeomId::ECal::GetModuleClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetModuleECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kECal
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) % 2;
}

CP::TGeometryId CP::GeomId::ECal::Layer(int ecal, int clam, 
                                        int module, int layer) {
    TSettableGeometryId id;
    if (ecal == CP::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kPECal) {
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
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::ECal::kGlobal,
                CP::GeomId::Def::ECal::kSeqIdMSB,
                CP::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                CP::GeomId::Def::ECal::kModuleMSB,
                CP::GeomId::Def::ECal::kModuleLSB);
    id.SetField(CP::GeomId::Def::ECal::Global::kLayer,
                CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                CP::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(layer,
                CP::GeomId::Def::ECal::Global::kFieldMSB,
                CP::GeomId::Def::ECal::Global::kFieldLSB);
    return id;
}

int CP::GeomId::ECal::GetLayerECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = CP::GeomId::GetSubSystem(id);
    if (det == CP::GeomId::Def::kDSECal) return det;
    else if (det == CP::GeomId::Def::kTECal) return det;
    else if (det == CP::GeomId::Def::kPECal) return det;
    else return -1;
}

int CP::GeomId::ECal::GetLayerClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetLayerECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kLayer
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) % 2;
}

int CP::GeomId::ECal::GetLayerModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetLayerECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kLayer
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) / 2;
}

int CP::GeomId::ECal::GetLayerNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetLayerECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kLayer
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::Global::kFieldMSB,
                       CP::GeomId::Def::ECal::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::ECal::Radiator(int ecal, int clam,
                                           int module, int rad) {
    TSettableGeometryId id;
    if (ecal == CP::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kPECal) {
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
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::ECal::kGlobal,
                CP::GeomId::Def::ECal::kSeqIdMSB,
                CP::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                CP::GeomId::Def::ECal::kModuleMSB,
                CP::GeomId::Def::ECal::kModuleLSB);
    id.SetField(CP::GeomId::Def::ECal::Global::kRadiator,
                CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                CP::GeomId::Def::ECal::Global::kSeqIdLSB);
    id.SetField(rad,
                CP::GeomId::Def::ECal::Global::kFieldMSB,
                CP::GeomId::Def::ECal::Global::kFieldLSB);
    return id;
}

int CP::GeomId::ECal::GetRadiatorECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = CP::GeomId::GetSubSystem(id);
    if (det == CP::GeomId::Def::kDSECal) return det;
    else if (det == CP::GeomId::Def::kTECal) return det;
    else if (det == CP::GeomId::Def::kPECal) return det;
    else return -1;
}

int CP::GeomId::ECal::GetRadiatorClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetRadiatorECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kRadiator
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) % 2;
}

int CP::GeomId::ECal::GetRadiatorModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetRadiatorECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kRadiator
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) / 2;
}

int CP::GeomId::ECal::GetRadiatorNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetRadiatorECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kGlobal
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::ECal::Global::kRadiator
        != id.GetField(CP::GeomId::Def::ECal::Global::kSeqIdMSB,
                       CP::GeomId::Def::ECal::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::Global::kFieldMSB,
                       CP::GeomId::Def::ECal::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::ECal::Bar(int ecal, int clam,
                                      int module, int layer, int bar) {
    TSettableGeometryId id;
    if (ecal == CP::GeomId::Def::kDSECal) {
        if (clam != 0) {
            ND280Error("Downstream ECal with invalid clam: " << clam);
            return id;
        }
        if (module != 0) {
            ND280Error("Downstream ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kTECal) {
        if (clam != 0 && clam != 1) {
            ND280Error("Tracker ECal with invalid clam: " << clam);
            return id;
        }
        if (module<0 || 2<module) {
            ND280Error("Tracker ECal with invalid module: " << module);
            return id;
        }
    }
    else if (ecal == CP::GeomId::Def::kPECal) {
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
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::ECal::kScintillator,
                CP::GeomId::Def::ECal::kSeqIdMSB,
                CP::GeomId::Def::ECal::kSeqIdLSB);
    id.SetField(module*2+clam,
                CP::GeomId::Def::ECal::kModuleMSB,
                CP::GeomId::Def::ECal::kModuleLSB);
    id.SetField(layer,
                CP::GeomId::Def::ECal::Bar::kLayerMSB,
                CP::GeomId::Def::ECal::Bar::kLayerLSB);
    id.SetField(bar,
                CP::GeomId::Def::ECal::Bar::kBarMSB,
                CP::GeomId::Def::ECal::Bar::kBarLSB);
    return id;
}

int CP::GeomId::ECal::GetBarECal(TGeometryId i) {
    TSettableGeometryId id(i);
    int det = CP::GeomId::GetSubSystem(id);
    if (det == CP::GeomId::Def::kDSECal) return det;
    else if (det == CP::GeomId::Def::kTECal) return det;
    else if (det == CP::GeomId::Def::kPECal) return det;
    else return -1;
}

int CP::GeomId::ECal::GetBarClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kScintillator
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) % 2;
}

int CP::GeomId::ECal::GetBarModule(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kScintillator
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::kModuleMSB,
                       CP::GeomId::Def::ECal::kModuleLSB) / 2;
}

int CP::GeomId::ECal::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kScintillator
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::Bar::kLayerMSB,
                       CP::GeomId::Def::ECal::Bar::kLayerLSB);
}

int CP::GeomId::ECal::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::ECal::GetBarECal(i)<0) return -1;
    if (CP::GeomId::Def::ECal::kScintillator
        != id.GetField(CP::GeomId::Def::ECal::kSeqIdMSB,
                       CP::GeomId::Def::ECal::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::ECal::Bar::kBarMSB,
                       CP::GeomId::Def::ECal::Bar::kBarLSB);
}

////////////////////////////////////////////////////////////
// Downstream ECal Geometry Identifiers.
////////////////////////////////////////////////////////////
bool CP::GeomId::DSECal::IsDSECal(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kDSECal
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::DSECal::Detector() {
    return CP::GeomId::ECal::Module(CP::GeomId::Def::kDSECal,0,0);
}

CP::TGeometryId CP::GeomId::DSECal::Layer(int layer) {
    return CP::GeomId::ECal::Layer(CP::GeomId::Def::kDSECal,0,0,layer);
}

int CP::GeomId::DSECal::GetLayer(TGeometryId i) {
    if (!CP::GeomId::DSECal::IsDSECal(i)) return -1;
    return CP::GeomId::ECal::GetLayerNumber(i);
}

CP::TGeometryId CP::GeomId::DSECal::Radiator(int rad) {
    return CP::GeomId::ECal::Radiator(CP::GeomId::Def::kDSECal,0,0,rad);
}

int CP::GeomId::DSECal::GetRadiator(TGeometryId i) {
    if (!CP::GeomId::DSECal::IsDSECal(i)) return -1;
    return CP::GeomId::ECal::GetRadiatorNumber(i);
}

CP::TGeometryId CP::GeomId::DSECal::Bar(int layer, int bar) {
    return CP::GeomId::ECal::Bar(CP::GeomId::Def::kDSECal,
                                 0,0,layer,bar);
}

int CP::GeomId::DSECal::GetBarLayer(TGeometryId i) {
    if (!CP::GeomId::DSECal::IsDSECal(i)) return -1;
    return CP::GeomId::ECal::GetBarLayer(i);
}

int CP::GeomId::DSECal::GetBarNumber(TGeometryId i) {
    if (!CP::GeomId::DSECal::IsDSECal(i)) return -1;
    return CP::GeomId::ECal::GetBarNumber(i);
}

////////////////////////////////////////////////////////////
// Tracker ECal Geometry Identifiers.
////////////////////////////////////////////////////////////
bool CP::GeomId::TECal::IsTECal(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kTECal
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::TECal::Module(int clam, int module) {
    return CP::GeomId::ECal::Module(CP::GeomId::Def::kTECal,
                                    clam,module);
}

CP::TGeometryId CP::GeomId::TECal::Layer(int clam, int module, int layer) {
    return CP::GeomId::ECal::Layer(CP::GeomId::Def::kTECal,
                                   clam,module,layer);
}

CP::TGeometryId CP::GeomId::TECal::Radiator(int clam, int module, int rad) {
    return CP::GeomId::ECal::Radiator(CP::GeomId::Def::kTECal,
                                      clam,module,rad);
}

CP::TGeometryId CP::GeomId::TECal::Bar(int clam, int mod, int layer, int bar) {
    return CP::GeomId::ECal::Bar(CP::GeomId::Def::kTECal,
                                      clam,mod,layer,bar);
}

int CP::GeomId::TECal::GetModuleNumber(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetModuleNumber(i);
}

int CP::GeomId::TECal::GetModuleClam(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetModuleClam(i);
}

int CP::GeomId::TECal::GetLayerNumber(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetLayerNumber(i);
}

int CP::GeomId::TECal::GetLayerClam(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetLayerClam(i);
}

int CP::GeomId::TECal::GetLayerModule(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetLayerModule(i);
}

int CP::GeomId::TECal::GetRadiatorNumber(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetRadiatorNumber(i);
}

int CP::GeomId::TECal::GetRadiatorClam(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetRadiatorClam(i);
}

int CP::GeomId::TECal::GetRadiatorModule(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetRadiatorModule(i);
}

int CP::GeomId::TECal::GetBarNumber(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetBarNumber(i);
}

int CP::GeomId::TECal::GetBarLayer(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetBarLayer(i);
}

int CP::GeomId::TECal::GetBarClam(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetBarClam(i);
}

int CP::GeomId::TECal::GetBarModule(TGeometryId i) {
    if (!CP::GeomId::TECal::IsTECal(i)) return -1;
    return CP::GeomId::ECal::GetBarModule(i);
}

////////////////////////////////////////////////////////////
// P0D ECal Geometry Identifiers.
////////////////////////////////////////////////////////////
bool CP::GeomId::PECal::IsPECal(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kPECal
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::PECal::Module(int clam, int module) {
    return CP::GeomId::ECal::Module(CP::GeomId::Def::kPECal,
                                    clam,module);
}

CP::TGeometryId CP::GeomId::PECal::Layer(int clam, int module, int layer) {
    return CP::GeomId::ECal::Layer(CP::GeomId::Def::kPECal,
                                   clam,module,layer);
}

CP::TGeometryId CP::GeomId::PECal::Radiator(int clam, int module, int rad) {
    return CP::GeomId::ECal::Radiator(CP::GeomId::Def::kPECal,
                                      clam,module,rad);
}

CP::TGeometryId CP::GeomId::PECal::Bar(int clam, int mod, int layer, int bar) {
    return CP::GeomId::ECal::Bar(CP::GeomId::Def::kPECal,
                                      clam,mod,layer,bar);
}

int CP::GeomId::PECal::GetModuleNumber(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetModuleNumber(i);
}

int CP::GeomId::PECal::GetModuleClam(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetModuleClam(i);
}

int CP::GeomId::PECal::GetLayerNumber(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetLayerNumber(i);
}

int CP::GeomId::PECal::GetLayerClam(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetLayerClam(i);
}

int CP::GeomId::PECal::GetLayerModule(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetLayerModule(i);
}

int CP::GeomId::PECal::GetRadiatorNumber(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetRadiatorNumber(i);
}

int CP::GeomId::PECal::GetRadiatorClam(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetRadiatorClam(i);
}

int CP::GeomId::PECal::GetRadiatorModule(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetRadiatorModule(i);
}

int CP::GeomId::PECal::GetBarNumber(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetBarNumber(i);
}

int CP::GeomId::PECal::GetBarLayer(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetBarLayer(i);
}

int CP::GeomId::PECal::GetBarClam(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetBarClam(i);
}

int CP::GeomId::PECal::GetBarModule(TGeometryId i) {
    if (!CP::GeomId::PECal::IsPECal(i)) return -1;
    return CP::GeomId::ECal::GetBarModule(i);
}

////////////////////////////////////////////////////////////
// SMRD Geometry Identifiers.
////////////////////////////////////////////////////////////
bool CP::GeomId::SMRD::IsSMRD(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kSMRD 
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::SMRD::Module(int clam, int yoke, 
                                         int layer, int slot) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kSMRD,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::SMRD::kGlobal, 
                CP::GeomId::Def::SMRD::kSeqIdMSB, 
                CP::GeomId::Def::SMRD::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::SMRD::Global::kModule,
                CP::GeomId::Def::SMRD::Global::kSeqIdMSB,
                CP::GeomId::Def::SMRD::Global::kSeqIdLSB);
    id.SetField(clam,
                CP::GeomId::Def::SMRD::Global::Module::kClamMSB,
                CP::GeomId::Def::SMRD::Global::Module::kClamLSB);
    id.SetField(yoke,
                CP::GeomId::Def::SMRD::Global::Module::kYokeMSB,
                CP::GeomId::Def::SMRD::Global::Module::kYokeLSB);
    id.SetField(layer,
                CP::GeomId::Def::SMRD::Global::Module::kLayerMSB,
                CP::GeomId::Def::SMRD::Global::Module::kLayerLSB);
    id.SetField(slot,
                CP::GeomId::Def::SMRD::Global::Module::kSlotMSB,
                CP::GeomId::Def::SMRD::Global::Module::kSlotLSB);
    return id;
}

int CP::GeomId::SMRD::GetModuleClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kGlobal
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::Global::kModule
        != id.GetField(CP::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    
    return id.GetField(CP::GeomId::Def::SMRD::Global::Module::kClamMSB, 
                       CP::GeomId::Def::SMRD::Global::Module::kClamLSB);
}

int CP::GeomId::SMRD::GetModuleYoke(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kGlobal
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::Global::kModule
        != id.GetField(CP::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    

    return id.GetField(CP::GeomId::Def::SMRD::Global::Module::kYokeMSB, 
                       CP::GeomId::Def::SMRD::Global::Module::kYokeLSB);
}

int CP::GeomId::SMRD::GetModuleLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kGlobal
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::Global::kModule
        != id.GetField(CP::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    

    return id.GetField(CP::GeomId::Def::SMRD::Global::Module::kLayerMSB, 
                       CP::GeomId::Def::SMRD::Global::Module::kLayerLSB);
}

int CP::GeomId::SMRD::GetModuleSlot(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kGlobal
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::Global::kModule
        != id.GetField(CP::GeomId::Def::SMRD::Global::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::Global::kSeqIdLSB)) return -1;
    

    return id.GetField(CP::GeomId::Def::SMRD::Global::Module::kSlotMSB, 
                       CP::GeomId::Def::SMRD::Global::Module::kSlotLSB);
}

CP::TGeometryId CP::GeomId::SMRD::Bar(int clam, int yoke, 
                                      int layer, int slot, int bar) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kSMRD,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::SMRD::kScintillator, 
                CP::GeomId::Def::SMRD::kSeqIdMSB, 
                CP::GeomId::Def::SMRD::kSeqIdLSB);
    id.SetField(clam,
                CP::GeomId::Def::SMRD::Bar::kClamMSB,
                CP::GeomId::Def::SMRD::Bar::kClamLSB);
    id.SetField(yoke,
                CP::GeomId::Def::SMRD::Bar::kYokeMSB,
                CP::GeomId::Def::SMRD::Bar::kYokeLSB);
    id.SetField(layer,
                CP::GeomId::Def::SMRD::Bar::kLayerMSB,
                CP::GeomId::Def::SMRD::Bar::kLayerLSB);
    id.SetField(slot,
                CP::GeomId::Def::SMRD::Bar::kSlotMSB,
                CP::GeomId::Def::SMRD::Bar::kSlotLSB);
    id.SetField(bar,
                CP::GeomId::Def::SMRD::Bar::kBarMSB,
                CP::GeomId::Def::SMRD::Bar::kBarLSB);
    return id;
}

int CP::GeomId::SMRD::GetBarClam(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kScintillator
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(CP::GeomId::Def::SMRD::Bar::kClamMSB, 
                       CP::GeomId::Def::SMRD::Bar::kClamLSB);
}

int CP::GeomId::SMRD::GetBarYoke(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kScintillator
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(CP::GeomId::Def::SMRD::Bar::kYokeMSB, 
                       CP::GeomId::Def::SMRD::Bar::kYokeLSB);
}

int CP::GeomId::SMRD::GetBarLayer(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kScintillator
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(CP::GeomId::Def::SMRD::Bar::kLayerMSB, 
                       CP::GeomId::Def::SMRD::Bar::kLayerLSB);
}

int CP::GeomId::SMRD::GetBarSlot(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kScintillator
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(CP::GeomId::Def::SMRD::Bar::kSlotMSB, 
                       CP::GeomId::Def::SMRD::Bar::kSlotLSB);
}

int CP::GeomId::SMRD::GetBarNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kSMRD 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::SMRD::kScintillator
        != id.GetField(CP::GeomId::Def::SMRD::kSeqIdMSB, 
                       CP::GeomId::Def::SMRD::kSeqIdLSB)) return -1;

    return id.GetField(CP::GeomId::Def::SMRD::Bar::kBarMSB, 
                       CP::GeomId::Def::SMRD::Bar::kBarLSB);
}

////////////////////////////////////////////////////////////
// INGRID Geometry Identifiers.
////////////////////////////////////////////////////////////

CP::TGeometryId CP::GeomId::INGRID::Scintillator(
    int objType, int obj, int trk, int proj, int scinti){
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kINGRID,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(objType,
                CP::GeomId::Def::INGRID::kIngridObjIDMSB,
                CP::GeomId::Def::INGRID::kIngridObjIDLSB);
    id.SetField(obj,
                CP::GeomId::Def::INGRID::kIngridModNumMSB,
                CP::GeomId::Def::INGRID::kIngridModNumLSB);
    id.SetField(trk,
                CP::GeomId::Def::INGRID::kIngridTrkNumMSB,
                CP::GeomId::Def::INGRID::kIngridTrkNumLSB);
    id.SetField(proj,
                CP::GeomId::Def::INGRID::kIngridProjMSB,
                CP::GeomId::Def::INGRID::kIngridProjLSB);
    id.SetField(scinti,
                CP::GeomId::Def::INGRID::kIngridSciNumMSB,
                CP::GeomId::Def::INGRID::kIngridSciNumLSB);
    return id;
}

CP::TGeometryId CP::GeomId::INGRID::ModScintillator(
    int obj, int trk, int proj, int scinti){
    return CP::GeomId::INGRID::Scintillator(
        CP::GeomId::Def::INGRID::kIngridModule, obj, trk, proj, scinti);
}

CP::TGeometryId CP::GeomId::INGRID::VertVetoScintillator(int obj, int scinti){
    return CP::GeomId::INGRID::Scintillator(
        CP::GeomId::Def::INGRID::kIngridVeto, obj, 0,
        CP::GeomId::Def::INGRID::kVertical, scinti);
}

CP::TGeometryId CP::GeomId::INGRID::HorzVetoScintillator(int obj, int scinti){
    return CP::GeomId::INGRID::Scintillator(
        CP::GeomId::Def::INGRID::kIngridVeto, obj, 0,
        CP::GeomId::Def::INGRID::kHorizontal, scinti);
}
