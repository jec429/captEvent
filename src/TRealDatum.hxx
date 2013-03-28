#ifndef TRealDatum_hxx_seen
#define TRealDatum_hxx_seen

#include <vector>
#include <TDatum.hxx>

namespace ND {
    class TRealDatum;
}

/// Save floating point values as a TDatum.  
class ND::TRealDatum: public TDatum {
public:
    TRealDatum();
    explicit TRealDatum(const char* name, double value=0);
    TRealDatum(const char* name, const char* title, double value=0);
    TRealDatum(const TRealDatum& rhs);

    virtual ~TRealDatum();

    double GetValue(void) const;
    void SetValue(double value);
    std::vector<double>& GetVector(void);
    const std::vector<double>& GetVector(void) const;

    double& operator [] (int i) {return fValue[i];}
    const double& operator [] (int i) const {return fValue[i];}
    double At(int i) const {return fValue.at(i);}
    unsigned int size() const {return fValue.size();}
    void push_back(double r) {fValue.push_back(r);}

    void ls(Option_t* opt = "") const;

private:
    std::vector<double> fValue;

    ClassDef(TRealDatum,2);
};
#endif
