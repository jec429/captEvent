#include "TAlgorithm.hxx"
#include "TManager.hxx"
#include "TCaptLog.hxx"

ClassImp(CP::TAlgorithm);

CP::TAlgorithm::TAlgorithm(const char* name, const char* title) :
    TNamed(name, title){
    SetVersion("none set");
}

CP::TAlgorithm::~TAlgorithm(){ }

void CP::TAlgorithm::Clear(Option_t*) {}

void CP::TAlgorithm::SetVersion(const char* v) {
    fVersion = v;
}

CP::THandle<CP::TAlgorithmResult> 
CP::TAlgorithm::Process(const CP::TAlgorithmResult&,
                        const CP::TAlgorithmResult&,
                        const CP::TAlgorithmResult&) {
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
        geom = CP::TManager::Get().Geometry();
    }
    catch (...) {
        CaptTrace("Geometry not found");
        geom = NULL;
    }
    return geom;
}
