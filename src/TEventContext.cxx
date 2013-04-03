#include "TEventContext.hxx"
#include <ctime>

const int CP::TEventContext::Invalid = 0xDEADBEEF;

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

int CP::TEventContext::GetPartition() const {return fPartition;}
int CP::TEventContext::GetRun() const {return fRun;}
int CP::TEventContext::GetSubRun() const {return fSubRun;}
int CP::TEventContext::GetEvent() const {return fEvent;}
int CP::TEventContext::GetSpill() const {return fSpill;}
int CP::TEventContext::GetTimeStamp() const {return fTimeStamp;}
std::string CP::TEventContext::GetTimeStampString() const {
    std::time_t t = GetTimeStamp();
    char buf[256];
    std::strftime(buf,sizeof(buf),"'%Y-%m-%d %H:%M:%S'",std::gmtime(&t));
    return std::string(buf);
}

void CP::TEventContext::SetPartition(int v) {fPartition = v;}
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

bool CP::TEventContext::Valid() const {
    if (GetPartition()!=CP::TEventContext::Invalid) return true;
    if (GetRun()!=CP::TEventContext::Invalid)  return true;
    if (GetSubRun()!=CP::TEventContext::Invalid) return true;
    if (GetEvent()!=CP::TEventContext::Invalid) return true;
    if (GetSpill()!=CP::TEventContext::Invalid) return true;
    if (GetTimeStamp()!=CP::TEventContext::Invalid) return true;
    return false;
}

std::ostream& CP::operator<<(std::ostream& s, const CP::TEventContext& c) {
    s << "<";
    bool valid = false;
    if (c.GetPartition()!=CP::TEventContext::Invalid) {
        s << "P: " << c.GetPartition();
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
