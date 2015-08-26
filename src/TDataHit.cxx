////////////////////////////////////////////////////////////
// $Id: TDataHit.cxx,v 1.11 2010/07/07 17:35:44 mcgrew Exp $
//

#include "TDataHit.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TDataHit);
ClassImp(CP::TWritableDataHit);

CP::TDataHit::TDataHit() 
    : fChannelId(0) {}

CP::TDataHit::TDataHit(const CP::TWritableDataHit& h) 
    : CP::TSingleHit(h), fProxy(h.fProxy), fChannelId(h.fChannelId) {}

CP::TDataHit::~TDataHit() { }

int CP::TDataHit::GetDigitCount() const {
    if (fProxy.IsValid()) return 1;
    else return 0;
}

const CP::TDigitProxy& CP::TDataHit::GetDigit(int i) const {
    if (i!=0) throw CP::EHitOutOfRange();
    if (!fProxy.IsValid()) throw CP::EHitOutOfRange();
    return fProxy;
}

int CP::TDataHit::GetChannelIdCount() const {
    return 1;
}

CP::TChannelId CP::TDataHit::GetChannelId(int i) const {
    return TChannelId(fChannelId);
}

CP::TWritableDataHit::TWritableDataHit() {}
CP::TWritableDataHit::TWritableDataHit(const CP::TWritableDataHit& h) 
  : CP::TDataHit(h) {}
CP::TWritableDataHit::~TWritableDataHit() {}

//////////////////////////////////////////////////
// Setter methods for CP::TWritableDataHit
//////////////////////////////////////////////////

void CP::TWritableDataHit::SetGeomId(CP::TGeometryId id) {
    fGeomId = id.AsUInt();
}

void CP::TWritableDataHit::SetCharge(double q) {fCharge = q;}

void CP::TWritableDataHit::SetChargeUncertainty(double q) {
    fChargeUncertainty = q;
}

void CP::TWritableDataHit::SetTime(double t) {fTime = t;}

void CP::TWritableDataHit::SetTimeUncertainty(double t) {fTimeUncertainty = t;}

void CP::TWritableDataHit::SetTimeRMS(double t) {fTimeRMS = t;}

void CP::TWritableDataHit::SetTimeStart(double t) {fTimeStart = t;}

void CP::TWritableDataHit::SetTimeStop(double t) {fTimeStop = t;}

void CP::TWritableDataHit::SetDigit(CP::TDigitProxy proxy) {
    fProxy = proxy;
    CaptNamedDebug("dataHit","Set digit for " << proxy.AsString()
                   << " " << fProxy.AsString() << " " << fProxy.IsValid());
    if (fProxy.IsValid()) {
        try {
            CP::TDigit* digit = *fProxy;
            if (digit) {
                if (fChannelId == 0) {
                    fChannelId = digit->GetChannelId().AsUInt();
                }
                else if (fChannelId != digit->GetChannelId().AsUInt()
                         && digit->GetChannelId().AsUInt() != 0) {
                    CaptError("Hit channel id disagrees with digit proxy");
                }
            }
        }
        catch (...) {}
    }
    else if (fChannelId == 0) {
        CaptError("Invalid digit used to create hit. ("
                  << proxy.AsString() << ")"
                   "  Use SetChannelId to set channel before SetDigit.");
    }
}

void CP::TWritableDataHit::SetChannelId(CP::TChannelId id) {
    fChannelId = id.AsUInt();
}

void CP::TWritableDataHit::SetChargeValidity(bool valid) {
    CP::THit::SetChargeValidity(valid);
}

void CP::TWritableDataHit::SetTimeValidity(bool valid) {
    CP::THit::SetTimeValidity(valid);
}
