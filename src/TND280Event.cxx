////////////////////////////////////////////////////////////
// $Id: TND280Event.cxx,v 1.39 2011/12/07 21:53:26 mcgrew Exp $
//
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>

#include "TND280Event.hxx"
#include "TEventFolder.hxx"
#include "THitSelection.hxx"
#include "TAlgorithmResult.hxx"
#include "TOADatabase.hxx"
#include "TDigitManager.hxx"
#include "TDigitContainer.hxx"

ClassImp(CP::TND280Event);

CP::TND280Event::TND280Event() {
    /// All other fields are self initializing.
    Build();
}

CP::TND280Event::TND280Event(const CP::TEventContext& context)
    : fContext(context){
    /// All other fields are self initializing.
    Build();
}

CP::TND280Event::~TND280Event() {
    CP::TEventFolder::RemoveEvent(this);
}

void CP::TND280Event::Build(void) {
    if (GetRunId() == CP::TEventContext::Invalid
        || GetEventId() == CP::TEventContext::Invalid) {
        SetName("event.uninitialized");
    }
    else {
        std::stringstream name;
        name << "event." 
             << std::setw(5) << std::setfill('0') << GetRunId()
             << std::setw(8) << std::setfill('0') << GetEventId();
        SetName(name.str().c_str());
    }
    if (!FindDatum("digits")) {
        AddDatum(new CP::TDataVector("digits","Uncalibrated digit data"));
    }
    if (!FindDatum("hits")) {
        AddDatum(new CP::TDataVector("hits","Calibrated hit data"));
    }
    if (!FindDatum("fits")) {
        AddDatum(new CP::TDataVector("fits","Fit results from reconstruction"));
    }
    Register();
}

void CP::TND280Event::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Context: " << GetContext() << std::endl;
    std::ios::fmtflags save = std::cout.flags();

    if (GetGeometryHash().Valid()) {
        TROOT::IndentLevel();
        std::cout << "Geometry:  "
                  << GetGeometryHash()
                  << std::endl;
    }

    if (GetAlignmentId().Valid()) {
        TROOT::IndentLevel();
        std::cout << "Alignment: "
                  << GetAlignmentId()
                  << std::endl;
    }

    std::cout.flags(save);
    for (const_iterator v = begin();
         v != end(); 
         ++v) {
        (*v)->ls(opt);
    }
    std::cout.fill(' ');
    std::cout.flags(save);
    TROOT::DecreaseDirLevel();
}

void CP::TND280Event::Register(void) {
    CP::TEventFolder::RegisterEvent(this);
}

CP::THandle<CP::TDigitContainer> CP::TND280Event::GetDigits(const char* name) {
    return CP::TOADatabase::Get().Digits().CacheDigits(*this,name);
}

CP::THandle<CP::THitSelection> CP::TND280Event::GetHitSelection(const char* name) 
    const {
    std::string hitName("hits/");
    hitName += name;
    CP::THandle<CP::TDatum> hitsData = Get<CP::TDatum>(hitName.c_str());
    if (!hitsData) return CP::THandle<CP::THitSelection>();
    return Get<CP::THitSelection>(hitName.c_str());
}

CP::THandle<CP::TAlgorithmResult> CP::TND280Event::GetFit(const char* name) const {
    std::string fitName = "~/fits/";
    return Get<CP::TAlgorithmResult>(fitName + name);
}

void CP::TND280Event::AddFit(CP::TAlgorithmResult* fit, const char* name) {
    CP::THandle<CP::TDataVector> fits = Get<CP::TDataVector>("~/fits");
    fits->AddDatum(fit,name);
}

void CP::TND280Event::AddFit(CP::THandle<CP::TAlgorithmResult> fit, const char* name) {
    CP::THandle<CP::TDataVector> fits = Get<CP::TDataVector>("~/fits");
    fit.Release();
    fits->AddDatum(GetPointer(fit),name);
}
