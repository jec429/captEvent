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
// Captain Geometry Identifiers.
////////////////////////////////////////////////////////////

bool CP::GeomId::Captain::IsCryostat(TGeometryId i) {
    TSettableGeometryId id(i);
    return (CP::GeomId::Def::kCryostat 
            == id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                           CP::GeomId::Def::kDetectorIdLSB));
}

CP::TGeometryId CP::GeomId::Captain::Detector() {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Captain::kGlobal,
                CP::GeomId::Def::Captain::kSeqIdMSB,
                CP::GeomId::Def::Captain::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::Captain::Global::kCryostat,
                CP::GeomId::Def::Captain::Global::kSeqIdMSB,
                CP::GeomId::Def::Captain::Global::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::Captain::Global::kFieldMSB,
                CP::GeomId::Def::Captain::Global::kFieldLSB);
    return id;
}

CP::TGeometryId CP::GeomId::Captain::Drift() {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Captain::kGlobal, 
                CP::GeomId::Def::Captain::kSeqIdMSB, 
                CP::GeomId::Def::Captain::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::Captain::Global::kDriftRegion,
                CP::GeomId::Def::Captain::Global::kSeqIdMSB,
                CP::GeomId::Def::Captain::Global::kSeqIdLSB);
    id.SetField(0,
                CP::GeomId::Def::Captain::Global::kFieldMSB,
                CP::GeomId::Def::Captain::Global::kFieldLSB);
    return id;
}

CP::TGeometryId CP::GeomId::Captain::Plane(int plane) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB, 
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Captain::kGlobal, 
                CP::GeomId::Def::Captain::kSeqIdMSB,
                CP::GeomId::Def::Captain::kSeqIdLSB);
    id.SetField(CP::GeomId::Def::Captain::Global::kWirePlane,
                CP::GeomId::Def::Captain::Global::kSeqIdMSB,
                CP::GeomId::Def::Captain::Global::kSeqIdLSB);
    id.SetField(plane,
                CP::GeomId::Def::Captain::Global::kFieldMSB,
                CP::GeomId::Def::Captain::Global::kFieldLSB);
    return id;
}

int CP::GeomId::Captain::GetPlane(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kCryostat
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::Captain::kGlobal 
        != id.GetField(CP::GeomId::Def::Captain::kSeqIdMSB, 
                       CP::GeomId::Def::Captain::kSeqIdLSB)) return -1;
    if (CP::GeomId::Def::Captain::Global::kWirePlane 
        != id.GetField(CP::GeomId::Def::Captain::Global::kSeqIdMSB, 
                       CP::GeomId::Def::Captain::Global::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::Captain::Global::kFieldMSB,
                       CP::GeomId::Def::Captain::Global::kFieldLSB);
}

CP::TGeometryId CP::GeomId::Captain::Wire(int plane, 
                                       int wire) {
    TSettableGeometryId id;
    id.SetField(CP::GeomId::Def::kCryostat,
                CP::GeomId::Def::kDetectorIdMSB,
                CP::GeomId::Def::kDetectorIdLSB);
    id.SetField(CP::GeomId::Def::Captain::kWire,
                CP::GeomId::Def::Captain::kSeqIdMSB,
                CP::GeomId::Def::Captain::kSeqIdLSB);
    id.SetField(plane,
                CP::GeomId::Def::Captain::Wire::kPlaneMSB,
                CP::GeomId::Def::Captain::Wire::kPlaneLSB);
    id.SetField(wire,
                CP::GeomId::Def::Captain::Wire::kWireMSB,
                CP::GeomId::Def::Captain::Wire::kWireLSB);
    return id;
}

int CP::GeomId::Captain::GetWirePlane(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kCryostat 
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::Captain::kWire 
        != id.GetField(CP::GeomId::Def::Captain::kSeqIdMSB, 
                       CP::GeomId::Def::Captain::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::Captain::Wire::kPlaneMSB,
                       CP::GeomId::Def::Captain::Wire::kPlaneLSB);
}

int CP::GeomId::Captain::GetWireNumber(TGeometryId i) {
    TSettableGeometryId id(i);
    if (CP::GeomId::Def::kCryostat
        != id.GetField(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB)) return -1;
    if (CP::GeomId::Def::Captain::kWire 
        != id.GetField(CP::GeomId::Def::Captain::kSeqIdMSB, 
                       CP::GeomId::Def::Captain::kSeqIdLSB)) return -1;
    return id.GetField(CP::GeomId::Def::Captain::Wire::kWireMSB,
                       CP::GeomId::Def::Captain::Wire::kWireLSB);
}

