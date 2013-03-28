#include <TROOT.h>
#include <nd280EventLoop.hxx>

#include "THandle.hxx"
#include "TAlgorithmResult.hxx"
#include "TReconBase.hxx"
#include "TReconTrack.hxx"
#include "TReconShower.hxx"
#include "TReconCluster.hxx"
#include "TReconVertex.hxx"
#include "TReconNode.hxx"
#include "TReconPID.hxx"

bool CheckReconTrack(const std::string& name,
                     ND::THandle<ND::TReconTrack> track) {
    return true;
}

bool CheckReconShower(const std::string& name,
                      ND::THandle<ND::TReconShower> track) {
    return true;
}

bool CheckReconCluster(const std::string& name,
                       ND::THandle<ND::TReconCluster> track) {
    return true;
}

bool CheckReconVertex(const std::string& name,
                      ND::THandle<ND::TReconVertex> vertex) {
    return true;
}

bool CheckReconPID(const std::string& name,
                   ND::THandle<ND::TReconPID> track) {
    return true;
}

bool CheckReconObject(const std::string& name, 
                      ND::THandle<ND::TReconBase> recon) {
    if (!recon) return false;

    ND::THandle<ND::TReconTrack> track = recon;
    if (track) return CheckReconTrack(name,track);
    ND::THandle<ND::TReconShower> shower = recon;
    if (shower) return CheckReconShower(name,shower);
    ND::THandle<ND::TReconCluster> cluster = recon;
    if (cluster) return CheckReconCluster(name,cluster);
    ND::THandle<ND::TReconVertex> vertex = recon;
    if (vertex) return CheckReconVertex(name,vertex);
    ND::THandle<ND::TReconPID> pid = recon;
    if (pid) return CheckReconPID(name,pid);
    return false;
}

bool CheckReconObjects(const std::string& name,
                       ND::THandle<ND::TReconObjectContainer> recon) {
    if (!recon) return true;

    bool ok = true;
    // Find all of the recon objects and check them.
    for (ND::TReconObjectContainer::iterator obj = recon->begin();
         obj != recon->end();
         ++obj) {
        if (!CheckReconObject(name, *obj)) ok = false;
    }

    return ok;
}


class TCheckFits: public ND::TND280EventLoopFunction {
public:
    TCheckFits() {
    }

    virtual ~TCheckFits() {};

    void Usage(void) {
    }

    virtual bool SetOption(std::string option,std::string value="") {
        return true;
    }

    bool operator () (ND::TND280Event& event) {

        // Forage the algorithm results
        std::vector<ND::TAlgorithmResult*> algoResults;
        {
            std::vector<ND::TDatum*> datumStack;
                datumStack.push_back(&event);
            while (!datumStack.empty()) {
                ND::TDatum* current = datumStack.back();
                datumStack.pop_back();
                ND::TAlgorithmResult* rc 
                    = dynamic_cast<ND::TAlgorithmResult*>(current);
                if (rc) {
                    algoResults.push_back(rc);
                }
                ND::TDataVector* dv = dynamic_cast<ND::TDataVector*>(current);
                if (dv) {
                    for (ND::TDataVector::iterator d = dv->begin();
                         d != dv->end();
                         ++d) {
                        datumStack.push_back(*d);
                    }
                }
            }
        }

        // Forage the reconstruction objects.
        std::vector<ND::TReconObjectContainer*> reconObjs;
        {
            std::vector<ND::TDatum*> datumStack;
            datumStack.push_back(&event);
            while (!datumStack.empty()) {
                ND::TDatum* current = datumStack.back();
                datumStack.pop_back();
                ND::TReconObjectContainer* rc 
                    = dynamic_cast<ND::TReconObjectContainer*>(current);
                if (rc) {
                    reconObjs.push_back(rc);
                    continue;
                }
                ND::TDataVector* dv = dynamic_cast<ND::TDataVector*>(current);
                if (dv) {
                    for (ND::TDataVector::iterator d = dv->begin();
                         d != dv->end();
                         ++d) {
                        datumStack.push_back(*d);
                    }
                }
            }
        }

        for (std::vector<ND::TAlgorithmResult*>::const_iterator r 
                 = algoResults.begin();
             r != algoResults.end();
             ++r) {
            std::string fullName((*r)->GetFullName());
            bool ok = true;

            ND::THandle<ND::TReconObjectContainer> result
                = (*r)->GetResultsContainer();
            ND::THandle<ND::THitSelection> hits
                = (*r)->GetHitSelection();

            // OK if the AlgorithmResult is empty.  That means that the
            // algorithm ran, but didn't produce any results or hit selections
            // (usually the event didn't pass sanity cuts).
            if (!result && !hits) continue;

            // If an TReconObjectContainer exists, check that it's OK.
            if (result) {
                std::string resultName(result->GetName());
                if (resultName != "final") {
                    ok = false;
                    ND280Warn("Default object container not named final: "
                              << fullName << "(was " << resultName << ")");
                }
                ND::THandle<ND::TReconObjectContainer> final
                    = (*r)->GetResultsContainer("final");
                if (!final) {
                    ok = false;
                    ND280Error("Final object container does not exist: "
                               << fullName);
                }
            }

            if (hits || result) {
                if (!hits) {
                    ok = false;
                    ND280Warn("No hits in result: "
                              << fullName);
                }
                else {
                    std::string hitName(hits->GetName());
                    if (hitName == "used") {
                        ok = false;
                        ND280Warn("Default hits are not named used: "
                                   << fullName);
                    }
                    ND::THandle<ND::THitSelection> used
                        = (*r)->GetHitSelection("used");
                    if (!used) {
                        ok = false;
                        ND280Error("Used hits selection does not exist: "
                                   << fullName);
                    }
                }
            }

            if (ok) continue;
            std::cout << "FAIL: " << fullName << std::endl;
        }

        for (std::vector<ND::TReconObjectContainer*>::const_iterator r 
                 = reconObjs.begin();
             r != reconObjs.end();
             ++r) {
            std::string fullName((*r)->GetFullName());

            ND::THandle<ND::TReconObjectContainer> results(*r);
            results.Release();

            if (CheckReconObjects(fullName, results)) continue;

            std::cout << "FAIL ReconObjectContainer: " << fullName << std::endl;
        }

        return true;
    }

};

int main(int argc, char **argv) {
    TCheckFits userCode;
    ND::nd280EventLoop(argc,argv,userCode,1);
}

