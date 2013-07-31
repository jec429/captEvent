#ifndef TIntegerDatum_hxx_seen
#define TIntegerDatum_hxx_seen

#include <vector>
#include <TDatum.hxx>

namespace CP {
    class TIntegerDatum;
}

/// Save integer values as a TDatum.  This allows named arrays of integers to
/// be added to any TDataVector object.  It doesn't implement full vector
/// sematics, and is mostly intended to fill in for missing parts of the data
/// format definition.  If it's being used heavily, that's an indication that
/// the event API is being abused, or that the API is missing a critical
/// feature.
class CP::TIntegerDatum: public TDatum {
public:
    typedef int Element;
    typedef std::vector<int>::iterator iterator;
    typedef std::vector<int>::const_iterator const_iterator;
    
    TIntegerDatum();
    explicit TIntegerDatum(const char* name, int value=0);
    TIntegerDatum(const char* name, const char* title, int value=0);
    TIntegerDatum(const TIntegerDatum&);

    virtual ~TIntegerDatum();

    int GetValue(void) const;
    void SetValue(int value);

    iterator begin(void) {return fValue.begin();}
    iterator end() {return fValue.end();}
    const_iterator begin(void) const {return fValue.begin();}
    const_iterator end() const {return fValue.end();}

    int& operator [] (int i) {return fValue[i];}
    const int& operator [] (int i) const {return fValue[i];}
    int at(int i) const {return fValue.at(i);}
    unsigned int size() const {return fValue.size();}
    void push_back(int i) {fValue.push_back(i);}
    void clear(void) {fValue.clear();}

    void ls(Option_t* opt = "") const;

private:
    /// This is the payload.  It should be match the typedefs above, but
    /// because of how ROOT handles types, it's best to leave this as a direct
    /// declaration.
    std::vector<int> fValue;

    ClassDef(TIntegerDatum,2);
};
#endif
