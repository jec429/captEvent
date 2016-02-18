// $Id: TGeometryId.cxx,v 1.12 2012/02/22 03:15:09 mcgrew Exp $

#include "TCaptLog.hxx"
#include "TGeometryId.hxx"
#include "CaptGeomIdDef.hxx"
#include "TManager.hxx"
#include "TGeomIdManager.hxx"

#include <sstream>

#include <exception>

ClassImp(CP::TGeometryId);

CP::TGeometryId::TGeometryId() : fGeometryId(CP::GeomId::Def::kEmptyId) {}

CP::TGeometryId::TGeometryId(int i) : fGeometryId(i) {}

CP::TGeometryId::TGeometryId(const TGeometryId& geom) 
    : fGeometryId(geom.fGeometryId) {}

CP::TGeometryId::~TGeometryId() {}

void CP::TGeometryId::SetFieldSafe(int val, int msb, int lsb) {
    // Make sure the bit range is valid.
    if (msb<lsb || msb>31 || lsb <0) {
        CaptSevere("Invalid bit field range -- MSB: " << msb
                   << " LSB: " << lsb);
        throw EGeomIdMSBLSB();
    }
    // Make sure val is in range.
    int maxValue = (1 << (msb-lsb+1));
    if (val >= maxValue) {
        CaptSevere("Value out of range " << val 
                   << " Bits in field " << msb-lsb+1 
                   << " Maximum value " << maxValue);
        throw EGeomIdOutOfRange();
    }
    if (val < 0) {
        CaptSevere("Value out of range " << val 
                   << " -- Negative values are not allowed");
        throw EGeomIdOutOfRange();
    }
    // Build a mask for this field
    long mask = ((1<<(msb-lsb+1))-1) << lsb;
    // Clear the bit field.
    fGeometryId &= ~mask;
    // Shift the value and set the field.
    fGeometryId |= ((val << lsb)&mask);
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
    // If it's negative, that means the guard bit is set and this is probably
    // a channel id.
    if (fGeometryId < 0) return false;
    
    // If it's zero (kEmptyId), that means it's probably uninitialized.
    if (fGeometryId == CP::GeomId::Def::kEmptyId) return false;

    int field = fGeometryId >> CP::GeomId::Def::kDetectorIdLSB;
    int mask = (1<<(CP::GeomId::Def::kDetectorIdMSB
                    - CP::GeomId::Def::kDetectorIdLSB+1))-1;
    int sys = (field & mask);

    // Make sure that the detector id field has a valid detector.  This use to
    // include the possiblity of using raw ROOT node identifiers, but that
    // capability has been removed.
    switch (sys) {
    case CP::GeomId::Def::kCryostat:
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
    int maxlen = s.width();
    if (maxlen < 1) maxlen=4096; // Any big number will work.
    try {
        CP::TManager::Get().Geometry();
        std::string name = id.GetName();
        int len = name.size();
        // chop of the front of the path until the length is OK.
        while (len > maxlen) {
             std::size_t pos = name.find_first_of("/");
             if (pos == std::string::npos) break;
             pos = name.find_first_of("/",pos+1);
             if (pos == std::string::npos) break;
             name = ".../" + name.substr(pos+1);
             len = name.size();
        }
        if (len > maxlen) {
            throw std::runtime_error("Geometry path name too long");
        }
        s << name;
    }
    catch (...) {
        s << id.AsInt();
    };

    return s;
}
