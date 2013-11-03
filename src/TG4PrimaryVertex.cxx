////////////////////////////////////////////////////////////
// $Id: TG4PrimaryVertex.cxx,v 1.15 2009/09/16 18:30:06 mcgrew Exp $
//

#include <algorithm>
#include <cmath>

#include <TClass.h>
#include <TStreamerInfo.h>

#include "HEPUnits.hxx"
#include "TUnitsTable.hxx"
#include "TG4PrimaryVertex.hxx"

ClassImp(CP::TG4PrimaryVertex);
ClassImp(CP::TG4PrimaryVertexContainer);

CP::TG4PrimaryVertex::TG4PrimaryVertex(void)
    : fPosition(0,0,0,0), fGeneratorName("none"),
      fInteractionNumber(0), fCrossSection(0.0), fDiffCrossSection(0.0),
      fWeight(0.0), fProbability(0.0) {}

CP::TG4PrimaryVertex::TG4PrimaryVertex(const CP::TG4PrimaryVertex& rhs) 
    : TObject(rhs),
      fPosition(rhs.fPosition), fPrimaryParticles(rhs.fPrimaryParticles),
      fGeneratorName(rhs.fGeneratorName), fReaction(rhs.fReaction), 
      fInformational(rhs.fInformational), fFilename(rhs.fFilename),
      fInteractionNumber(rhs.fInteractionNumber), 
      fCrossSection(rhs.fCrossSection),
      fDiffCrossSection(rhs.fDiffCrossSection),
      fWeight(rhs.fWeight), fProbability(rhs.fWeight) {}

CP::TG4PrimaryVertex::~TG4PrimaryVertex() { }

void CP::TG4PrimaryVertex::ls(Option_t* opt) const {
    CP::ls_header(this,opt);
    std::cout << " #" << fInteractionNumber << std::endl;
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << " Position: " << unit::AsString(fPosition,"length")
              << std::endl;
    std::string generatorName = GetGeneratorName();
    std::string reactionMode = GetReaction();
    if (generatorName != "" || reactionMode != "") {
        TROOT::IndentLevel();
        std::cout << "Generator: " << generatorName 
                  << "  Reaction: <" << reactionMode << ">"
                  << std::endl;
    }
    std::string filename = GetFilename();
    if (filename != "") {
        TROOT::IndentLevel();
        std::cout << "Filename: " << filename << std::endl;
    }
    if (fCrossSection>0.0 || fDiffCrossSection>0.0) {
        TROOT::IndentLevel();
        std::cout << "Cross Section: " 
                  << fCrossSection/(unit::femto*unit::barn) << "fb"
                  << "   Diff: "
                  << fDiffCrossSection/(unit::femto*unit::barn) << "fb"
                  << std::endl;
    }
    if (std::abs(fProbability-1.0)>1E-3 || std::abs(fWeight-1.0)>1E-3) {
        TROOT::IndentLevel();
        std::cout << "Probability: " << fProbability 
                  << "  Weight: "  << fWeight
                  << std::endl;
    }
    for (CP::TG4PrimaryParticleContainer::const_iterator v 
             = fPrimaryParticles.begin();
         v != fPrimaryParticles.end(); 
         ++v) {
        v->ls(opt);
    };
    if (fInformational.size()>0) {
        TROOT::IncreaseDirLevel();
        fInformational.ls(opt);
        TROOT::DecreaseDirLevel();
    }
    TROOT::DecreaseDirLevel();
}

void CP::TG4PrimaryVertexContainer::ls(Option_t* opt) const {
    CP::TDatum::ls(opt);
    TROOT::IncreaseDirLevel();
    for (const_iterator v = begin();
         v != end(); 
         ++v) {
        v->ls(opt);
    };
    TROOT::DecreaseDirLevel();
}

void CP::TG4PrimaryVertex::SetGeneratorName(const char *name) {
    fGeneratorName = name;
}

void CP::TG4PrimaryVertex::SetReaction(const char *reaction) {
    fReaction = reaction;
}

void CP::TG4PrimaryVertex::SetInfoVertex(const CP::TG4PrimaryVertexContainer& info) {
    fInformational = info;
}

void CP::TG4PrimaryVertex::SetFilename(const char *filename) {
    fFilename = filename;
}
