#include <iostream>
#include <iomanip>

#include <TBrowser.h>

#include "TReconBase.hxx"
#include "TReconNode.hxx"
#include "TReconState.hxx"
#include "TRealDatum.hxx"
#include "TIntegerDatum.hxx"

ClassImp(CP::TReconBase);

namespace {
    // This keeps track of the previous TReconBase unique identifier.
    static UInt_t gReconBaseId = 0;
}

CP::TReconBase::TReconBase() 
    : TDataVector("", "Reconstruction Object"),
      fQuality(0), fState(NULL), fNodes(NULL), fStatus(0), fNDOF(0) {
    SetUniqueID(++gReconBaseId);
}

CP::TReconBase::TReconBase(const char* name, const char* title)
    : TDataVector(name,title),
      fQuality(0), fState(NULL), fNodes(NULL), fStatus(0), fNDOF(0) {
    SetUniqueID(++gReconBaseId);
}
      
CP::TReconBase::TReconBase(const CP::TReconBase& object)
    : TDataVector(object.GetName(), object.GetTitle()),
      fState(NULL), fNodes(NULL) {
    SetUniqueID(++gReconBaseId);
    
    fQuality = object.GetQuality();
    fStatus = object.GetStatus();
    fNDOF = object.GetNDOF();  
    
    if (object.GetHits()) {
        CP::THitSelection* hits = new THitSelection();
        std::copy(object.GetHits()->begin(), object.GetHits()->end(), 
                  std::back_inserter(*hits));
        AddHits(hits);
    }
    
    if (object.GetConstituents()) {
        AddConstituents(object.GetConstituents());
    }

    // Explicitly look for TRealDatum and TIntegerDatum objects and copy them.
    for (CP::TReconBase::const_iterator obj = object.begin(); 
         obj != object.end(); ++obj) {
        CP::TDatum* datum = dynamic_cast<CP::TDatum*>(*obj);
        // Copy any TRealDatum objects.
        CP::TRealDatum *rDatum = dynamic_cast<CP::TRealDatum*>(datum);
        if (rDatum) {
            CP::TRealDatum* nDatum = new TRealDatum(*rDatum);
            AddDatum(nDatum);
            continue;
        }

        // Copy any TIntegerDatum objects.
        CP::TIntegerDatum *iDatum = dynamic_cast<CP::TIntegerDatum*>(datum);
        if (iDatum) {
            CP::TIntegerDatum* nDatum = new TIntegerDatum(*iDatum);
            AddDatum(nDatum);
            continue;
        }
    }

    // Don't copy the nodes and the state since they depend of the object type 
}

CP::TReconBase::~TReconBase() {
    if (fNodes) delete fNodes;
    if (fState) delete fState;
}

CP::TReconBase::Status CP::TReconBase::GetStatus() const {
    // Notice that this returns both the status bits and the deetector bits.
    return fStatus;
}

void CP::TReconBase::SetStatus(CP::TReconBase::Status status) {
    fStatus |= (status & kStatusMask);
}

void CP::TReconBase::ClearStatus(CP::TReconBase::Status status) {
    fStatus &= ~ (status & kStatusMask);
}

bool CP::TReconBase::CheckStatus(CP::TReconBase::Status status) const {
    return 0 != (fStatus & (status & kStatusMask));
}

CP::TReconBase::Status CP::TReconBase::GetDetectors() const {
    return (fStatus & kDetectorMask);
}

void CP::TReconBase::AddDetector(CP::TReconBase::Status status) {
    fStatus |= (status & kDetectorMask);
}

bool CP::TReconBase::UsesDetector(CP::TReconBase::Status status) const{
    return 0 != (fStatus & (status & kDetectorMask));
}

void CP::TReconBase::RemoveDetector(CP::TReconBase::Status status) {
    fStatus &= ~(status & kDetectorMask);
}

CP::THandle<CP::THitSelection> CP::TReconBase::GetHits() const {
    return Get<CP::THitSelection>("hits");
}

void CP::TReconBase::AddHits(CP::THitSelection* hits) {
    TDatum* h = FindDatum("hits");
    while (h) {
        erase(h);
        delete h;
        h = FindDatum("hits");
    }
    hits->SetName("hits");
    AddDatum(hits);
}

void CP::TReconBase::AddConstituent(CP::THandle<CP::TReconBase> obj) {
    CP::THandle<CP::TReconObjectContainer> container =
        Get<CP::TReconObjectContainer>("constituents");
    if (!container) {
        // Make sure that there aren't any data vector entries with a
        // conflicting name.
        TDatum* h = FindDatum("constituents");
        while (h) {
            erase(h);
            delete h;
            h = FindDatum("constituents");
        }
        // Create the needed object.
        CP::TReconObjectContainer* c
            = new TReconObjectContainer("constituents", 
                                        "Constituents of this object");
        AddDatum(c);
        // And fill the handle so the creation happens transparently.
        container = Get<CP::TReconObjectContainer>("constituents");
    }
    container->push_back(obj);
}

