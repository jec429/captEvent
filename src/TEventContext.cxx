#include "TEventContext.hxx"
#include <ctime>

const UInt_t CP::TEventContext::Invalid = 0xDEADBEEF;

ClassImp(CP::TEventContext);

CP::TEventContext::~TEventContext() {}

CP::TEventContext::TEventContext()
    : fPartition(Invalid), 
      fRun(Invalid), fSubRun(Invalid), fEvent(Invalid), 
      fSpill(Invalid), fTimeStamp(Invalid) {}

CP::TEventContext::TEventContext(int partition, 
                                 int run, int subRun, int event, 
                                 int spill, int stamp)
    : fPartition(partition), 
      fRun(run), fSubRun(subRun), fEvent(event), 
      fSpill(spill), fTimeStamp(stamp) {}

UInt_t CP::TEventContext::GetPartition() const {return fPartition;}
UInt_t CP::TEventContext::GetRun() const {return fRun;}
UInt_t CP::TEventContext::GetSubRun() const {return fSubRun;}
UInt_t CP::TEventContext::GetEvent() const {return fEvent;}
UInt_t CP::TEventContext::GetSpill() const {return fSpill;}
CP::TEventContext::Time CP::TEventContext::GetTimeStamp() const {
    return fTimeStamp;}
std::string CP::TEventContext::GetTimeStampString() const {
    std::time_t t = GetTimeStamp();
    char buf[256];
    std::strftime(buf,sizeof(buf),"'%Y-%m-%d %H:%M:%S'",std::gmtime(&t));
    return std::string(buf);
}

void CP::TEventContext::SetPartition(UInt_t v) {fPartition = v;}
void CP::TEventContext::SetRun(int v) {fRun = v;}
void CP::TEventContext::SetSubRun(int v) {fSubRun = v;}
void CP::TEventContext::SetEvent(int v) {fEvent = v;}
void CP::TEventContext::SetSpill(int v) {fSpill = v;}
void CP::TEventContext::SetTimeStamp(int v) {fTimeStamp = v;}

bool CP::TEventContext::IsMC() const {
    if (fPartition == Invalid) return false;
    if (fPartition & kMCData) return true;
    return false;
}

bool CP::TEventContext::IsDetector() const {
    if (fPartition == Invalid) return false;
    if (fPartition & kMCData) return false;
    return true;
}

bool CP::TEventContext::IsValid() const {
    if (GetPartition()!=CP::TEventContext::Invalid) return true;
    if (GetRun()!=CP::TEventContext::Invalid)  return true;
    if (GetSubRun()!=CP::TEventContext::Invalid) return true;
    if (GetEvent()!=CP::TEventContext::Invalid) return true;
    if (GetSpill()!=CP::TEventContext::Invalid) return true;
    if (GetTimeStamp()!=CP::TEventContext::Invalid) return true;
    return false;
}

bool CP::TEventContext::operator!= (const CP::TEventContext& rhs) const {
    return not (*this == rhs);
}

bool CP::TEventContext::operator== (const CP::TEventContext& rhs) const {
    if (!IsValid()) return false;
    if (!rhs.IsValid()) return false;

    if (GetPartition()!=CP::TEventContext::Invalid
        || rhs.GetPartition()!=CP::TEventContext::Invalid) {
        if (GetPartition()!=rhs.GetPartition()) return false;
    }

    if (GetRun()!=CP::TEventContext::Invalid
        || rhs.GetRun()!=CP::TEventContext::Invalid) {
        if (GetRun()!=rhs.GetRun()) return false;
    }

    if (GetSubRun()!=CP::TEventContext::Invalid
        || rhs.GetSubRun()!=CP::TEventContext::Invalid) {
        if (GetSubRun()!=rhs.GetSubRun()) return false;
    }

    if (GetEvent()!=CP::TEventContext::Invalid
        || rhs.GetEvent()!=CP::TEventContext::Invalid) {
        if (GetEvent()!=rhs.GetEvent()) return false;
    }

    if (GetSpill()!=CP::TEventContext::Invalid
        || rhs.GetSpill()!=CP::TEventContext::Invalid) {
        if (GetSpill()!=rhs.GetSpill()) return false;
    }

    if (GetTimeStamp()!=CP::TEventContext::Invalid
        || rhs.GetTimeStamp()!=CP::TEventContext::Invalid) {
        if (GetTimeStamp()!=rhs.GetTimeStamp()) return false;
    }

    return true;
}

std::ostream& CP::operator<<(std::ostream& s, const CP::TEventContext& c) {
    s << "<";
    bool valid = false;
    if (c.GetPartition()!=CP::TEventContext::Invalid) {
        s << "P: " << std::hex << "0x" << c.GetPartition() << std::dec;
        valid = true;
    }
    if (c.GetRun()!=CP::TEventContext::Invalid) {
        if (valid) s << " ";
        s << "R: " << c.GetRun();
        valid = true;
    }
    if (c.GetSubRun()!=CP::TEventContext::Invalid) {
        if (valid) s << " ";
        s << "SR: " << c.GetSubRun();
        valid = true;
    }
    if (c.GetEvent()!=CP::TEventContext::Invalid) {
        if (valid) s << " ";
        s << "E: " << c.GetEvent();
        valid = true;
    }
    if (c.GetSpill()!=CP::TEventContext::Invalid) {
        if (valid) s << " ";
        s << "S: " << c.GetSpill();
        valid = true;
    }
    if (c.GetTimeStamp()!=CP::TEventContext::Invalid) {
        if (valid) s << " ";
        std::time_t t = c.GetTimeStamp();
        char buf[64];
        std::strftime(buf,sizeof(buf),"%y/%m/%d %X %Z",std::gmtime(&t));
        s << "T: " << buf;
        valid = true;
    }
    if (valid) {
        if (c.IsMC()) s << " MC";
        else if (c.IsDetector()) s << " DET";
    }
    else {
        s << "Invalid Context";
    }
    s << ">";
    return s;
}
