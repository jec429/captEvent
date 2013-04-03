#ifndef TIntegerDatum_hxx_seen
#define TIntegerDatum_hxx_seen

#include <vector>
#include <TDatum.hxx>

namespace CP {
    class TIntegerDatum;
}

/// Save floating point values as a TDatum.  
class CP::TIntegerDatum: public TDatum {
public:
    TIntegerDatum();
    explicit TIntegerDatum(const char* name, int value=0);
    TIntegerDatum(const char* name, const char* title, int value=0);
    TIntegerDatum(const TIntegerDatum&);

    virtual ~TIntegerDatum();

    int GetValue(void) const;
    void SetValue(int value);
    std::vector<int>& GetVector(void);
    const std::vector<int>& GetVector(void) const;

    int& operator [] (int i) {return fValue[i];}
    const int& operator [] (int i) const {return fValue[i];}
    int At(int i) const {return fValue.at(i);}
    unsigned int size() const {return fValue.size();}
    void push_back(int i) {fValue.push_back(i);}

    void ls(Option_t* opt = "") const;

private:
    std::vector<int> fValue;

    ClassDef(TIntegerDatum,2);
};
#endif
