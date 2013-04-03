#include "TAlgorithm.hxx"
#include "TOADatabase.hxx"
#include "TCaptLog.hxx"

ClassImp(CP::TAlgorithm);

CP::TAlgorithm::TAlgorithm(const char* name, const char* title) :
    TNamed(name, title) , fVerbose(Quiet), fQuiet(false) {

    SetBriefDescription(
        "A quick description of the algorithm :"
        " override this with relevant information");

    SetSummaryDescription(
        "A summary of the algorithm :"
        " override this with relevant information.");

    SetDetailedDescription(
        "A full description of the algorithm :"
        " override this with relevant information");

    SetVersion("none set");
}

CP::TAlgorithm::~TAlgorithm(){ }

std::string CP::TAlgorithm::GetDescription() const {
    switch (fVerbose){
    case CP::TAlgorithm::Quiet: return std::string(GetName());
    case CP::TAlgorithm::Brief: return fBriefDescription;
    case CP::TAlgorithm::Summary: return fSummaryDescription;
    case CP::TAlgorithm::Detailed: return fDetailedDescription;
    }
    return std::string("No Description");
}

void CP::TAlgorithm::Clear(Option_t*) {}

void CP::TAlgorithm::SetVersion(const char* v) {
    fVersion = v;
}

CP::THandle<CP::TAlgorithmResult> CP::TAlgorithm::Execute() {
    CP::TAlgorithmResult empty;
    return Process(empty);
}

CP::THandle<CP::TAlgorithmResult> 
CP::TAlgorithm::MaybeProcess(const CP::TAlgorithmResult& input) {
    CP::THandle<CP::TAlgorithmResult>
        result(GetPointer(GetEvent().GetFit(GetName())));
    if (!result) {
        result = Process(input);
        if (result) GetEvent().AddFit(GetPointer(result));
    }
    return result;
}

CP::THandle<CP::TAlgorithmResult> 
CP::TAlgorithm::Process(const CP::TAlgorithmResult&) {
    // The very most trivial Process method that can be written.
    return CP::THandle<CP::TAlgorithmResult>();
}

CP::TAlgorithmTag CP::TAlgorithm::GetTag() const {
    return CP::TAlgorithmTag(*this);
}

CP::TEvent& CP::TAlgorithm::GetEvent() const {
    TEvent* event = CP::TEventFolder::GetCurrentEvent();
    if (!event) {
        CaptError("No current event");
        throw EAlgorithmWithoutEvent();
    }
    return *event;
}

TGeoManager* CP::TAlgorithm::GetGeom() const {
    TGeoManager* geom = NULL;
    try {
        geom = CP::TOADatabase::Get().Geometry();
    }
    catch (...) {
        CaptTrace("Geometry not found");
        geom = NULL;
    }
    return geom;
}
