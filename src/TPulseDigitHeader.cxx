#include "TPulseDigitHeader.hxx"

ClassImp(CP::TPulseDigitHeader);

CP::TPulseDigitHeader::TPulseDigitHeader(): TDigitHeader("pulse") { }

CP::TPulseDigitHeader::TPulseDigitHeader(const std::string& name)
    : TDigitHeader(name) {}

CP::TPulseDigitHeader::~TPulseDigitHeader() { }

