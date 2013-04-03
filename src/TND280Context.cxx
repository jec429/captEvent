#include "TND280Context.hxx"
#include <ctime>

const int CP::TND280Context::Invalid = 0xDEADBEEF;

ClassImp(CP::TND280Context);

CP::TND280Context::~TND280Context() {}

CP::TND280Context::TND280Context()
    : fPartition(Invalid), 
      fRun(Invalid), fSubRun(Invalid), fEvent(Invalid), 
      fSpill(Invalid), fTimeStamp(Invalid) {}

CP::TND280Context::TND280Context(int partition, 
                                 int run, int subRun, int event, 
                                 int spill, int stamp)
    : fPartition(partition), 
      fRun(run), fSubRun(subRun), fEvent(event), 
      fSpill(spill), fTimeStamp(stamp) {}

int CP::TND280Context::GetPartition() const {return fPartition;}
int CP::TND280Context::GetRun() const {return fRun;}
int CP::TND280Context::GetSubRun() const {return fSubRun;}
int CP::TND280Context::GetEvent() const {return fEvent;}
int CP::TND280Context::GetSpill() const {return fSpill;}
int CP::TND280Context::GetTimeStamp() const {return fTimeStamp;}
std::string CP::TND280Context::GetTimeStampString() const {
    std::time_t t = GetTimeStamp();
    char buf[256];
    std::strftime(buf,sizeof(buf),"'%Y-%m-%d %H:%M:%S'",std::gmtime(&t));
    return std::string(buf);
}

void CP::TND280Context::SetPartition(int v) {fPartition = v;}
void CP::TND280Context::SetRun(int v) {fRun = v;}
void CP::TND280Context::SetSubRun(int v) {fSubRun = v;}
void CP::TND280Context::SetEvent(int v) {fEvent = v;}
void CP::TND280Context::SetSpill(int v) {fSpill = v;}
void CP::TND280Context::SetTimeStamp(int v) {fTimeStamp = v;}

bool CP::TND280Context::IsMC() const {
    if (fPartition == Invalid) return false;
    if (fPartition & kMCData) return true;
    return false;
}

bool CP::TND280Context::IsDetector() const {
    if (fPartition == Invalid) return false;
    if (fPartition & kMCData) return false;
    return true;
}

bool CP::TND280Context::Valid() const {
    if (GetPartition()!=CP::TND280Context::Invalid) return true;
    if (GetRun()!=CP::TND280Context::Invalid)  return true;
    if (GetSubRun()!=CP::TND280Context::Invalid) return true;
    if (GetEvent()!=CP::TND280Context::Invalid) return true;
    if (GetSpill()!=CP::TND280Context::Invalid) return true;
    if (GetTimeStamp()!=CP::TND280Context::Invalid) return true;
    return false;
}

std::ostream& operator<<(std::ostream& s, const CP::TND280Context& c) {
    s << "<";
    bool valid = false;
    if (c.GetPartition()!=CP::TND280Context::Invalid) {
        s << "P: " << c.GetPartition();
        valid = true;
    }
    if (c.GetRun()!=CP::TND280Context::Invalid) {
        if (valid) s << " ";
        s << "R: " << c.GetRun();
        valid = true;
    }
    if (c.GetSubRun()!=CP::TND280Context::Invalid) {
        if (valid) s << " ";
        s << "SR: " << c.GetSubRun();
        valid = true;
    }
    if (c.GetEvent()!=CP::TND280Context::Invalid) {
        if (valid) s << " ";
        s << "E: " << c.GetEvent();
        valid = true;
    }
    if (c.GetSpill()!=CP::TND280Context::Invalid) {
        if (valid) s << " ";
        s << "S: " << c.GetSpill();
        valid = true;
    }
    if (c.GetTimeStamp()!=CP::TND280Context::Invalid) {
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
