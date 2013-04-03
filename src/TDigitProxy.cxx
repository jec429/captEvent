#include <sstream>
#include <string>

#include "TDigitProxy.hxx"
#include "TDigitManager.hxx"
#include "TOADatabase.hxx"

ClassImp(CP::TDigitProxy);

CP::TDigitProxy::~TDigitProxy() {}

CP::TDigitProxy::TDigitProxy() 
  : fDigitSignature(0), fDigit(NULL), fContainer(NULL) {}

CP::TDigitProxy::TDigitProxy(const CP::TDigitContainer& container,
                             unsigned int offset) 
    : fDigitSignature(0), fDigit(NULL), fContainer(NULL) {

    int type = TDigitProxy::ConvertName(container.GetName());
    SetProxyType(type);
    if (container.size() <= offset) throw EDigitNotFound();
    CP::TDigit* digit = container[offset];
    unsigned int cid = digit->GetChannelId().AsUInt();
    unsigned int salt = container.GetSignature()+cid;
    SetProxySalt(salt);
    SetProxyOffset(offset);
    SetProxyCache(digit,NULL);
}

CP::TDigitProxy::TDigitProxy(const CP::TDigitContainer& container,
                             const CP::TDigitContainer::iterator index) 
    : fDigitSignature(0), fDigit(NULL), fContainer(NULL) {

    int type = TDigitProxy::ConvertName(container.GetName());
    SetProxyType(type);
    int offset = index-container.begin();
    if (offset<0 || container.size()<=(unsigned)offset) throw EDigitNotFound();
    CP::TDigit* digit = container[offset];
    unsigned int cid = digit->GetChannelId().AsUInt();
    unsigned int salt = container.GetSignature()+cid;
    SetProxySalt(salt);
    SetProxyOffset(offset);
    SetProxyCache(digit,NULL);
}

CP::TDigitProxy::TDigitProxy(int proxy) 
    : fDigitSignature(proxy), fDigit(NULL), fContainer(NULL) {}

CP::TDigit* CP::TDigitProxy::operator*() const {
    return CP::TOADatabase::Get().Digits().GetDigit(*this);
}

CP::TDigitContainer& CP::TDigitProxy::GetContainer() const {
    CP::TOADatabase::Get().Digits().GetDigit(*this);
    return *fContainer;
}

enum CP::TDigitProxy::ProxyType CP::TDigitProxy::ConvertName(std::string name) {
    if (name == "p0d") return kP0D;
    if (name == "tpc") return kTPC;
    if (name == "fgd") return kFGD;
    if (name == "ecal") return kECal;
    if (name == "smrd") return kSMRD;
    if (name == "ingrid") return kINGRID;
    if (name == "test") return kTest;
    return kInvalid;
}

std::string CP::TDigitProxy::ConvertType(int type) {
    std::string name;

    // Find the name of the TDigitContainer based on the proxy type.
    switch (type) {
    case kTest: name = "test"; break;
    case kP0D: name = "p0d"; break;
    case kTPC: name = "tpc"; break;
    case kFGD: name = "fgd"; break;
    case kECal: name = "ecal"; break;
    case kSMRD: name = "smrd"; break;
    case kINGRID: name = "ingrid"; break;
    case kInvalid: name = "invalid"; break;
    default:
        throw EDigitTypeInvalid();
    }

    return name;
}

bool CP::TDigitProxy::CheckSalt(unsigned int signature,
                                const CP::TDigit* digit) const {
    unsigned int trial = (signature + digit->GetChannelId().AsUInt())%1024;
    return trial == GetProxySalt();
}

enum CP::TDigitProxy::ProxyType CP::TDigitProxy::GetProxyType() const {
    unsigned sig = (fDigitSignature & 0xF8000000u) >> 27;
    return (CP::TDigitProxy::ProxyType) sig;
}

unsigned int CP::TDigitProxy::GetProxySalt() const {
    unsigned int salt = (fDigitSignature & 0x07FE0000u) >> 17;
    return salt;
}

unsigned int CP::TDigitProxy::GetProxyOffset() const {
    unsigned int offset = (fDigitSignature & 0x0001FFFF);
    return offset;
}

CP::TDigit* CP::TDigitProxy::GetProxyCache() const {
    return fDigit;
}

void CP::TDigitProxy::SetProxyCache(TDigit* digit,
                                    TDigitContainer* container) const {
    fDigit = digit;
    fContainer = container;
}

void CP::TDigitProxy::SetProxyType(int type) {
    fDigitSignature = (fDigitSignature & 0x07FFFFFFu)|((type & 0x001Fu) << 27);
}

void CP::TDigitProxy::SetProxySalt(int salt) {
    fDigitSignature = (fDigitSignature & 0xF801FFFFu)|((salt & 0x03FFu) << 17);
}

void CP::TDigitProxy::SetProxyOffset(int off) {
    if (off & 0xFFFE0000u) off = 0x1FFFFu;
    if (off == 0x1FFFFu) ND280Severe("Invalid proxy has been created.");
    fDigitSignature = (fDigitSignature & 0xFFFE0000u) | (off & 0x1FFFFu);
}

bool CP::TDigitProxy::IsValid() const {
    if (GetProxyOffset() == 0x1FFFFu) return false;
    if (GetProxyType() == kTest) return true;
    if (GetProxyType() == kP0D) return true;
    if (GetProxyType() == kTPC) return true;
    if (GetProxyType() == kFGD) return true;
    if (GetProxyType() == kECal) return true;
    if (GetProxyType() == kSMRD) return true;
    if (GetProxyType() == kINGRID) return true;
    return false;
}

std::string CP::TDigitProxy::AsString() const {
    std::ostringstream out;
    try {
        out << ConvertType(GetProxyType()) << ":";
    }
    catch (...) {
        out << "invalid:";
    }
    unsigned int offset = GetProxyOffset();
    if (offset == 0x1FFFFu) out << " invalid offset";
    else out << " " << offset;
    return out.str();
}

bool CP::TDigitProxy::operator == (const CP::TDigitProxy& rhs) const {
    return fDigitSignature == rhs.fDigitSignature;
}
