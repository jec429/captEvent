#ifndef TPulseDigitHeader_hxx_seen
#define TPulseDigitHeader_hxx_seen

#include <TDigitHeader.hxx>

namespace CP {
    class TPulseDigitHeader;
};

class CP::TPulseDigitHeader : public CP::TDigitHeader{
public:
    TPulseDigitHeader();
    explicit TPulseDigitHeader(const std::string& name);
    virtual ~TPulseDigitHeader();
private:
    ClassDef(TPulseDigitHeader, 1);
};
#endif
