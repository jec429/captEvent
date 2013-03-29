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

ClassImp(ND::TND280Event);

ND::TND280Event::TND280Event() {
    /// All other fields are self initializing.
    Build();
}

ND::TND280Event::TND280Event(const ND::TND280Context& context)
    : fContext(context){
    /// All other fields are self initializing.
    Build();
}

ND::TND280Event::~TND280Event() {
    ND::TEventFolder::RemoveEvent(this);
}

void ND::TND280Event::Build(void) {
    if (GetRunId() == ND::TND280Context::Invalid
        || GetEventId() == ND::TND280Context::Invalid) {
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
        AddDatum(new ND::TDataVector("digits","Uncalibrated digit data"));
    }
    if (!FindDatum("hits")) {
        AddDatum(new ND::TDataVector("hits","Calibrated hit data"));
    }
    if (!FindDatum("fits")) {
        AddDatum(new ND::TDataVector("fits","Fit results from reconstruction"));
    }
    Register();
}

void ND::TND280Event::ls(Option_t* opt) const {
    ND::TDatum::ls(opt);
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

void ND::TND280Event::Register(void) {
    ND::TEventFolder::RegisterEvent(this);
}

ND::THandle<ND::TDigitContainer> ND::TND280Event::GetDigits(const char* name) {
    return ND::TOADatabase::Get().Digits().CacheDigits(*this,name);
}

ND::THandle<ND::THitSelection> ND::TND280Event::GetHitSelection(const char* name) 
    const {
    std::string hitName("hits/");
    hitName += name;
    ND::THandle<ND::TDatum> hitsData = Get<ND::TDatum>(hitName.c_str());
    if (!hitsData) return ND::THandle<ND::THitSelection>();
    return Get<ND::THitSelection>(hitName.c_str());
}

ND::THandle<ND::TAlgorithmResult> ND::TND280Event::GetFit(const char* name) const {
    std::string fitName = "~/fits/";
    return Get<ND::TAlgorithmResult>(fitName + name);
}

void ND::TND280Event::AddFit(ND::TAlgorithmResult* fit, const char* name) {
    ND::THandle<ND::TDataVector> fits = Get<ND::TDataVector>("~/fits");
    fits->AddDatum(fit,name);
}

void ND::TND280Event::AddFit(ND::THandle<ND::TAlgorithmResult> fit, const char* name) {
    ND::THandle<ND::TDataVector> fits = Get<ND::TDataVector>("~/fits");
    fit.Release();
    fits->AddDatum(GetPointer(fit),name);
}
