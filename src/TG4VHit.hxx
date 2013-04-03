#ifndef TG4VHit_hxx_seen
#define TG4VHit_hxx_seen

#include <vector> 

#include <TObject.h>

#include "TDatum.hxx"

namespace CP {
    class TG4VHit;
    class TG4HitContainer;
}

/// DO NOT USE THIS CLASS IN ANALYSIS CODE.
///
/// This is an abstract base class used to save G4Hit objects into a root
/// output file.  It contains the information that all hits from the
/// simulation should save into the output file, and allows the rest of the
/// code to handle hits in a polymorphic way.  It must be inherited by an
/// specific instance class to actually save MC hit information.
///
/// In the the simulation, this class is multiply inherited with G4VHit to
/// create a class that can be used in a G4HitCollection.  The behavior is
/// designed so that the TG4VHit inherited classes in captEvent contain all of
/// the payload for the hit, and class in the simulation contains all of the
/// interface related to the G4 infrastructure.
class CP::TG4VHit: public TObject {
public:
    TG4VHit();
    virtual ~TG4VHit();
    
    /// Print the hit information.
    virtual void ls(Option_t * = "") const;

protected:
    ClassDef(TG4VHit,1);
};

/// A container for the G4 hit detector element information. 
class CP::TG4HitContainer : public TDatum, public std::vector<TG4VHit*> {
public:
    TG4HitContainer(const char* name="G4hits", 
                        const char* title="G4 Hit Information") 
        : TDatum(name,title) {};
    virtual ~TG4HitContainer();
  
    /// Print the data vector information.
    virtual void ls(Option_t* opt = "") const;

private:
    ClassDef(TG4HitContainer,1);
};
#endif
