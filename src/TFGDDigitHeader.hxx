#ifndef TFGDDigitHeader_hxx_seen
#define TFGDDigitHeader_hxx_seen

#include <vector>
#include <TDigitHeader.hxx>

namespace CP {
        class TCMBDigitHeader;
    class TFGDDigitHeader;
};

/// Header information for a single CMB (Crate Master Board).
class CP::TCMBDigitHeader{

public:

    TCMBDigitHeader();

    TCMBDigitHeader(int Minicrate, unsigned int TriggerCounter, unsigned int Timestamp,
                    unsigned int FecError, unsigned int LosFlag, unsigned int SynFail);

    virtual ~TCMBDigitHeader(){};

    int GetMinicrate() const {return fMinicrate;};

    unsigned int GetCMBTriggerCounter() const {return fTriggerCounter;};
    unsigned int GetCMBTimestamp() const {return fTimestamp;};
    
    unsigned int GetCMBFecError() const {return fFecError;}
    unsigned int GetCMBLosFlag() const {return fLosFlag;}
    unsigned int GetCMBSynFail() const {return fSynFail;}

private:

    int fMinicrate;

    unsigned int fTriggerCounter;
    unsigned int fTimestamp;

    unsigned int fFecError;
    unsigned int fLosFlag;
    unsigned int fSynFail;
    
    ClassDef(TCMBDigitHeader, 1);
};

/// Header information for FGD:
/// essentially just a vector of CMB header information.
class CP::TFGDDigitHeader : public CP::TDigitHeader{

public:

    TFGDDigitHeader();

    virtual ~TFGDDigitHeader();

    std::vector<CP::TCMBDigitHeader> const GetCMBHeaders() const {return fCMBHeaders;}

    CP::TCMBDigitHeader const  GetCMBHeader(int cmb) const {

        // Find the right header, else return blank.
        for(unsigned int i = 0 ; i < fCMBHeaders.size(); i++){
            if(fCMBHeaders[i].GetMinicrate() == cmb)
                return fCMBHeaders[i];
        }
        return CP::TCMBDigitHeader();
    }

    void AddCMBHeader(CP::TCMBDigitHeader header){
        fCMBHeaders.push_back(header);        
    }

private:

    std::vector<CP::TCMBDigitHeader> fCMBHeaders;

    ClassDef(TFGDDigitHeader, 1);

};
#endif