void CP::TReconBase::AddConstituents(
    CP::THandle<CP::TReconObjectContainer> objs) {
    CP::TReconObjectContainer::const_iterator it;
    for (it = objs->begin(); it!= objs->end(); ++it) {
        AddConstituent((*it));
    }
}

CP::THandle<CP::TReconObjectContainer> CP::TReconBase::GetConstituents() const {
    return Get<CP::TReconObjectContainer>("constituents");
}

std::string CP::TReconBase::ConvertStatus() const { 
    std::string s("(");
    bool notFirst = false;
    if (CheckStatus(kRan)) {
        s+= "ran";
        notFirst = true;
    }
    if (CheckStatus(kSuccess)) {
        if (notFirst) s+=":";
        s+= "success";
        notFirst = true;
    }
    if (CheckStatus(kChi2Fit)) {
        if (notFirst) s+=":";
        s+= "chi2";
        notFirst = true;
    }
    if (CheckStatus(kLikelihoodFit)) {
        if (notFirst) s+=":";
        s+= "likelihood";
        notFirst = true;
    }
    if (CheckStatus(kKalmanFit)) {
        if (notFirst) s+=":";
        s+= "kalman";
        notFirst = true;
    }
    if (CheckStatus(kStocasticFit)) {
        if (notFirst) s+=":";
        s+= "stocastic";
        notFirst = true;
    }
    s+= ")";
    return s;
}

std::string CP::TReconBase::ConvertDetector() const {
    std::string s("(");
    bool notFirst = false;
    if (UsesDetector(kTPC)) {
        if (notFirst) s+="-";
        s+= "TPC";
        notFirst = true;
    }
    s+= ")";
    return s;
}

void CP::TReconBase::ls(Option_t *opt) const {
    ls_base(opt);

    TROOT::IncreaseDirLevel();
    std::string option(opt);
    if (fState) {
        TROOT::IncreaseDirLevel();
        fState->ls(opt);
        TROOT::DecreaseDirLevel();
    }
    if (fNodes && (option.find("dump") != std::string::npos
                   || option.find("recon") != std::string::npos)) {
        TROOT::IncreaseDirLevel();
        fNodes->ls(opt);
        TROOT::DecreaseDirLevel();
    }

    TROOT::IncreaseDirLevel();
    for (const_iterator v = begin(); v != end(); ++v) {
        (*v)->ls(opt);
    };
    TROOT::DecreaseDirLevel();

    TROOT::DecreaseDirLevel();
}

void CP::TReconBase::ls_base(Option_t *opt) const {
    CP::TData::ls(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Status: " << ConvertStatus();
    std::string tmp;
    tmp = GetAlgorithmName();
    if (tmp == "") tmp = "not-set";
    if (tmp != "") std::cout << " Algorithm: \"" << tmp << "\"";
    tmp = ConvertDetector();
    if (tmp == "()") tmp = "(not-set)";
    std::cout << "  In: " << tmp; 
    std::cout << std::endl;

    TROOT::IndentLevel();
    std::ios::fmtflags save = std::cout.flags();
    std::cout << "Quality: " << std::setprecision(4) << GetQuality() 
              << " for " << GetNDOF() << " d.o.f." << std::endl;
    std::cout.flags(save);
    TROOT::DecreaseDirLevel();
}

// Add all of the contents to the browser.
void CP::TReconBase::Browse(TBrowser *b) {
    CP::TDataVector::Browse(b);
    if (!b) return;
    b->Add(fNodes,"Nodes");
    b->Add(fState,"State");
}

ClassImp(CP::TReconObjectContainer);

CP::TReconObjectContainer::TReconObjectContainer() 
    : CP::TDatum("unnamed","Recon Object Container") {}

CP::TReconObjectContainer::TReconObjectContainer(const char* name,
                                                 const char* title) 
    : CP::TDatum(name,title) {}

CP::TReconObjectContainer::~TReconObjectContainer() {}

void CP::TReconObjectContainer::push_back(
    CP::THandle<CP::TReconBase> data) {
    std::string name = data->GetName();
    if (name == "unnamed") data->SetName(data->ClassName());
    std::vector< CP::THandle<CP::TReconBase> >::push_back(data);
}

void CP::TReconObjectContainer::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    TROOT::IncreaseDirLevel();
    for (const_iterator t = begin();
         t != end();
         ++t) {
        t->ls(opt);
    }
    TROOT::DecreaseDirLevel();
}

// Add all of the contents to the browser.
void CP::TReconObjectContainer::Browse(TBrowser *b) {
    if (!b) return;
    for (iterator i = begin(); i != end(); ++i) {
        TObject* obj = CP::GetPointer(*i);
        if (!obj) continue;
        std::string name = obj->ClassName();
        if (obj->GetName()) name = obj->GetName();
        b->Add(obj,name.c_str());
    }
}
