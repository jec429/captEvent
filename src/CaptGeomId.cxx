#include "TCaptLog.hxx"
#include "TGeometryId.hxx"
#include "CaptGeomId.hxx"
#include "CaptGeomIdDef.hxx"

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
// Cryo Geometry Identifiers.
////////////////////////////////////////////////////////////

bool CP::GeomId::Cryo::IsCryostat(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kCryostat 
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::Cryo::Detector() {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Cryo::kGlobal,
                CP::GeomId::Def::Cryo::kSeqIdMSB,
                CP::GeomId::Def::Cryo::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::Cryo::Global::kCryostat,
                CP::GeomId::Def::Cryo::Global::kSeqIdMSB,
                CP::GeomId::Def::Cryo::Global::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::Cryo::Global::kFieldMSB,
                CP::GeomId::Def::Cryo::Global::kFieldLSB);
    return id;
}

CP::TGeometryId CP::GeomId::Cryo::Drift() {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Cryo::kGlobal, 
                CP::GeomId::Def::Cryo::kSeqIdMSB, 
                CP::GeomId::Def::Cryo::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::Cryo::Global::kDriftRegion,
                CP::GeomId::Def::Cryo::Global::kSeqIdMSB,
                CP::GeomId::Def::Cryo::Global::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::Cryo::Global::kFieldMSB,
                CP::GeomId::Def::Cryo::Global::kFieldLSB);
    return id;
}

CP::TGeometryId CP::GeomId::Cryo::Plane(int plane) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Cryo::kGlobal, 
                CP::GeomId::Def::Cryo::kSeqIdMSB,
                CP::GeomId::Def::Cryo::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::Cryo::Global::kWirePlane,
                CP::GeomId::Def::Cryo::Global::kSeqIdMSB,
                CP::GeomId::Def::Cryo::Global::kSeqIdLSB);
    id.SetField(plane,
                CP::GeomId::Def::Cryo::Global::kFieldMSB,
                CP::GeomId::Def::Cryo::Global::kFieldLSB);
    return id;
}

int CP::GeomId::Cryo::GetPlane(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kCryostat
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::Cryo::kGlobal 
        != id.GetField(CP::GeomId::Def::Cryo::kSeqIdMSB, 
                       CP::GeomId::Def::Cryo::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::Cryo::Global::kWirePlane 
        != id.GetField(CP::GeomId::Def::Cryo::Global::kSeqIdMSB, 
                       CP::GeomId::Def::Cryo::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::Cryo::Global::kFieldMSB,
                       CP::GeomId::Def::Cryo::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::Cryo::Wire(int plane, 
                                       int wire) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Cryo::kWire,
                CP::GeomId::Def::Cryo::kSeqIdMSB,
                CP::GeomId::Def::Cryo::kSeqIdLSB);
    id.SetField(plane,
                CP::GeomId::Def::Cryo::Wire::kPlaneMSB,
                CP::GeomId::Def::Cryo::Wire::kPlaneLSB);
    id.SetField(wire,
                CP::GeomId::Def::Cryo::Wire::kWireMSB,
                CP::GeomId::Def::Cryo::Wire::kWireLSB);
    return id;
}

int CP::GeomId::Cryo::GetWirePlane(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kCryostat 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::Cryo::kWire 
        != id.GetField(CP::GeomId::Def::Cryo::kSeqIdMSB, 
                       CP::GeomId::Def::Cryo::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::Cryo::Wire::kPlaneMSB,
                       CP::GeomId::Def::Cryo::Wire::kPlaneLSB);
}

int CP::GeomId::Cryo::GetWireNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kCryostat
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::Cryo::kWire 
        != id.GetField(CP::GeomId::Def::Cryo::kSeqIdMSB, 
                       CP::GeomId::Def::Cryo::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::Cryo::Wire::kWireMSB,
                       CP::GeomId::Def::Cryo::Wire::kWireLSB);
}

