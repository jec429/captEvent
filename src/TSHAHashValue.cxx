#include <iostream>
#include <sstream>
#include <iomanip>

#include "TSHAHashValue.hxx"

ClassImp(CP::TSHAHashValue);

CP::TSHAHashValue::~TSHAHashValue() {}

CP::TSHAHashValue::TSHAHashValue() {
    for(int i=0; i<5; ++i) fHash[i] = 0;
}

CP::TSHAHashValue::TSHAHashValue(const TSHAHashValue& hc) {
    for(int i=0; i<5; ++i) fHash[i] = hc(i);
}

CP::TSHAHashValue::TSHAHashValue(unsigned int hc[5]) {
    for(int i=0; i<5; ++i) fHash[i] = hc[i];
}

CP::TSHAHashValue::TSHAHashValue(unsigned int hc0, 
                                 unsigned int hc1, 
                                 unsigned int hc2, 
                                 unsigned int hc3, 
                                 unsigned int hc4) {
    fHash[0] = hc0;
    fHash[1] = hc1;
    fHash[2] = hc2;
    fHash[3] = hc3;
    fHash[4] = hc4;
}

std::string CP::TSHAHashValue::AsString() const {
    std::ostringstream nameStream;
    nameStream << std::hex << std::nouppercase << std::setfill('0');
    if (fHash[0]) nameStream << std::setw(8) << fHash[0];
    else nameStream << "xxxxxxxx";
    nameStream << '-';
    if (fHash[1]) nameStream << std::setw(8) << fHash[1];
    else nameStream << "xxxxxxxx";
    nameStream << '-';
    if (fHash[2]) nameStream << std::setw(8) << fHash[2];
    else nameStream << "xxxxxxxx";    
    nameStream << '-';
    if (fHash[3]) nameStream << std::setw(8) << fHash[3];
    else nameStream << "xxxxxxxx";    
    nameStream << '-';
    if (fHash[4]) nameStream << std::setw(8) << fHash[4];
    else nameStream << "xxxxxxxx";    
    return nameStream.str();
}

bool CP::TSHAHashValue::Equivalent(const TSHAHashValue& hc) const {
    bool valid = false;
    for (int i=0; i<5; ++i) {
        if (!fHash[i]) continue;
        if (!hc(i)) continue;
        if (hc(i)!=fHash[i]) return false;
        valid = true;
    }
    return valid;
}

bool CP::TSHAHashValue::Valid() const {
    return (fHash[0] || fHash[1] || fHash[2] || fHash[3] || fHash[4]);
}
    
bool CP::operator<(const CP::TSHAHashValue& lhs, const CP::TSHAHashValue& rhs) {
    for (int i=0; i<5; ++i) {
        if (lhs(i) > rhs(i)) return false;
        if (lhs(i) < rhs(i)) return true;
    }
    return false;
}

bool CP::operator==(const CP::TSHAHashValue& lhs, 
                    const CP::TSHAHashValue& rhs) {
    for (int i=0; i<5; ++i) {
        if (lhs(i) != rhs(i)) return false;
    }
    return true;
}

bool CP::operator!=(const CP::TSHAHashValue& lhs, 
                    const CP::TSHAHashValue& rhs) {
    return !(lhs == rhs);
}

std::ostream& CP::operator<<(std::ostream& s, const CP::TSHAHashValue& c) {
    s << "<";
    s << c.AsString();
    s << ">";
    return s;
}
