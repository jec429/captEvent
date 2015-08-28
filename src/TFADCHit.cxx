////////////////////////////////////////////////////////////
#include "TFADCHit.hxx"
#include "HEPUnits.hxx"

ClassImp(CP::TFADCHit);
ClassImp(CP::TWritableFADCHit);

CP::TFADCHit::TFADCHit() 
    : fChannelId(0) {}

CP::TFADCHit::TFADCHit(const CP::TWritableFADCHit& h) 
    : CP::TPulseHit(h), fProxy(h.fProxy), fChannelId(h.fChannelId) {}

CP::TFADCHit::~TFADCHit() { }

int CP::TFADCHit::GetDigitCount() const {
    if (fProxy.IsValid()) return 1;
    else return 0;
}

const CP::TDigitProxy& CP::TFADCHit::GetDigit(int i) const {
    if (i!=0) throw CP::EHitOutOfRange();
    if (!fProxy.IsValid()) throw CP::EHitOutOfRange();
    return fProxy;
}

int CP::TFADCHit::GetChannelIdCount() const {
    return 1;
}

CP::TChannelId CP::TFADCHit::GetChannelId(int i) const {
    return TChannelId(fChannelId);
}

CP::TWritableFADCHit::TWritableFADCHit() {}
CP::TWritableFADCHit::TWritableFADCHit(const CP::TWritableFADCHit& h) 
  : CP::TFADCHit(h) {}
CP::TWritableFADCHit::~TWritableFADCHit() {}

//////////////////////////////////////////////////
// Setter methods for CP::TWritableFADCHit
//////////////////////////////////////////////////

void CP::TWritableFADCHit::SetGeomId(CP::TGeometryId id) {
    fGeomId = id.AsUInt();
}

void CP::TWritableFADCHit::SetCharge(double q) {fCharge = q;}

void CP::TWritableFADCHit::SetChargeUncertainty(double q) {
    fChargeUncertainty = q;
}

void CP::TWritableFADCHit::SetTime(double t) {fTime = t;}

void CP::TWritableFADCHit::SetTimeUncertainty(double t) {fTimeUncertainty = t;}

void CP::TWritableFADCHit::SetTimeRMS(double t) {fTimeRMS = t;}

void CP::TWritableFADCHit::SetTimeStart(double t) {fTimeStart = t;}

void CP::TWritableFADCHit::SetTimeStop(double t) {fTimeStop = t;}

void CP::TWritableFADCHit::SetDigit(CP::TDigitProxy proxy) {
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

void CP::TWritableFADCHit::SetChannelId(CP::TChannelId id) {
    fChannelId = id.AsUInt();
}

void CP::TWritableFADCHit::SetChargeValidity(bool valid) {
    CP::THit::SetChargeValidity(valid);
}

void CP::TWritableFADCHit::SetTimeValidity(bool valid) {
    CP::THit::SetTimeValidity(valid);
}
