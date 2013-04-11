// $Id: TGeometryId.cxx,v 1.12 2012/02/22 03:15:09 mcgrew Exp $

#include <sstream>
#include "TCaptLog.hxx"
#include "TGeometryId.hxx"
#include "CaptGeomIdDef.hxx"
#include "TManager.hxx"
#include "TGeomIdManager.hxx"

ClassImp(CP::TGeometryId);

CP::TGeometryId::TGeometryId() : fGeometryId(CP::GeomId::Def::kEmptyId) {}

CP::TGeometryId::TGeometryId(int i) : fGeometryId(i) {}

CP::TGeometryId::TGeometryId(const TGeometryId& geom) 
    : fGeometryId(geom.fGeometryId) {}

CP::TGeometryId::~TGeometryId() {}

void CP::TGeometryId::SetFieldSafe(int val, int msb, int lsb) {
    // Make sure the bit range is valid.
    if (msb<lsb || msb>31 || lsb <0) {
        CaptError("Invalid bit field range -- MSB: " << msb
                   << " LSB: " << lsb);
        throw EGeomIdMSBLSB();
    }
    // Make sure val is in range.
    int maxValue = (1 << (msb-lsb+1));
    if (val >= maxValue) {
        CaptError("Value out of range " << val 
                   << " Bits in field " << msb-lsb+1 
                   << " Maximum value " << maxValue);
        throw EGeomIdOutOfRange();
    }
    if (val < 0) {
        CaptError("Value out of range " << val 
                   << " -- Negative values are not allowed");
        throw EGeomIdOutOfRange();
    }
    // Build a mask for this field
    long mask = ((1<<(msb-lsb+1))-1) << lsb;
    // Clear the bit field.
    fGeometryId &= ~mask;
    // Shift the value and set the field.
    fGeometryId |= ((val << lsb)&mask);
    if (!IsValid()) {
        CaptError("Invalid id: " << fGeometryId);
        throw EGeomIdInvalid();
    }
}

int CP::TGeometryId::GetFieldSafe(int msb, int lsb) const {
    if (msb<lsb || msb>31 || lsb <0) throw EGeomIdMSBLSB();
    int field = fGeometryId >> lsb;
    int mask = (1<<(msb-lsb+1))-1;
    return (field & mask);
}

int CP::TGeometryId::GetSubsystemId() const {
    return GetFieldSafe(CP::GeomId::Def::kDetectorIdMSB,
                    CP::GeomId::Def::kDetectorIdLSB);
}

const bool CP::TGeometryId::IsValid() const {
    if (fGeometryId < 0) return false;

    int field = fGeometryId >> CP::GeomId::Def::kDetectorIdLSB;
    int mask = (1<<(CP::GeomId::Def::kDetectorIdMSB
                    - CP::GeomId::Def::kDetectorIdLSB+1))-1;
    int sys = (field & mask);

    switch (sys) {
    case CP::GeomId::Def::kCryostat:
    case CP::GeomId::Def::kROOTGeoNodeId:
        return true;
    default:
        return false;
    }

    return false;
}

std::string CP::TGeometryId::GetName() const {
    return TManager::Get().GeomId().GetPath(*this);
}

std::string CP::TGeometryId::GetSubsystemName() const {
    int sys = GetFieldSafe(CP::GeomId::Def::kDetectorIdMSB,
                       CP::GeomId::Def::kDetectorIdLSB);

    switch (sys) {
    case CP::GeomId::Def::kCryostat: return "Cryostat";
    case CP::GeomId::Def::kROOTGeoNodeId: return "node";
    }

    return "unknown";
}

TVector3 CP::TGeometryId::GetPosition() const {
    TVector3 position(0,0,0);
    if (!TManager::Get().GeomId().GetPosition(*this,position)) {
        CaptWarn("Position for invalid geometry id was requested");
        throw CP::EGeomIdInvalid();
    }
    return position;
}

bool CP::operator==(const CP::TGeometryId& a, const CP::TGeometryId& b) {
    return (a.AsInt() == b.AsInt());
}

bool CP::operator!=(const CP::TGeometryId& a, const CP::TGeometryId& b) {
    return (a.AsInt() != b.AsInt());
}

std::ostream& CP::operator<<(std::ostream& s, const CP::TGeometryId& id) {
    try {
        CP::TManager::Get().Geometry();
        std::string name = id.GetName();
        int len = name.size();
        int maxlen = 50;
        if (len > maxlen) name = "..." + name.substr(name.size()-maxlen);
        s << name;
    }
    catch (...) {
        s << id.AsInt();
    };

    return s;
}
