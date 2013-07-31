#ifndef TRealDatum_hxx_seen
#define TRealDatum_hxx_seen

#include <vector>
#include <TDatum.hxx>

namespace CP {
    class TRealDatum;
}

/// Save floating point values as a TDatum.  This allows named arrays of
/// floating points to be added to any TDataVector object.  It doesn't
/// implement full vector sematics, and is mostly intended to fill in for
/// missing parts of the data format definition.  If it's being used heavily,
/// that's an indication that the event API is being abused, or that the API
/// is missing a critical feature.
class CP::TRealDatum: public TDatum {
public:
    typedef double Element;
    typedef std::vector<double>::iterator iterator;
    typedef std::vector<double>::const_iterator const_iterator;

    TRealDatum();
    explicit TRealDatum(const char* name, double value=0);
    TRealDatum(const char* name, const char* title, double value=0);
    TRealDatum(const TRealDatum& rhs);

    virtual ~TRealDatum();

    double GetValue(void) const;
    void SetValue(double value);

    iterator begin(void) {return fValue.begin();}
    iterator end() {return fValue.end();}
    const_iterator begin(void) const {return fValue.begin();}
    const_iterator end() const {return fValue.end();}

    double& operator [] (int i) {return fValue[i];}
    const double& operator [] (int i) const {return fValue[i];}
    double at(int i) const {return fValue.at(i);}
    unsigned int size() const {return fValue.size();}
    void push_back(double r) {fValue.push_back(r);}
    void clear(void) {fValue.clear();}

    void ls(Option_t* opt = "") const;

private:
    /// This is the payload.  It should be match the typedefs above, but
    /// because of how ROOT handles types, it's best to leave this as a direct
    /// declaration.
    std::vector<double> fValue;

    ClassDef(TRealDatum,2);
};
#endif
