#include "TAlignmentId.hxx"

ClassImp(CP::TAlignmentId);

CP::TAlignmentId::~TAlignmentId() {}

CP::TAlignmentId::TAlignmentId() {}

CP::TAlignmentId::TAlignmentId(const TAlignmentId& hc) 
    : TSHAHashValue(hc), fDocString(hc.fDocString) {}

CP::TAlignmentId::TAlignmentId(const TSHAHashValue& hc) 
    : TSHAHashValue(hc), fDocString("") {}

CP::TAlignmentId::TAlignmentId(unsigned int hc[5]) 
    : TSHAHashValue(hc) {}

CP::TAlignmentId::TAlignmentId(unsigned int hc0, 
                               unsigned int hc1, 
                               unsigned int hc2, 
                               unsigned int hc3, 
                               unsigned int hc4) 
    : TSHAHashValue(hc0, hc1, hc2, hc3, hc4) {}
