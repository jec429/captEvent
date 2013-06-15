#include "TAlgorithmResult.hxx"

#include "TAlgorithm.hxx"
#include "TAlgorithmTag.hxx"
#include "TDataSymLink.hxx"
#include "TCaptLog.hxx"


ClassImp(CP::TAlgorithmResult);

const CP::TAlgorithmResult CP::TAlgorithmResult::Empty;

CP::TAlgorithmResult::TAlgorithmResult(): fStatusSummary("") { }

CP::TAlgorithmResult::TAlgorithmResult(const char* name, const char* title) 
    : CP::TDataVector(name, title), fStatusSummary("") { }

CP::TAlgorithmResult::TAlgorithmResult(const CP::TAlgorithm& algo) 
    : CP::TDataVector(algo.GetName(), "An Algorithm Result"),
      fTag(algo.GetTag()), fStatusSummary("") { }

CP::TAlgorithmResult::TAlgorithmResult(const CP::THitSelection& hits)
    : CP::TDataVector(hits.GetName(), "An Algorithm Result"),
      fStatusSummary("") {
    SetName(hits.GetName());
    CP::THitSelection* internalHits = new CP::THitSelection(hits.GetName());
    for (CP::THitSelection::const_iterator h = hits.begin();
         h != hits.end();
         ++h) {
        internalHits->push_back(*h);
    }
    AddHitSelection(internalHits);
}

CP::TAlgorithmResult::~TAlgorithmResult() { }

void CP::TAlgorithmResult::AddStatus(const char* s) {
    std::string status(s);
    fStatusSummary = "(" + status + ") " + fStatusSummary;
}

void CP::TAlgorithmResult::AddStatus(const std::string& status) {
    fStatusSummary = "(" + status + ") " + fStatusSummary;
}

void CP::TAlgorithmResult::SetStatus(const char* status) {
    fStatusSummary = status;
}

void CP::TAlgorithmResult::SetStatus(const std::string& status) {
    fStatusSummary = status;
}

std::string CP::TAlgorithmResult::GetStatus() const {
    return fStatusSummary;
}

void CP::TAlgorithmResult::AddResultsContainer(
    CP::TReconObjectContainer* results) {
    SetDefaultResultsContainer(results->GetName());
    AddDatum(results);
}

void CP::TAlgorithmResult::SetDefaultResultsContainer(const char* name) {
    std::string trackName(name);
    if (trackName == "results") {
        CaptSevere("Illegal TReconObjectContainer name: " << name );
        throw EIllegalRecObjContainerName();
    }

    // Update or add the symbolic link to the default set of tracks.
    CP::TDataSymLink* link 
        = dynamic_cast<CP::TDataSymLink*>(FindDatum("results"));
    if (link) {
        link->SetLink(name);
    }
    else {
        AddDatum(new CP::TDataSymLink("results",name));
    }
}

CP::THandle<CP::TReconObjectContainer> 
CP::TAlgorithmResult::GetResultsContainer(const char* name) const {
    CP::THandle<CP::TReconObjectContainer> result 
        = Get<CP::TReconObjectContainer>(name);
    return result;
}

void CP::TAlgorithmResult::AddHitSelection(CP::THitSelection* hits) {
    SetDefaultHitSelection(hits->GetName());
    AddDatum(hits);
}

void CP::TAlgorithmResult::SetDefaultHitSelection(const char* name) {
    std::string hitName(name);
    if (hitName == "hits") {
        CaptSevere("Illegal hit name " << name);
        throw EIllegalHitSelectionName();
    }

    // Update or add the symbolic link to the default set of hits.
    CP::TDataSymLink* link = dynamic_cast<CP::TDataSymLink*>(FindDatum("hits"));
    if (link) {
        link->SetLink(name);
    }
    else {
        AddDatum(new CP::TDataSymLink("hits",name));
    }
}

CP::THandle<CP::THitSelection> CP::TAlgorithmResult::GetHitSelection(
    const char* name) const {
    CP::THandle<CP::THitSelection> result = Get<CP::THitSelection>(name);
    return result;
}

/// Print the hit information.
void CP::TAlgorithmResult::ls(Option_t *opt) const {
    CP::TData::ls(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Status: ";
    if (fStatusSummary != "") std::cout << fStatusSummary;
    else std::cout << "None";
    std::cout << std::endl;
    fTag.ls(opt);
    for (const_iterator v = begin(); v != end(); ++v) {
        (*v)->ls(opt);
    };
    TROOT::DecreaseDirLevel();
}
