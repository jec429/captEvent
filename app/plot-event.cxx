///////////////////////////////////////////////////////////////
//
// This is NOT a good example of how to access and use oaEvent.
//
// A small utility used to help debug oaEvent.  This plots information saved
// in an event in a simple, but not very friendly way.  This is NOT an event
// display (well OK, it does display events, but you should be using
// eventDisplay), but is a way to look at internals of events.
//

#include <string>
#include <iomanip>
#include <iostream> 
#include <sstream>
#include <vector> 
#include <utility>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <memory>

#include <TROOT.h>
#include <TApplication.h>
#include <TBrowser.h>
#include <TGClient.h> 
#include <TGButton.h> 
#include <TGFrame.h> 
#include <TGListBox.h> 

#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TGeoShape.h>
#include <TGeoBBox.h>

#include <TCanvas.h>
#include <TH2F.h>
#include <TBox.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPolyLine.h>
#include <TPolyMarker.h>
#include <TEllipse.h>

#include <TMath.h>
#include <TVector3.h>

#define INTERACTIVE_TEVENTFOLDER
#include "TEventFolder.hxx"
#include "TND280Input.hxx"
#include "TND280Output.hxx"

#include "TND280Log.hxx"
#include "TG4Trajectory.hxx"
#include "TG4PrimaryVertex.hxx"
#include "TG4PrimaryParticle.hxx"
#include "HEPUnits.hxx"
#include "TND280Event.hxx"
#include "TOADatabase.hxx"
#include "THit.hxx"
#include "THitSelection.hxx"
#include "TG4VHit.hxx"
#include "TG4HitSegment.hxx"
#include "THandle.hxx"
#include "TAlgorithmResult.hxx"
#include "TReconBase.hxx"
#include "TReconTrack.hxx"
#include "TReconShower.hxx"
#include "TReconCluster.hxx"
#include "TReconVertex.hxx"
#include "TReconNode.hxx"
#include "TReconPID.hxx"
#include "TCorrValues.hxx"

#include "oaEvent_version.h"

// Create a control panel frame with buttons.
class TControlPanel : public TGMainFrame { 
public:
    typedef std::vector<TObject*> CleanupVector;
    
private: 
    enum {kDraw = 57, 
          kNext, kBack, 
          kG4Hits, kG4Traj, kTHits,
          kSkip10, kSkip100, kSkip1000, kSkipn100, 
          kSave, kReject, kQuit};

    TGListBox* fResultList;

    ND::TND280Input* fInputFile;
    ND::TND280Output* fOutputFile;
    ND::TND280Output* fRejectFile;
    ND::TND280Event* fCurrentEvent;

    CleanupVector fCleanup;

    void SaveEvent(void);
    void SaveReject(void);
    void QuitProgram(void);

public: 

    TControlPanel(const TGWindow *p, UInt_t w, UInt_t h); 
    ~TControlPanel(); 
    Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2); 

    void OpenInput(const std::string& file);
    void OpenOutput(const std::string& file);
    void OpenReject(const std::string& file);
    void DrawNextEventHits(void);
    void DrawEventHits(void);
    void NextEvent(void);
    void PreviousEvent(void);
    void Skip10Event(void);
    void Skip100Event(void);
    void Skip1000Event(void);
    void Skipn100Event(void);
    void SetResultEntries(void);

    std::vector<std::string> GetResultEntries(void);

    void CleanCanvas(void);
};

class TForager {
public:
    std::pair<TVector3,TVector3> fBoundMagnet;
    std::pair<TVector3,TVector3> fBoundBasket;
    std::pair<TVector3,TVector3> fBoundP0D;
    std::pair<TVector3,TVector3> fBoundTPC1;
    std::pair<TVector3,TVector3> fBoundTPC2;
    std::pair<TVector3,TVector3> fBoundTPC3;
    std::pair<TVector3,TVector3> fBoundFGD1;
    std::pair<TVector3,TVector3> fBoundFGD2;
    std::pair<TVector3,TVector3> fBoundDsECal;
    std::pair<TVector3,TVector3> fBoundLTP0DECal;
    std::pair<TVector3,TVector3> fBoundLSP0DECal;
    std::pair<TVector3,TVector3> fBoundLBP0DECal;
    std::pair<TVector3,TVector3> fBoundRTP0DECal;
    std::pair<TVector3,TVector3> fBoundRSP0DECal;
    std::pair<TVector3,TVector3> fBoundRBP0DECal;
    std::pair<TVector3,TVector3> fBoundLTBrlECal;
    std::pair<TVector3,TVector3> fBoundLSBrlECal;
    std::pair<TVector3,TVector3> fBoundLBBrlECal;
    std::pair<TVector3,TVector3> fBoundRTBrlECal;
    std::pair<TVector3,TVector3> fBoundRSBrlECal;
    std::pair<TVector3,TVector3> fBoundRBBrlECal;
    
    TForager() {
        fBoundMagnet.first.SetXYZ(4*unit::km,4*unit::km,4*unit::km);
        fBoundMagnet.second = - fBoundMagnet.first;
        
        fBoundBasket = fBoundMagnet;
        
        fBoundP0D = fBoundMagnet;
        
        fBoundTPC1 = fBoundMagnet;
        fBoundTPC2 = fBoundMagnet;
        fBoundTPC3 = fBoundMagnet;
        
        fBoundFGD1 = fBoundMagnet;
        fBoundFGD2 = fBoundMagnet;
        
        fBoundDsECal = fBoundMagnet;
        
        fBoundLTP0DECal = fBoundMagnet;
        fBoundLSP0DECal = fBoundMagnet;
        fBoundLBP0DECal = fBoundMagnet;
        fBoundRTP0DECal = fBoundMagnet;
        fBoundRSP0DECal = fBoundMagnet;
        fBoundRBP0DECal = fBoundMagnet;
        
        fBoundLTBrlECal = fBoundMagnet;
        fBoundLSBrlECal = fBoundMagnet;
        fBoundLBBrlECal = fBoundMagnet;
        fBoundRTBrlECal = fBoundMagnet;
        fBoundRSBrlECal = fBoundMagnet;
        fBoundRBBrlECal = fBoundMagnet;
    }
    virtual ~TForager() {}
    
    /// A non-virtual function to visit every node in the geometry.
    void VisitGeometry() {
        ND::TOADatabase::Get().Geometry();
        gGeoManager->PushPath();
        gGeoManager->CdTop();
        RecurseGeometry(0,"");
        gGeoManager->PopPath();
    }

    /// Provide a convenient way to get the global coordinate of a node.
    TVector3 LocalToMaster(const TVector3& local) {
        double loc[3] = {local.X(), local.Y(), local.Z()};
        double mas[3];
        gGeoManager->LocalToMaster(loc,mas);
        return TVector3(mas[0], mas[1], mas[2]);
    }

    /// A user provided virtual function to process a particular node which
    /// returns true if the node daughters should be visited.  The VisitNode
    /// method must not change the state of the geometry.  If it needs to
    /// manipulate the geometry, then use gGeoManager->PushPath(), and
    /// gGeoManager->PopPath() to save and restore the current state.  The
    /// global coordinates of the current node can be found using
    /// TGeoNode::LocalToMaster(double [3]), and (direction) vectors in the
    /// local coordinate system can be translated to the global coordinate
    /// system using TGeoNode::LocalToMasterVect(double [3]);
    bool VisitNode(int, 
                   const std::string& name, 
                   const TGeoNode* node) {
        
        TGeoVolume *volume = node->GetVolume();
        TGeoBBox* shape = dynamic_cast<TGeoBBox*>(volume->GetShape());
        TVector3 high(shape->GetDX(), shape->GetDY(), shape->GetDZ());
        TVector3 low = -high;

        if (name.find("Magnet") != std::string::npos) {
            Bound(fBoundMagnet,low,high);
        }

        if (name.find("P0D_") != std::string::npos
            && name.find("Bar_") != std::string::npos) {
            Bound(fBoundP0D,low,high);
            return false;
        }
        if (name.find("TPC1") != std::string::npos
            && name.find("Drift") != std::string::npos) {
            Bound(fBoundTPC1,low,high);
            return false;
        }
        if (name.find("TPC2") != std::string::npos
            && name.find("Drift") != std::string::npos) {
            Bound(fBoundTPC2,low,high);
            return false;
        }
        if (name.find("TPC3") != std::string::npos
            && name.find("Drift") != std::string::npos) {
            Bound(fBoundTPC3,low,high);
            return false;
        }
        if (name.find("FGD1") != std::string::npos
            && name.find("/Bar_") != std::string::npos) {
            Bound(fBoundFGD1,low,high);
            return false;
        }
        if (name.find("FGD2") != std::string::npos
            && name.find("/Bar_") != std::string::npos) {
            Bound(fBoundFGD2,low,high);
            return false;
        }
        if (name.find("DsECal") != std::string::npos) {
            Bound(fBoundDsECal,low,high);
            return false;
        }

        if (name.find("/LeftClam_") != std::string::npos
            && name.find("/P0DECal_") != std::string::npos
            && name.find("/Top_") != std::string::npos) {
            Bound(fBoundLTP0DECal,low,high);
            return false;
        }
        if (name.find("/LeftClam_") != std::string::npos
            && name.find("/P0DECal_") != std::string::npos
            && name.find("/Side_") != std::string::npos) {
            Bound(fBoundLSP0DECal,low,high);
            return false;
        }
        if (name.find("/LeftClam_") != std::string::npos
            && name.find("/P0DECal_") != std::string::npos
            && name.find("/Bottom_") != std::string::npos) {
            Bound(fBoundLBP0DECal,low,high);
            return false;
        }
        if (name.find("/RightClam_") != std::string::npos
            && name.find("/P0DECal_") != std::string::npos
            && name.find("/Top_") != std::string::npos) {
            Bound(fBoundRTP0DECal,low,high);
            return false;
        }
        if (name.find("/RightClam_") != std::string::npos
            && name.find("/P0DECal_") != std::string::npos
            && name.find("/Side_") != std::string::npos) {
            Bound(fBoundRSP0DECal,low,high);
            return false;
        }
        if (name.find("/RightClam_") != std::string::npos
            && name.find("/P0DECal_") != std::string::npos
            && name.find("/Bottom_") != std::string::npos) {
            Bound(fBoundRBP0DECal,low,high);
            return false;
        }
            
        if (name.find("/LeftClam_") != std::string::npos
            && name.find("/BrlECal_") != std::string::npos
            && name.find("/Top_") != std::string::npos) {
            Bound(fBoundLTBrlECal,low,high);
            return false;
        }
        if (name.find("/LeftClam_") != std::string::npos
            && name.find("/BrlECal_") != std::string::npos
            && name.find("/Side_") != std::string::npos) {
            Bound(fBoundLSBrlECal,low,high);
            return false;
        }
        if (name.find("/LeftClam_") != std::string::npos
            && name.find("/BrlECal_") != std::string::npos
            && name.find("/Bottom_") != std::string::npos) {
            Bound(fBoundLBBrlECal,low,high);
            return false;
        }
        if (name.find("/RightClam_") != std::string::npos
            && name.find("/BrlECal_") != std::string::npos
            && name.find("/Top_") != std::string::npos) {
            Bound(fBoundRTBrlECal,low,high);
            return false;
        }
        if (name.find("/RightClam_") != std::string::npos
            && name.find("/BrlECal_") != std::string::npos
            && name.find("/Side_") != std::string::npos) {
            Bound(fBoundRSBrlECal,low,high);
            return false;
        }
        if (name.find("/RightClam_") != std::string::npos
            && name.find("/BrlECal_") != std::string::npos
            && name.find("/Bottom_") != std::string::npos) {
            Bound(fBoundRBBrlECal,low,high);
            return false;
        }
        return true;
    }

private:
    void Bound(std::pair<TVector3,TVector3>& bnd, 
               const TVector3& c1, 
               const TVector3& c2) {
        TVector3 low = LocalToMaster(c1);
        TVector3 high = LocalToMaster(c2);
        for (int i=0; i<3; ++i) {
            bnd.first[i] = std::min(bnd.first[i],low[i]);
            bnd.first[i] = std::min(bnd.first[i],high[i]);
            bnd.second[i] = std::max(bnd.second[i],low[i]);
            bnd.second[i] = std::max(bnd.second[i],high[i]);
        }
    }

    void RecurseGeometry(int depth, std::string name)  {
        TGeoNode* node = gGeoManager->GetCurrentNode();
        name = name + "/" + node->GetName();
        
        if (!VisitNode(depth, name, node)) {
            gGeoManager->CdUp();
            return;
        }
        
        // Push all of the daughters onto the stack.
        for (int i=0; i<node->GetNdaughters(); ++i) {
            gGeoManager->CdDown(i);
            RecurseGeometry(depth + 1, name);
        }
        
        gGeoManager->CdUp();
    }
};

// Reset the drawing to the original zoom factor.
void ResetZoom();

// Print generic information about the event to std:cout.
void DisplayEventInfo(ND::TND280Event& event);

// Increment the color index to the next "good" color.
int increment_color(int cindex);

// Draw the hits and only the hits.
bool DrawHits(TControlPanel::CleanupVector& cleanup, 
              ND::TND280Event& event);
// Draw the hits and the selected result.
bool DrawResultsHits(TControlPanel::CleanupVector& cleanup, 
                     ND::TND280Event& event);
// Draw the hits and the truth information.
bool DrawTruthHits(TControlPanel::CleanupVector& cleanup, 
                   ND::TND280Event& event);
// Draw the hits, results and truth information.
bool DrawAllHits(TControlPanel::CleanupVector& cleanup, 
                 ND::TND280Event& event);
void LoopOverTHits(TControlPanel::CleanupVector& cleanup, 
                   ND::THitSelection& hits,int color=1,int fill=0);

// Find the average time of the hits.
double AverageHitTime(const ND::THitSelection& hits);

void ShowG4Hits(TControlPanel::CleanupVector& cleanup, 
                ND::TND280Event& event);
void ShowTHits(TControlPanel::CleanupVector& cleanup, 
               ND::TND280Event& event);
void ShowG4Traj(TControlPanel::CleanupVector& cleanup, 
                ND::TND280Event& event);
void ShowPrimaries(TControlPanel::CleanupVector& cleanup, 
                   ND::TND280Event& event);
void ShowResults(TControlPanel::CleanupVector& cleanup, 
                 ND::TND280Event& event,
                 const std::vector<std::string>& results);
void ShowReconObject(TControlPanel::CleanupVector& graphicsDelete,
                     ND::THandle<ND::TReconBase> recon,
                     int color,
                     int baseColor);
int ShowReconObjects(TControlPanel::CleanupVector& graphicsDelete,
                     ND::THandle<ND::TReconObjectContainer>& recon,
                     int color, bool increm=true);
void ShowReconPID(TControlPanel::CleanupVector& graphicsDelete,
                  ND::THandle<ND::TReconPID>& recon,
                  int color, int baseColor);
void ShowReconTrack(TControlPanel::CleanupVector& graphicsDelete,
                    ND::THandle<ND::TReconTrack>& recon,
                    int color, int baseColor);
void ShowReconShower(TControlPanel::CleanupVector& graphicsDelete,
                     ND::THandle<ND::TReconShower>& recon,
                     int color, int baseColor);
void ShowReconCluster(TControlPanel::CleanupVector& graphicsDelete,
                      ND::THandle<ND::TReconCluster>& recon,
                      int color, int baseColor);

std::string ShowVariance(double var, double unit) {
    if (ND::TCorrValues::IsFree(var)) return "(free)";
    if (ND::TCorrValues::IsFixed(var)) return "(fixed)";
    std::ostringstream variance;

    variance << "+- " << std::setprecision(2) << std::sqrt(var)/unit;

    return variance.str();
}

TBox* BoundingBoxXZ(const std::pair<TVector3,TVector3>& bnd);
TBox* BoundingBoxYZ(const std::pair<TVector3,TVector3>& bnd);
std::vector<std::string> gDefaultAlgorithmNames;
std::vector<double> gSelectedTimes;

void usage(std::string programName) {
    std::cout << "usage: " << programName 
              << " [options] [input-file] ..." 
              << std::endl
              << std::endl
              << "Options:"
              << std::endl
              << "    -o <file>         Set the name of an output file."
              << std::endl
              << "    -R <name>         File name for rejected events"
              << std::endl
              << "    -a                Show the whole detector [default]"
              << std::endl
              << "    -t                Show the Tracker"
              << std::endl
              << "    -p                Show the P0D"
              << std::endl
              << "    -r <name>         Name of algorithm result to show"
              << std::endl
              << "    -s [HTPVh]        What to show (H: G4Hits, T: G4Traj"
              << std::endl
              << "                      P: G4TrajPnts, V: Primaries, h: THits)"
              << std::endl
              << "    -v                Increase the verbosity"
              << std::endl << std::endl;
    exit(1);
}

namespace canvas {
    TCanvas *main = NULL;
    TH2* sideView = NULL;
    TH2* topView = NULL;

    std::pair<TVector3,TVector3> currentBounds;
    
    bool showG4Hits = false;
    bool showG4Traj = false;
    bool showG4TrajPnt = false;
    bool showG4Prim = false;
    bool showTHits = false;

    bool showP0D = true;
    bool showTracker = false;

    TForager* forager = NULL;
    TBox* sideMagnet = NULL;
    TBox* topMagnet = NULL;
    TBox* sideP0D = NULL;
    TBox* topP0D = NULL;
    TBox* sideTPC1 = NULL;
    TBox* topTPC1 = NULL;
    TBox* sideTPC2 = NULL;
    TBox* topTPC2 = NULL;
    TBox* sideTPC3 = NULL;
    TBox* topTPC3 = NULL;
    TBox* sideFGD1 = NULL;
    TBox* topFGD1 = NULL;
    TBox* sideFGD2 = NULL;
    TBox* topFGD2 = NULL;
    TBox* sideDsECal = NULL;
    TBox* topDsECal = NULL;

    TBox* sideLTP0DECal = NULL;
    TBox* topLTP0DECal = NULL;
    TBox* sideLSP0DECal = NULL;
    TBox* topLSP0DECal = NULL;
    TBox* sideLBP0DECal = NULL;
    TBox* topLBP0DECal = NULL;
    TBox* sideRTP0DECal = NULL;
    TBox* topRTP0DECal = NULL;
    TBox* sideRSP0DECal = NULL;
    TBox* topRSP0DECal = NULL;
    TBox* sideRBP0DECal = NULL;
    TBox* topRBP0DECal = NULL;

    TBox* sideLTBrlECal = NULL;
    TBox* topLTBrlECal = NULL;
    TBox* sideLSBrlECal = NULL;
    TBox* topLSBrlECal = NULL;
    TBox* sideLBBrlECal = NULL;
    TBox* topLBBrlECal = NULL;
    TBox* sideRTBrlECal = NULL;
    TBox* topRTBrlECal = NULL;
    TBox* sideRSBrlECal = NULL;
    TBox* topRSBrlECal = NULL;
    TBox* sideRBBrlECal = NULL;
    TBox* topRBBrlECal = NULL;
};

int main(int argc, char **argv) {
    int logLevel = -1; // Will choose default logging level...
    std::string outputName = "";
    std::string rejectName = "";
    std::string drawRegion = "";
    bool overrideDefaultAlgorithm = false;
    gDefaultAlgorithmNames.push_back("oaRecon/final");

    const char* options = "actpR:r:o:s:v";
    while (true) {
        int option = getopt(argc,argv,options);
        if (option<0) break;
        switch (option) {
        case 'a':
            drawRegion = "magnet";
            break;
        case 't':
            drawRegion = "tracker";
            break;
        case 'p':
            drawRegion = "p0d";
            break;
        case 'o':
            outputName = optarg;
            break;
        case 'R':
            rejectName = optarg;
            break;
        case 'r':
            if (!overrideDefaultAlgorithm) {
                overrideDefaultAlgorithm = true;
                gDefaultAlgorithmNames.clear();
            }
            gDefaultAlgorithmNames.push_back(optarg);
            break;
        case 's':
        {
            std::string tmp = optarg;
            canvas::showG4Hits = (tmp.find("H") != std::string::npos);
            canvas::showG4Traj = (tmp.find("T") != std::string::npos);
            canvas::showG4TrajPnt = (tmp.find("P") != std::string::npos);
            canvas::showG4Prim = (tmp.find("V") != std::string::npos);
            canvas::showTHits = (tmp.find("h") != std::string::npos);
            break;
        }
        case 'v':
        {
            // increase the verbosity level.
            if (logLevel>0) ++logLevel;
            else logLevel = 2;
            break;
        }
        default:
            usage(argv[0]);
        }
    }
 
    if (argc<=optind) usage(argv[0]);

    // Open input the file
    std::string inputName = argv[optind];

    //Initialize GUI
    TApplication theApp("App",&argc,argv);

    // Set up the logging code.
    ND::TND280Log::Configure();
    
    if (logLevel == 0) {
        ND::TND280Log::SetLogLevel(ND::TND280Log::QuietLevel);
    }
    else if (logLevel == 1) {
        ND::TND280Log::SetLogLevel(ND::TND280Log::LogLevel);
        ND280Log("Set log level to LogLevel");
    }
    else if (logLevel == 2) {
        ND::TND280Log::SetLogLevel(ND::TND280Log::InfoLevel);
        ND280Info("Set log level to InfoLevel");
    }
    else if (logLevel >= 3) {
        ND::TND280Log::SetLogLevel(ND::TND280Log::VerboseLevel);
        ND280Verbose("Set log level to VerboseLevel");
    }
    
    ND::TEventFolder::GetEventFolder();

    canvas::main = new TCanvas ("canvas::main","T2K ND280 Event Display",
                                0,100,700,900);
  
    TControlPanel* control = new TControlPanel(gClient->GetRoot(), 700, 700);
    control->OpenInput(inputName);
    if (outputName != "") control->OpenOutput(outputName);
    if (rejectName != "") control->OpenReject(rejectName);
    
    canvas::forager = new TForager;
    canvas::forager->VisitGeometry();

    if (drawRegion == "tracker") {
        ND280Log("Limit drawing to the tracker");
        canvas::showTracker = true;
        canvas::showP0D = false;
        canvas::currentBounds.first[0] 
            = canvas::forager->fBoundRSBrlECal.first.X() - 10*unit::cm;
        canvas::currentBounds.first[1]
            = canvas::forager->fBoundRBBrlECal.first.Y() - 10*unit::cm;
        canvas::currentBounds.first[2] 
            = std::min(canvas::forager->fBoundRBBrlECal.first.Z()-10*unit::cm,
                       canvas::forager->fBoundTPC1.first.Z()) - 10*unit::cm;
        canvas::currentBounds.second[0]
            = canvas::forager->fBoundLSBrlECal.second.X() + 10*unit::cm;
        canvas::currentBounds.second[1]
            = canvas::forager->fBoundLTBrlECal.second.Y() + 10*unit::cm;
        canvas::currentBounds.second[2]
            = std::max(canvas::forager->fBoundLTBrlECal.second.Z()+10*unit::cm,
                       canvas::forager->fBoundDsECal.second.Z()) + 10*unit::cm;
    }
    else if (drawRegion == "p0d") {
        ND280Log("Limit drawing to the P0D");
        canvas::showTracker = false;
        canvas::showP0D = true;
        canvas::currentBounds.first[0]
            = std::min(canvas::forager->fBoundRSP0DECal.first.X(),
                       canvas::forager->fBoundP0D.first.X()) - 10*unit::cm;
        canvas::currentBounds.first[1]
            = std::min(canvas::forager->fBoundRBP0DECal.first.Y(),
                       canvas::forager->fBoundP0D.first.Y()) - 10*unit::cm;
        canvas::currentBounds.first[2]
            = std::min(canvas::forager->fBoundRBP0DECal.first.Z(),
                       canvas::forager->fBoundP0D.first.Z()) - 10*unit::cm;
        canvas::currentBounds.second[0]
            = std::max(canvas::forager->fBoundLSP0DECal.second.X(),
                       canvas::forager->fBoundP0D.second.X()) + 10*unit::cm;
        canvas::currentBounds.second[1]
            = std::max(canvas::forager->fBoundLTP0DECal.second.Y(),
                       canvas::forager->fBoundP0D.second.Y()) + 10*unit::cm;
        canvas::currentBounds.second[2]
            = canvas::forager->fBoundP0D.second.Z() + 10*unit::cm;
    }
    else {
        ND280Log("Show the full magnet");
        canvas::currentBounds.first[0]
            = canvas::forager->fBoundMagnet.first.X() - 10*unit::cm;
        canvas::currentBounds.first[1]
            = canvas::forager->fBoundMagnet.first.Y() - 10*unit::cm;
        canvas::currentBounds.first[2]
            = canvas::forager->fBoundMagnet.first.Z() - 10*unit::cm;
        canvas::currentBounds.second[0]
            = canvas::forager->fBoundMagnet.second.X() + 10*unit::cm;
        canvas::currentBounds.second[1]
            = canvas::forager->fBoundMagnet.second.Y() + 10*unit::cm;
        canvas::currentBounds.second[2]
            = canvas::forager->fBoundMagnet.second.Z() + 10*unit::cm;
    }        
        
    canvas::topView  = new TH2F("canvas::topView","Top (X-Z)",
                                100, canvas::currentBounds.first[2]/unit::cm,
                                canvas::currentBounds.second[2]/unit::cm,
                                100, canvas::currentBounds.first[0]/unit::cm,
                                canvas::currentBounds.second[0]/unit::cm); 
    canvas::topView->SetStats(false);
    canvas::topMagnet = BoundingBoxXZ(canvas::forager->fBoundMagnet);
    canvas::topP0D = BoundingBoxXZ(canvas::forager->fBoundP0D);
    canvas::topTPC1 = BoundingBoxXZ(canvas::forager->fBoundTPC1);
    canvas::topTPC2 = BoundingBoxXZ(canvas::forager->fBoundTPC2);
    canvas::topTPC3 = BoundingBoxXZ(canvas::forager->fBoundTPC3);
    canvas::topFGD1 = BoundingBoxXZ(canvas::forager->fBoundFGD1);
    canvas::topFGD2 = BoundingBoxXZ(canvas::forager->fBoundFGD2);
    canvas::topDsECal = BoundingBoxXZ(canvas::forager->fBoundDsECal);
    canvas::topLTP0DECal = BoundingBoxXZ(canvas::forager->fBoundLTP0DECal);
    canvas::topLSP0DECal = BoundingBoxXZ(canvas::forager->fBoundLSP0DECal);
    canvas::topLBP0DECal = BoundingBoxXZ(canvas::forager->fBoundLBP0DECal);
    canvas::topRTP0DECal = BoundingBoxXZ(canvas::forager->fBoundRTP0DECal);
    canvas::topRSP0DECal = BoundingBoxXZ(canvas::forager->fBoundRSP0DECal);
    canvas::topRBP0DECal = BoundingBoxXZ(canvas::forager->fBoundRBP0DECal);
    canvas::topLTBrlECal = BoundingBoxXZ(canvas::forager->fBoundLTBrlECal);
    canvas::topLSBrlECal = BoundingBoxXZ(canvas::forager->fBoundLSBrlECal);
    canvas::topLBBrlECal = BoundingBoxXZ(canvas::forager->fBoundLBBrlECal);
    canvas::topRTBrlECal = BoundingBoxXZ(canvas::forager->fBoundRTBrlECal);
    canvas::topRSBrlECal = BoundingBoxXZ(canvas::forager->fBoundRSBrlECal);
    canvas::topRBBrlECal = BoundingBoxXZ(canvas::forager->fBoundRBBrlECal);

    canvas::sideView = new TH2F("canvas::sideView","Side (Y-Z)",
                                100, canvas::currentBounds.first[2]/unit::cm,
                                canvas::currentBounds.second[2]/unit::cm,
                                100, canvas::currentBounds.first[1]/unit::cm,
                                canvas::currentBounds.second[1]/unit::cm); 
    canvas::sideView->SetStats(false);
    canvas::sideMagnet = BoundingBoxYZ(canvas::forager->fBoundMagnet);
    canvas::sideP0D = BoundingBoxYZ(canvas::forager->fBoundP0D);
    canvas::sideTPC1 = BoundingBoxYZ(canvas::forager->fBoundTPC1);
    canvas::sideTPC2 = BoundingBoxYZ(canvas::forager->fBoundTPC2);
    canvas::sideTPC3 = BoundingBoxYZ(canvas::forager->fBoundTPC3);
    canvas::sideFGD1 = BoundingBoxYZ(canvas::forager->fBoundFGD1);
    canvas::sideFGD2 = BoundingBoxYZ(canvas::forager->fBoundFGD2);
    canvas::sideDsECal = BoundingBoxYZ(canvas::forager->fBoundDsECal);
    canvas::sideLTP0DECal = BoundingBoxYZ(canvas::forager->fBoundLTP0DECal);
    canvas::sideLSP0DECal = BoundingBoxYZ(canvas::forager->fBoundLSP0DECal);
    canvas::sideLBP0DECal = BoundingBoxYZ(canvas::forager->fBoundLBP0DECal);
    canvas::sideRTP0DECal = BoundingBoxYZ(canvas::forager->fBoundRTP0DECal);
    canvas::sideRSP0DECal = BoundingBoxYZ(canvas::forager->fBoundRSP0DECal);
    canvas::sideRBP0DECal = BoundingBoxYZ(canvas::forager->fBoundRBP0DECal);
    canvas::sideLTBrlECal = BoundingBoxYZ(canvas::forager->fBoundLTBrlECal);
    canvas::sideLSBrlECal = BoundingBoxYZ(canvas::forager->fBoundLSBrlECal);
    canvas::sideLBBrlECal = BoundingBoxYZ(canvas::forager->fBoundLBBrlECal);
    canvas::sideRTBrlECal = BoundingBoxYZ(canvas::forager->fBoundRTBrlECal);
    canvas::sideRSBrlECal = BoundingBoxYZ(canvas::forager->fBoundRSBrlECal);
    canvas::sideRBBrlECal = BoundingBoxYZ(canvas::forager->fBoundRBBrlECal);

    canvas::main->Divide(1,2);
    
    control->CleanCanvas();
    canvas::main->Update(); 
    
    control->SetResultEntries();
    control->DrawEventHits();
    
    theApp.Run();
    
    return 0;
}

bool DrawAllHits(TControlPanel::CleanupVector& graphicsDelete,
                 ND::TND280Event& event) {
    ResetZoom();

    if (!canvas::showG4Hits 
        && !canvas::showG4Traj 
        && !canvas::showG4TrajPnt
        && !canvas::showG4Prim
        && !canvas::showTHits) {
        canvas::showG4Hits = true;
        canvas::showTHits = true;
    }

    if (canvas::showG4Hits) ShowG4Hits(graphicsDelete,event);

    if (canvas::showTHits) ShowTHits(graphicsDelete,event);

    if (canvas::showG4Traj) ShowG4Traj(graphicsDelete, event);

    if (canvas::showG4Prim) ShowPrimaries(graphicsDelete, event);

    return true;
}
  
void ResetZoom() {
    canvas::sideView->GetXaxis()->UnZoom();
    canvas::sideView->GetYaxis()->UnZoom();
    canvas::topView->GetXaxis()->UnZoom();
    canvas::topView->GetYaxis()->UnZoom();
}

double AverageHitTime(const ND::THitSelection& hits) {
    double sumT = 0;
    double sumQ = 0;
    for (ND::THitSelection::const_iterator h = hits.begin();
         h != hits.end();
         ++h) {
        if (!(*h)->HasValidTime()) continue;
        sumT += (*h)->GetTime()*(*h)->GetCharge();
        sumQ += (*h)->GetCharge();
    }
    if (sumQ<1) return 0.0;
    return sumT/sumQ;
}

void LoopOverTHits(TControlPanel::CleanupVector& graphicsDelete,
                   ND::THitSelection& hits, int color, int fill) {  

    if (hits.size() < 1) return;

    ND::TND280Log::IncreaseIndentation();
    ND280Log("Hits " << hits.size());

    double maxQ = hits.front()->GetCharge();
    double minQ = maxQ;
    for (ND::THitSelection::const_iterator h = hits.begin(); 
         h != hits.end();
         ++h) {
        maxQ = std::max(maxQ, (*h)->GetCharge());
        minQ = std::min(minQ, (*h)->GetCharge());
    }

    maxQ = std::max(maxQ, minQ+1E-6);

    if (!(maxQ>minQ)) {
        ND280Severe("Bad Charges in " << hits.GetName());
    }

    double avgT = 0.0;
    double totalQ = 0.0;
    ///////////////////////////////////////////
    canvas::main->cd(1); 
    for (ND::THitSelection::const_iterator h = hits.begin(); 
         h != hits.end();
         ++h) {
        // Get the hit information.
        TVector3 pos = (*h)->GetPosition();
        double charge =  (*h)->GetCharge();
        double size = 0.4;
        totalQ += charge;
        avgT += charge*(*h)->GetTime();

        if (pos.X() < canvas::currentBounds.first.X()) continue;
        if (pos.Y() < canvas::currentBounds.first.Y()) continue;
        if (pos.Z() < canvas::currentBounds.first.Z()) continue;
        if (pos.X() > canvas::currentBounds.second.X()) continue;
        if (pos.Y() > canvas::currentBounds.second.Y()) continue;
        if (pos.Z() > canvas::currentBounds.second.Z()) continue;

        // Select hits with times near the selected times.
        if (gSelectedTimes.size()>0) {
            bool found = false;
            for (std::vector<double>::iterator t = gSelectedTimes.begin();
                 t != gSelectedTimes.end();
                 ++t) {
                if (std::abs((*h)->GetTime()-(*t)) < 100*unit::ns) {
                    found = true;
                    break;
                }
            }
            if (!found) continue;
        }

        size += 1*(sqrt(1+(charge-minQ)/(maxQ-minQ))-1);
        
        if (!(maxQ>minQ)) size = 2.0;

        if (fill<0) size += 1.0;
        
        if ((*h)->IsXHit() && (*h)->IsZHit()) {
            if (pos.X() < canvas::forager->fBoundMagnet.first.X()
                || pos.Y() < canvas::forager->fBoundMagnet.first.Y()
                || pos.Z() < canvas::forager->fBoundMagnet.first.Z()
                || pos.X() > canvas::forager->fBoundMagnet.second.X()
                || pos.Y() > canvas::forager->fBoundMagnet.second.Y()
                || pos.Z() > canvas::forager->fBoundMagnet.second.Z()) {
                ND280Error("Hit out of bounds for " << hits.GetName());
            }
            TBox *px = new TBox(pos.Z()/unit::cm-size,
                                pos.X()/unit::cm-size,
                                pos.Z()/unit::cm+size,
                                pos.X()/unit::cm+size); 
            px->SetLineStyle(1);
            px->SetLineWidth(1);
            px->SetLineColor(color);
            if (fill>0) {
                px->SetFillStyle(fill);
                px->SetFillColor(color);
            }
            else {
                px->SetFillStyle(0);
            }
            graphicsDelete.push_back(px); 
            px->Draw();
        }
    }
    gPad->Update();
    
    /////////////////////////////////
    canvas::main->cd(2); 
    for (ND::THitSelection::const_iterator h = hits.begin(); 
         h != hits.end();
         ++h) {
        // Get the hit information.
        TVector3 pos = (*h)->GetPosition();
        double charge =  (*h)->GetCharge();

        if (pos.X() < canvas::currentBounds.first.X()) continue;
        if (pos.Y() < canvas::currentBounds.first.Y()) continue;
        if (pos.Z() < canvas::currentBounds.first.Z()) continue;
        if (pos.X() > canvas::currentBounds.second.X()) continue;
        if (pos.Y() > canvas::currentBounds.second.Y()) continue;
        if (pos.Z() > canvas::currentBounds.second.Z()) continue;

        // Select hits with times near the selected times.
        if (gSelectedTimes.size()>0) {
            bool found = false;
            for (std::vector<double>::iterator t = gSelectedTimes.begin();
                 t != gSelectedTimes.end();
                 ++t) {
                if (std::abs((*h)->GetTime()-(*t)) < 100*unit::ns) {
                    found = true;
                    break;
                }
            }
            if (!found) continue;
        }

        double size = 0.4;
        size += 1*(sqrt(1+(charge-minQ)/(maxQ-minQ))-1);
        if (!(maxQ>minQ)) size = 2.0;

        if (fill<0) size += 1.0;

        if ((*h)->IsYHit() && (*h)->IsZHit()) {
            if (pos.X() < canvas::forager->fBoundMagnet.first.X()
                || pos.Y() < canvas::forager->fBoundMagnet.first.Y()
                || pos.Z() < canvas::forager->fBoundMagnet.first.Z()
                || pos.X() > canvas::forager->fBoundMagnet.second.X()
                || pos.Y() > canvas::forager->fBoundMagnet.second.Y()
                || pos.Z() > canvas::forager->fBoundMagnet.second.Z()) {
                ND280Error("Hit out of bounds for " << hits.GetName());
            }
            TBox *py = new TBox(pos.Z()/unit::cm-size,
                                pos.Y()/unit::cm-size,
                                pos.Z()/unit::cm+size,
                                pos.Y()/unit::cm+size); 
            py->SetLineStyle(1);
            py->SetLineWidth(1);
            py->SetLineColor(color);
            if (fill>0) {
                py->SetFillStyle(fill);
                py->SetFillColor(color);
            }
            else {
                py->SetFillStyle(0);
            }
            graphicsDelete.push_back(py); 
            py->Draw(); 
        }
    }
    gPad->Update();
    
    canvas::main->Update(); 
    if (totalQ>0) {
        ND280Log(std::fixed << std::setprecision(0) 
                 << "Hits: " << hits.size()
                 << "    Average Hit Time: " 
                 << avgT/totalQ/unit::ns << " ns");
    }
    ND::TND280Log::DecreaseIndentation();
}

void LoopOverG4Hits(TControlPanel::CleanupVector& graphicsDelete,
                    ND::TG4HitContainer& hits, 
                    int color=1) {  

    ND::TND280Log::IncreaseIndentation();
    ND280Log("G4Hits: " << hits.size());
    ///////////////////////////////////////////
    // Handle the XZ projection.
    canvas::main->cd(1); 
    for (ND::TG4HitContainer::const_iterator h = hits.begin(); 
         h != hits.end();
         ++h) {
        const ND::TG4HitSegment* seg 
            = dynamic_cast<const ND::TG4HitSegment*>((*h));

        if (!seg) {
            ND280Warn("TG4Hit can't be cast as TG4HitSegment.  Not showing");
            continue;
        }

        double energy = seg->GetEnergyDeposit();
        if (seg->GetTrackLength()>0.1*unit::mm) {
            energy /= seg->GetTrackLength();
        }
        
        if (seg->GetStartX() < canvas::currentBounds.first.X()) continue;
        if (seg->GetStartY() < canvas::currentBounds.first.Y()) continue;
        if (seg->GetStartZ() < canvas::currentBounds.first.Z()) continue;
        if (seg->GetStartX() > canvas::currentBounds.second.X()) continue;
        if (seg->GetStartY() > canvas::currentBounds.second.Y()) continue;
        if (seg->GetStartZ() > canvas::currentBounds.second.Z()) continue;

        TLine *px = new TLine(seg->GetStartZ()/unit::cm,
                              seg->GetStartX()/unit::cm,
                              seg->GetStopZ()/unit::cm,
                              seg->GetStopX()/unit::cm); 
        px->SetLineColor(color);
        px->SetLineStyle(1);
        int width = int(std::min(1.0,energy/(2*unit::MeV)+0.5));
        px->SetLineWidth(width);
        graphicsDelete.push_back(px); 
        px->Draw();
    }
    gPad->Update();

    ///////////////////////////////////////////
    // Handle the XY projection.
    canvas::main->cd(2); 
    for (ND::TG4HitContainer::const_iterator h = hits.begin(); 
         h != hits.end();
         ++h) {
        const ND::TG4HitSegment* seg 
            = dynamic_cast<const ND::TG4HitSegment*>((*h));

        if (!seg) {
            ND280Warn("TG4Hit can't be cast as TG4HitSegment.  Not showing");
            continue;
        }

        double energy = seg->GetEnergyDeposit();
        if (seg->GetTrackLength()>0.1*unit::mm) {
            energy /= seg->GetTrackLength();
        }
        
        if (seg->GetStartX() < canvas::currentBounds.first.X()) continue;
        if (seg->GetStartY() < canvas::currentBounds.first.Y()) continue;
        if (seg->GetStartZ() < canvas::currentBounds.first.Z()) continue;
        if (seg->GetStartX() > canvas::currentBounds.second.X()) continue;
        if (seg->GetStartY() > canvas::currentBounds.second.Y()) continue;
        if (seg->GetStartZ() > canvas::currentBounds.second.Z()) continue;

        TLine *py = new TLine(seg->GetStartZ()/unit::cm,
                              seg->GetStartY()/unit::cm,
                              seg->GetStopZ()/unit::cm,
                              seg->GetStopY()/unit::cm); 
        py->SetLineColor(color);
        py->SetLineStyle(1);
        int width = int(std::min(1.0,energy/(2*unit::MeV)+0.5));
        py->SetLineWidth(width);
        graphicsDelete.push_back(py); 
        py->Draw();
    }
    gPad->Update();
    
    ND::TND280Log::DecreaseIndentation();
    canvas::main->Update(); 
}

void DrawTrajectoryXZ(TControlPanel::CleanupVector& cleanup, 
                      ND::TG4Trajectory& particle,
                      int color, 
                      int width,
                      int style,
                      double length = -1.0) {
    ND::TG4Trajectory::Points points = particle.GetTrajectoryPoints();
    TPolyLine* xzTraj = new TPolyLine;
    cleanup.push_back(xzTraj);
    TPolyMarker* xzMark = NULL;
    if (canvas::showG4TrajPnt) {
        xzMark = new TPolyMarker;
        cleanup.push_back(xzMark);
    }
    TVector3 start = points.front().GetPosition().Vect();
    for (ND::TG4Trajectory::Points::iterator p = points.begin();
         p != points.end();
         ++p) {
        if (length>0 
            && (p->GetPosition().Vect() - start).Mag() > length
            && xzTraj->GetN()>1) continue;
        if (p->GetPosition().X() < canvas::currentBounds.first.X()) continue;
        if (p->GetPosition().Y() < canvas::currentBounds.first.Y()) continue;
        if (p->GetPosition().Z() < canvas::currentBounds.first.Z()) continue;
        if (p->GetPosition().X() > canvas::currentBounds.second.X()) continue;
        if (p->GetPosition().Y() > canvas::currentBounds.second.Y()) continue;
        if (p->GetPosition().Z() > canvas::currentBounds.second.Z()) continue;
        xzTraj->SetNextPoint(p->GetPosition().Z()/unit::cm,
                             p->GetPosition().X()/unit::cm);
        if (xzMark) xzMark->SetNextPoint(p->GetPosition().Z()/unit::cm,
                                             p->GetPosition().X()/unit::cm);

    }
    xzTraj->SetLineColor(color);
    xzTraj->SetLineWidth(width);
    xzTraj->SetLineStyle(style);
    xzTraj->Draw();
    if (xzMark) {
        xzMark->SetMarkerColor(color);
        xzMark->SetMarkerSize(1);
        xzMark->SetMarkerStyle(3);
        xzMark->Draw();
    }
}

void DrawTrajectoryYZ(TControlPanel::CleanupVector& cleanup, 
                      ND::TG4Trajectory& particle,
                      int color, 
                      int width,
                      int style,
                      double length = -1.0) {
    ND::TG4Trajectory::Points points = particle.GetTrajectoryPoints();
    TPolyLine* yzTraj = new TPolyLine;
    cleanup.push_back(yzTraj);
    TPolyMarker* yzMark = NULL;
    if (canvas::showG4TrajPnt) {
        yzMark = new TPolyMarker;
        cleanup.push_back(yzMark);
    }
    TVector3 start = points.front().GetPosition().Vect();
    for (ND::TG4Trajectory::Points::iterator p = points.begin();
         p != points.end();
         ++p) {
        if (length>0 
            && (p->GetPosition().Vect() - start).Mag() > length
            && yzTraj->GetN()>1) continue;
        if (p->GetPosition().X() < canvas::currentBounds.first.X()) continue;
        if (p->GetPosition().Y() < canvas::currentBounds.first.Y()) continue;
        if (p->GetPosition().Z() < canvas::currentBounds.first.Z()) continue;
        if (p->GetPosition().X() > canvas::currentBounds.second.X()) continue;
        if (p->GetPosition().Y() > canvas::currentBounds.second.Y()) continue;
        if (p->GetPosition().Z() > canvas::currentBounds.second.Z()) continue;
        yzTraj->SetNextPoint(p->GetPosition().Z()/unit::cm,
                             p->GetPosition().Y()/unit::cm);
        if (yzMark) yzMark->SetNextPoint(p->GetPosition().Z()/unit::cm,
                                             p->GetPosition().Y()/unit::cm);
    }
    yzTraj->SetLineColor(color);
    yzTraj->SetLineWidth(width);
    yzTraj->SetLineStyle(style);
    yzTraj->Draw();
    if (yzMark) {
        yzMark->SetMarkerColor(color);
        yzMark->SetMarkerSize(1);
        yzMark->SetMarkerStyle(3);
        yzMark->Draw();
    }
}

void DrawParticle(bool xProjection,
                  TControlPanel::CleanupVector& cleanup, 
                  ND::TG4Trajectory& particle) {
    if (particle.GetParticleName() == "neutron") {
        if (particle.GetInitialMomentum().P() > 500*unit::MeV) {
            if (xProjection) DrawTrajectoryXZ(cleanup,particle,28,1,2);
            else DrawTrajectoryYZ(cleanup,particle,28,1,2);
	}
    }
    else if (particle.GetParticleName() == "proton") {
        if (particle.GetInitialMomentum().P() > 400*unit::MeV) {
            if (xProjection) DrawTrajectoryXZ(cleanup,particle,28,3,1);
            else DrawTrajectoryYZ(cleanup,particle,28,3,1);
        }
    }
    else if (particle.GetParticleName() == "gamma") {
        if (particle.GetInitialMomentum().P() > 20*unit::MeV) {
            if (xProjection) DrawTrajectoryXZ(cleanup,particle,2,1,2);
            else DrawTrajectoryYZ(cleanup,particle,2,1,2);
        }
    }
    else if (particle.GetParticleName() == "e+"
             || particle.GetParticleName() == "e-") {
        if (xProjection) DrawTrajectoryXZ(cleanup,particle,38,2,1);
        else DrawTrajectoryYZ(cleanup,particle,38,2,1);
    }
    else if (particle.GetParticleName() == "mu+"
             || particle.GetParticleName() == "mu-") {
        if (xProjection) DrawTrajectoryXZ(cleanup,particle,50,2,1);
        else DrawTrajectoryYZ(cleanup,particle,50,2,1);
    }
    else if (particle.GetParticleName() == "pi+"
             || particle.GetParticleName() == "pi-") {
        if (xProjection) DrawTrajectoryXZ(cleanup,particle,7,2,1);
        else DrawTrajectoryYZ(cleanup,particle,7,2,1);
    }
    else {
        const TParticlePDG* pdgInfo = particle.GetParticle();
        if (pdgInfo && std::abs(pdgInfo->Charge()) > 0.1) {
            if (xProjection) DrawTrajectoryXZ(cleanup,particle,16,1,1);
            else DrawTrajectoryYZ(cleanup,particle,16,1,1);
        }
        else {
            if (xProjection) DrawTrajectoryXZ(cleanup,particle,16,1,2);
            else DrawTrajectoryYZ(cleanup,particle,16,1,2);
        }            
    }
}

void ShowPrimaries(TControlPanel::CleanupVector& cleanup, 
                   ND::TND280Event& event) {
    ND::THandle<ND::TG4PrimaryVertexContainer> primaries
        = event.Get<ND::TG4PrimaryVertexContainer>("truth/G4PrimVertex00");
    if (!primaries) return;

    canvas::main->cd(1);
    for (ND::TG4PrimaryVertexContainer::const_iterator v
             = primaries->begin();
         v != primaries->end();
         ++v) {
        TLorentzVector vertex = v->GetPosition();
        TMarker* vtx = new TMarker(vertex.Z()/unit::cm, 
                                   vertex.X()/unit::cm, 28);
        vtx->SetMarkerSize(2);
        vtx->SetMarkerColor(2);
        vtx->Draw();
        cleanup.push_back(vtx);
    }
    gPad->Update();

    canvas::main->cd(2);
    for (ND::TG4PrimaryVertexContainer::const_iterator v
             = primaries->begin();
         v != primaries->end();
         ++v) {
        TLorentzVector vertex = v->GetPosition();
        TMarker* vtx = new TMarker(vertex.Z()/unit::cm, 
                                   vertex.Y()/unit::cm, 28);
        vtx->SetMarkerSize(2);
        vtx->SetMarkerColor(2);
        vtx->Draw();
        cleanup.push_back(vtx);
    }
    gPad->Update();

    canvas::main->Update();
}

void ShowTHits(TControlPanel::CleanupVector& graphicsDelete,
               ND::TND280Event& event) {

    ND::THandle<ND::TDataVector> hits = event.Get<ND::TDataVector>("hits");
    if (!hits) return;

    for (ND::TDataVector::iterator h = hits->begin();
         h != hits->end();
         ++h) {
        ND::THandle<ND::THitSelection> selection 
            = (*h)->Get<ND::THitSelection>(".");
        if (selection) {
            std::string name = selection->GetName();
            if (!canvas::showTracker 
                && name.find("p0d") == std::string::npos) continue;
            if (!canvas::showP0D 
                && name.find("p0d") != std::string::npos) continue;
            ND280Log("Show " << selection->size() 
                     << " hits for " << selection->GetFullName());
            int color = 1;
            if (name.find("pecal") != std::string::npos) color = 4;
            if (name.find("barrel") != std::string::npos) color = 4;
            if (name.find("mrd") != std::string::npos) color = 6;
            LoopOverTHits(graphicsDelete,*selection, color);
        }
    }
}
  
void ShowG4Hits(TControlPanel::CleanupVector& graphicsDelete,
                ND::TND280Event& event) {
    
    ND::THandle<ND::TDataVector> hits 
        = event.Get<ND::TDataVector>("truth/g4Hits");
    if (!hits) return;

    ND::TND280Log::IncreaseIndentation();
    for (ND::TDataVector::iterator h = hits->begin();
         h != hits->end();
         ++h) {
        ND::THandle<ND::TG4HitContainer> g4Hits =
            (*h)->Get<ND::TG4HitContainer>(".");
        if (!g4Hits) {
            ND280Error("truth/g4Hits with non-TG4HitContainer: "
                       << (*h)->GetName());
            continue;
        }
        ND280Log("Show " << g4Hits->size()
                 << " hits for " << g4Hits->GetFullName());
        int color = 4;
        std::string name = g4Hits->GetName();
        if (name.find("p0d") != std::string::npos) color = 1;
        if (name.find("tpc") != std::string::npos) color = 1;
        if (name.find("fgd") != std::string::npos) color = 1;
        if (name.find("dsecal") != std::string::npos) color = 1;
        if (name.find("mrd") != std::string::npos) color = 6;
        LoopOverG4Hits(graphicsDelete,*g4Hits,color);
    }
    ND::TND280Log::DecreaseIndentation();
    
}
  
void ShowG4Traj(TControlPanel::CleanupVector& cleanup, 
                ND::TND280Event& event) {

    ND::THandle<ND::TG4TrajectoryContainer> trajectories
        = event.Get<ND::TG4TrajectoryContainer>("truth/G4Trajectories");
    if (!trajectories) return;

    canvas::main->cd(1);
    for (ND::TG4TrajectoryContainer::iterator traj = trajectories->begin();
         traj != trajectories->end();
         ++traj) {
        ND::TG4Trajectory& particle = traj->second;
        DrawParticle(true,cleanup,particle);
    }
    gPad->Update();

    canvas::main->cd(2);
    for (ND::TG4TrajectoryContainer::iterator traj = trajectories->begin();
         traj != trajectories->end();
         ++traj) {
        ND::TG4Trajectory& particle = traj->second;
        DrawParticle(false,cleanup,particle);
    }
    gPad->Update();

    canvas::main->Update();

}

void ShowReconPID(TControlPanel::CleanupVector& graphicsDelete,
                  ND::THandle<ND::TReconPID>& recon,
                  int color, int baseColor ) {
    if (!recon) return;
    ND::THandle<ND::TPIDState> state = recon->GetState();
    if (!state) {
        ND280Error("TPIDState missing!");
        return;
    }
    TVector3 head = state->GetPosition().Vect();
    TVector3 var = state->GetPositionVariance().Vect();
    TVector3 dir = state->GetDirection().Unit();
    TVector3 dvar = state->GetDirectionVariance();
    ND::THandle<ND::THitSelection> hits = recon->GetHits();

    ND280Log(std::setprecision(3)
             << "PID(" << recon->ConvertParticleId() << ") "
             << std::setprecision(3) 
             << head.X()/unit::cm 
             << std::setprecision(2) 
             << " " << ShowVariance(var.X(),unit::cm)
             << std::setprecision(3) 
             << " cm   " << head.Y()/unit::cm 
             << std::setprecision(2) 
             << " " << ShowVariance(var.Y(),unit::cm)
             << std::setprecision(3) 
             << " cm   " << head.Z()/unit::cm 
             << std::setprecision(2) 
             << " " << ShowVariance(var.Z(),unit::cm)
             << " cm   "
             << std::setprecision(6) 
             << state->GetPosition().T()/unit::ns << " ns");

    ND::TND280Log::IncreaseIndentation();
    ND280Log(std::setprecision(3)
             << "Direction: (" << dir.X() 
             << " " << ShowVariance(dvar.X(),1)
             << ", " << dir.Y()
             << " " << ShowVariance(dvar.Y(),1)
             << ", " << dir.Z()
             << " " << ShowVariance(dvar.Z(),1)
             << ")");

    ND280Log("Momentum " << state->GetMomentum() 
             << " " << ShowVariance(state->GetMomentumVariance(),1)
             << "   Particle Charge " << state->GetCharge()
             << " " << ShowVariance(state->GetChargeVariance(),1));

    if (hits) {
        LoopOverTHits(graphicsDelete,*hits,color);
    }

    TPolyLine *xzLine = NULL;
    TPolyLine *yzLine = NULL;
    TPolyLine *xzBase = NULL;
    TPolyLine *yzBase = NULL;

    ND::TReconNodeContainer& nodes = recon->GetNodes();
    ND280NamedInfo("nodes", "PID Nodes " << nodes.size());
    if (nodes.size()>1) {
        for (ND::TReconNodeContainer::iterator n = nodes.begin();
             n != nodes.end();
             ++n) {
            
            if (!(*n)->GetState()) {
                ND280Error("Node without a state");
                continue;
            }
            
            ND::THandle<ND::TPIDState> state = (*n)->GetState();
            if (!state) {
                ND280Error("Not a PID Node: " << (*n)->GetState()->ClassName());
                continue;
            }
            
            TLorentzVector position = state->GetPosition();
            TLorentzVector variance = state->GetPositionVariance();
            
            ND280NamedInfo("nodes",
                           std::fixed << std::setprecision(1)
                           << "Node: ("
                           << position.X()/unit::cm 
                           << " " << ShowVariance(variance.X(),unit::cm)
                           << " cm, " << position.Y()/unit::cm 
                           << " " << ShowVariance(variance.Y(),unit::cm)
                           << " cm, " << position.Z()/unit::cm 
                           << " " << ShowVariance(variance.Z(),unit::cm)
                           << " cm, "
                           << state->GetPosition().T()/unit::ns << " ns)");

            // plot in XZ
            if (!ND::TCorrValues::IsFree(variance.X()) 
                && !ND::TCorrValues::IsFree(variance.Z())) {
                if (!xzLine) {
                    xzLine = new TPolyLine;
                    graphicsDelete.push_back(xzLine);
                    if (color != baseColor) {
                        xzBase = new TPolyLine;
                        graphicsDelete.push_back(xzBase);
                    }
                }
                xzLine->SetNextPoint(position.Z()/unit::cm,
                                     position.X()/unit::cm); 
                if (xzBase) {
                    xzBase->SetNextPoint(position.Z()/unit::cm,
                                         position.X()/unit::cm); 
                }
            }
            
            if (!ND::TCorrValues::IsFree(variance.Y()) 
                && !ND::TCorrValues::IsFree(variance.Z())) {
                if (!yzLine) {
                    yzLine = new TPolyLine;
                    graphicsDelete.push_back(yzLine);
                    if (color != baseColor) {
                        yzBase = new TPolyLine;
                        graphicsDelete.push_back(yzBase);
                    }
                }
                yzLine->SetNextPoint(position.Z()/unit::cm,
                                     position.Y()/unit::cm);
                if (yzBase) {
                    yzBase->SetNextPoint(position.Z()/unit::cm,
                                         position.Y()/unit::cm); 
                }
            }
        }
        int lineStyle = 1;
        int lineWidth = 2;
        switch (recon->GetParticleId()) {
        case ND::TReconPID::kShower:
        case ND::TReconPID::kEM:
        case ND::TReconPID::kElectron:
        case ND::TReconPID::kGamma:
        case ND::TReconPID::kHadronic:
        case ND::TReconPID::kPiZero:
            lineStyle = 3;
            lineWidth = 4;
            break;
        case ND::TReconPID::kLightTrack:
        case ND::TReconPID::kMuon:
        case ND::TReconPID::kPion:
            lineStyle = 1;
            lineWidth = 2;
            break;
        case ND::TReconPID::kHeavyTrack:
        case ND::TReconPID::kProton:
        case ND::TReconPID::kKaon:
            lineStyle = 1;
            lineWidth = 4;
            break;
        default:
            break;
        }
        
        if (xzBase) {
            canvas::main->cd(1);
            xzBase->SetLineStyle(lineStyle);
            xzBase->SetLineWidth(lineWidth+2);
            xzBase->SetLineColor(baseColor);
            xzBase->Draw();
        }

        if (xzLine) {
            canvas::main->cd(1);
            xzLine->SetLineStyle(lineStyle);
            xzLine->SetLineWidth(lineWidth);
            xzLine->SetLineColor(color);
            xzLine->Draw();
        }
    
        if (yzBase) {
            canvas::main->cd(2);
            yzBase->SetLineStyle(lineStyle);
            yzBase->SetLineWidth(lineWidth+2);
            yzBase->SetLineColor(baseColor);
            yzBase->Draw();
        }

        if (yzLine) {
            canvas::main->cd(2);
            yzLine->SetLineStyle(lineStyle);
            yzLine->SetLineWidth(lineWidth);
            yzLine->SetLineColor(color);
            yzLine->Draw();
        }
    }
    else if (nodes.size() > 0) {
        ND::TND280Log::IncreaseIndentation();
        ND::THandle<ND::TReconNode> node = nodes.front();
        if (node) {
            ND::THandle<ND::TReconBase> obj = node->GetObject();
            if (obj) ShowReconObject(graphicsDelete,obj,color,baseColor);
        }
        ND::TND280Log::DecreaseIndentation();
    }

    ND::TND280Log::DecreaseIndentation();
}

void ShowReconTrack(TControlPanel::CleanupVector& graphicsDelete,
                    ND::THandle<ND::TReconTrack>& recon,
                    int color, int baseColor) {
    if (!recon) return;
    ND::THandle<ND::TTrackState> state = recon->GetState();
    if (!state) {
        ND280Error("TTrackState missing!");
        return;
    }
    TVector3 head = state->GetPosition().Vect();
    TVector3 var = state->GetPositionVariance().Vect();
    TVector3 dir = state->GetDirection().Unit();
    TVector3 dvar = state->GetDirectionVariance();
    TVector3 tail = head + 20*dir*unit::cm;
    ND::THandle<ND::THitSelection> hits = recon->GetHits();

    ND280Log(std::setprecision(3)
             << "Track " << head.X()/unit::cm 
             << " " << ShowVariance(var.X(),unit::cm)
             << std::setprecision(3)
             << " cm  " << head.Y()/unit::cm 
             << " " << ShowVariance(var.Y(),unit::cm)
             << std::setprecision(3)
             << " cm  " << head.Z()/unit::cm 
             << " " << ShowVariance(var.Z(),unit::cm)
             << " cm  "
             << std::setprecision(6)
             << state->GetPosition().T()/unit::ns << " ns");

    ND::TND280Log::IncreaseIndentation();
    ND280Log(std::setprecision(3)
             << "Direction: (" << dir.X() 
             << " " << ShowVariance(dvar.X(),1)
             << ", " << dir.Y()
             << " " << ShowVariance(dvar.Y(),1)
             << ", " << dir.Z()
             << " " << ShowVariance(dvar.Z(),1)
             << ")");

    if (hits) {
        LoopOverTHits(graphicsDelete,*hits,color);
    }

    TPolyLine *xzLine = NULL;
    TPolyLine *yzLine = NULL;
    TPolyLine *xzBase = NULL;
    TPolyLine *yzBase = NULL;

    //////////////////////////////////////////////////
    // Draw the initial direction and position of the track.
    /////////////////////////////////////////////////
    if (!ND::TCorrValues::IsFree(var.X()) 
        && !ND::TCorrValues::IsFree(var.Z())) {
        if (!xzLine) {
            xzLine = new TPolyLine;
            graphicsDelete.push_back(xzLine);
        }
        xzLine->SetNextPoint(head.Z()/unit::cm,
                             head.X()/unit::cm); 
        xzLine->SetNextPoint(tail.Z()/unit::cm,
                             tail.X()/unit::cm); 
    }

    if (!ND::TCorrValues::IsFree(var.Y())
        && !ND::TCorrValues::IsFree(var.Z())) {
        if (!yzLine) {
            yzLine = new TPolyLine;
            graphicsDelete.push_back(yzLine);
        }
        yzLine->SetNextPoint(head.Z()/unit::cm,
                             head.Y()/unit::cm); 
        yzLine->SetNextPoint(tail.Z()/unit::cm,
                             tail.Y()/unit::cm); 
    }

    if (xzLine) {
        canvas::main->cd(1);
        xzLine->SetLineStyle(3);
        xzLine->SetLineWidth(1);
        xzLine->SetLineColor(color);
        xzLine->Draw();
    }
    
    if (yzLine) {
        canvas::main->cd(2);
        yzLine->SetLineStyle(3);
        yzLine->SetLineWidth(1);
        yzLine->SetLineColor(color);
        yzLine->Draw();
    }

    //////////////////////////////////////////////////
    // Draw the nodes in the track.
    //////////////////////////////////////////////////
    xzLine = NULL;
    yzLine = NULL;
    ND::TReconNodeContainer& nodes = recon->GetNodes();
    ND280NamedInfo("nodes", "Track Nodes " << nodes.size());
    for (ND::TReconNodeContainer::iterator n = nodes.begin();
         n != nodes.end();
         ++n) {
        bool good = false;
        TLorentzVector position;
        TLorentzVector variance;
        TVector3 direction;

        if (ND::THandle<ND::TTrackState> state = (*n)->GetState()) {
            good = true;
            position = state->GetPosition();
            variance = state->GetPositionVariance();
            direction = state->GetDirection();
        }
        else {
            ND280Error("Not a track state");
        }

        if (!good) continue;

        const double nodeLength = 3*unit::cm;
        // plot in XZ
        if (!ND::TCorrValues::IsFree(variance.X()) 
            && !ND::TCorrValues::IsFree(variance.Z())) {
            if (!xzLine) {
                xzLine = new TPolyLine;
                graphicsDelete.push_back(xzLine);
                if (color != baseColor) {
                    xzBase = new TPolyLine;
                    graphicsDelete.push_back(xzBase);
                }
            }
            xzLine->SetNextPoint(position.Z()/unit::cm,
                                 position.X()/unit::cm); 
            if (xzBase) {
                xzBase->SetNextPoint(position.Z()/unit::cm,
                                     position.X()/unit::cm); 
            }
            TLine* nodeLine = 
                new TLine((position.Z()-nodeLength*direction.Z())/unit::cm,
                          (position.X()-nodeLength*direction.X())/unit::cm,
                          (position.Z()+nodeLength*direction.Z())/unit::cm,
                          (position.X()+nodeLength*direction.X())/unit::cm);
            graphicsDelete.push_back(nodeLine);
            canvas::main->cd(1);
            nodeLine->SetLineStyle(1);
            nodeLine->SetLineWidth(1);
            nodeLine->SetLineColor(color);
            nodeLine->Draw();
        }

        if (!ND::TCorrValues::IsFree(variance.Y())
            && !ND::TCorrValues::IsFree(variance.Z())) {
            if (!yzLine) {
                yzLine = new TPolyLine;
                graphicsDelete.push_back(yzLine);
                if (color != baseColor) {
                    yzBase = new TPolyLine;
                    graphicsDelete.push_back(yzBase);
                }
            }
            yzLine->SetNextPoint(position.Z()/unit::cm,
                                 position.Y()/unit::cm);
            if (yzBase) {
                yzBase->SetNextPoint(position.Z()/unit::cm,
                                     position.Y()/unit::cm); 
            }
            TLine* nodeLine = 
                new TLine((position.Z()-nodeLength*direction.Z())/unit::cm,
                          (position.Y()-nodeLength*direction.Y())/unit::cm,
                          (position.Z()+nodeLength*direction.Z())/unit::cm,
                          (position.Y()+nodeLength*direction.Y())/unit::cm);
            graphicsDelete.push_back(nodeLine);
            canvas::main->cd(2);
            nodeLine->SetLineStyle(1);
            nodeLine->SetLineWidth(1);
            nodeLine->SetLineColor(color);
            nodeLine->Draw();
        }
    }

    if (xzBase) {
        canvas::main->cd(1);
        xzBase->SetLineStyle(3);
        xzBase->SetLineWidth(4);
        xzBase->SetLineColor(baseColor);
        xzBase->Draw();
    }
    
    if (xzLine) {
        canvas::main->cd(1);
        xzLine->SetLineStyle(3);
        xzLine->SetLineWidth(2);
        xzLine->SetLineColor(color);
        xzLine->Draw();
    }
    
    if (yzBase) {
        canvas::main->cd(2);
        yzBase->SetLineStyle(3);
        yzBase->SetLineWidth(4);
        yzBase->SetLineColor(baseColor);
        yzBase->Draw();
    }

    if (yzLine) {
        canvas::main->cd(2);
        yzLine->SetLineStyle(3);
        yzLine->SetLineWidth(2);
        yzLine->SetLineColor(color);
        yzLine->Draw();
    }
    ND::TND280Log::DecreaseIndentation();

}

void ShowReconShower(TControlPanel::CleanupVector& graphicsDelete,
                     ND::THandle<ND::TReconShower>& recon,
                     int color, int baseColor) {
    if (!recon) return;
    ND::THandle<ND::TShowerState> state = recon->GetState();
    if (!state) {
        ND280Error("TShowerState missing!");
        return;
    }
    TVector3 var = state->GetPositionVariance().Vect();
    TVector3 coneVector = state->GetCone();
    double cone = std::max(coneVector.X(), coneVector.Y());
    TVector3 head = state->GetPosition().Vect();
    TVector3 dir = state->GetDirection().Unit();
    TVector3 dvar = state->GetDirectionVariance();
    ND::THandle<ND::THitSelection> hits = recon->GetHits();

    ND280Log(std::fixed << std::setprecision(3)
             << "Shower " << head.X()/unit::cm 
             << " " << ShowVariance(var.X(),unit::cm)
             << " cm  " << head.Y()/unit::cm 
             << " " << ShowVariance(var.Y(),unit::cm)
             << " cm  " << head.Z()/unit::cm 
             << " " << ShowVariance(var.Z(),unit::cm)
             << " cm  " 
             << state->GetPosition().T()/unit::ns << " ns");

    ND::TND280Log::IncreaseIndentation();

    ND280Log(std::fixed << std::setprecision(3)
             << "Direction: (" << dir.X() 
             << " " << ShowVariance(dvar.X(),1)
             << ", " << dir.Y()
             << " " << ShowVariance(dvar.Y(),1)
             << ", " << dir.Z()
             << " " << ShowVariance(dvar.Z(),1)
             << ")");

    ND280Log("Cone " << cone
             << "   EDeposit " << state->GetEDeposit() 
             << " " << ShowVariance(state->GetEDepositVariance(),1)
             << " pe");

    if (hits) {
        LoopOverTHits(graphicsDelete,*hits,color);
    }

    if (!ND::TCorrValues::IsFree(var.X())
        && !ND::TCorrValues::IsFree(var.Z())) {
        // plot in XZ
        TVector3 dir1 = dir;
        dir1.RotateY(cone);
        TVector3 tail1 = head + 1*unit::m*dir1;
        TVector3 dir2 = dir;
        dir2.RotateY(-cone);
        TVector3 tail2 = head + 1*unit::m*dir2;
        TPolyLine *line = new TPolyLine;
        graphicsDelete.push_back(line);
        line->SetNextPoint(tail1.Z()/unit::cm,tail1.X()/unit::cm);
        line->SetNextPoint(head.Z()/unit::cm,head.X()/unit::cm);
        line->SetNextPoint(tail2.Z()/unit::cm,tail2.X()/unit::cm);
        TPolyLine *cline = new TPolyLine;
        graphicsDelete.push_back(cline);
        cline->SetNextPoint(head.Z()/unit::cm,head.X()/unit::cm);
        TVector3 tail = head + 0.5*unit::m*dir;
        cline->SetNextPoint(tail.Z()/unit::cm,tail.X()/unit::cm);
        canvas::main->cd(1);
        line->SetLineStyle(2);
        line->SetLineWidth(2);
        line->SetLineColor(color);
        line->Draw();
        cline->SetLineStyle(2);
        cline->SetLineWidth(1);
        cline->SetLineColor(baseColor);
        cline->Draw();
    }

    if (!ND::TCorrValues::IsFree(var.Y())
        && !ND::TCorrValues::IsFree(var.Z())) {
        // plot in YZ
        TVector3 dir1 = dir;
        dir1.RotateX(cone);
        TVector3 tail1 = head + 1*unit::m*dir1;
        TVector3 dir2 = dir;
        dir2.RotateX(-cone);
        TVector3 tail2 = head + 1*unit::m*dir2;
        TPolyLine *line = new TPolyLine;
        graphicsDelete.push_back(line);
        line->SetNextPoint(tail1.Z()/unit::cm,tail1.Y()/unit::cm);
        line->SetNextPoint(head.Z()/unit::cm,head.Y()/unit::cm);
        line->SetNextPoint(tail2.Z()/unit::cm,tail2.Y()/unit::cm);
        TPolyLine *cline = new TPolyLine;
        graphicsDelete.push_back(cline);
        cline->SetNextPoint(head.Z()/unit::cm,head.Y()/unit::cm);
        TVector3 tail = head + 0.5*unit::m*dir;
        cline->SetNextPoint(tail.Z()/unit::cm,tail.Y()/unit::cm);
        canvas::main->cd(2);
        line->SetLineStyle(2);
        line->SetLineWidth(2);
        line->SetLineColor(color);
        line->Draw();
        cline->SetLineStyle(2);
        cline->SetLineWidth(1);
        cline->SetLineColor(baseColor);
        cline->Draw();
    }

    //////////////////////////////////////////////////
    // Draw the nodes in the shower.
    //////////////////////////////////////////////////
    TPolyLine* xzLine = NULL;
    TPolyLine* yzLine = NULL;
    TPolyLine* xzBase = NULL;
    TPolyLine* yzBase = NULL;
    ND::TReconNodeContainer& nodes = recon->GetNodes();
    ND280NamedInfo("nodes", "Shower Nodes " << nodes.size());
    // ND::TND280Log::IncreaseIndentation();
    for (ND::TReconNodeContainer::iterator n = nodes.begin();
         n != nodes.end();
         ++n) {
        bool good = false;
        TLorentzVector position;
        TLorentzVector variance;
        TVector3 direction;

        if (ND::THandle<ND::TShowerState> state = (*n)->GetState()) {
            good = true;
            position = state->GetPosition();
            variance = state->GetPositionVariance();
            direction = state->GetDirection();
        }
        else {
            ND280Error("Not a shower state");
        }

        if (!good) continue;

        ND280NamedInfo("nodes",
                       std::fixed << std::setprecision(1)
                       << "Node: ("
                       << position.X()/unit::cm 
                       << " " << ShowVariance(variance.X(),unit::cm)
                       << " cm, " << position.Y()/unit::cm 
                       << " " << ShowVariance(variance.Y(),unit::cm)
                       << " cm, " << position.Z()/unit::cm 
                       << " " << ShowVariance(variance.Z(),unit::cm)
                       << " cm, "
                       << state->GetPosition().T()/unit::ns << " ns)");
        
        // plot in XZ
        if (!ND::TCorrValues::IsFree(variance.X()) 
            && !ND::TCorrValues::IsFree(variance.Z())) {
            if (!xzLine) {
                xzLine = new TPolyLine;
                graphicsDelete.push_back(xzLine);
                if (color != baseColor) {
                    xzBase = new TPolyLine;
                    graphicsDelete.push_back(xzBase);
                }
            }
            xzLine->SetNextPoint(position.Z()/unit::cm,
                                 position.X()/unit::cm); 
            if (xzBase) {
                xzBase->SetNextPoint(position.Z()/unit::cm,
                                     position.X()/unit::cm); 
            }
        }

        if (!ND::TCorrValues::IsFree(variance.Y())
            && !ND::TCorrValues::IsFree(variance.Z())) {
            if (!yzLine) {
                yzLine = new TPolyLine;
                graphicsDelete.push_back(yzLine);
                if (color != baseColor) {
                    yzBase = new TPolyLine;
                    graphicsDelete.push_back(yzBase);
                }
            }
            yzLine->SetNextPoint(position.Z()/unit::cm,
                                 position.Y()/unit::cm);
            if (yzBase) {
                yzBase->SetNextPoint(position.Z()/unit::cm,
                                     position.Y()/unit::cm); 
            }

        }

        ND::TND280Log::IncreaseIndentation();
        ND::THandle<ND::TReconCluster> cluster = (*n)->GetObject();
        if (cluster) {
            ShowReconCluster(graphicsDelete, cluster, color, baseColor);
        }
        else {
            ND280Log("Shower with non cluster node");
        }
        ND::TND280Log::DecreaseIndentation();
    }
    // ND::TND280Log::DecreaseIndentation();

    if (xzBase) {
        canvas::main->cd(1);
        xzBase->SetLineStyle(3);
        xzBase->SetLineWidth(4);
        xzBase->SetLineColor(baseColor);
        xzBase->Draw();
    }
    
    if (xzLine) {
        canvas::main->cd(1);
        xzLine->SetLineStyle(3);
        xzLine->SetLineWidth(2);
        xzLine->SetLineColor(color);
        xzLine->Draw();
    }
    
    if (yzBase) {
        canvas::main->cd(2);
        yzBase->SetLineStyle(3);
        yzBase->SetLineWidth(4);
        yzBase->SetLineColor(baseColor);
        yzBase->Draw();
    }

    if (yzLine) {
        canvas::main->cd(2);
        yzLine->SetLineStyle(3);
        yzLine->SetLineWidth(2);
        yzLine->SetLineColor(color);
        yzLine->Draw();
    }

    ND::TND280Log::DecreaseIndentation();

    gPad->Update();
}

void ShowReconCluster(TControlPanel::CleanupVector& graphicsDelete,
                      ND::THandle<ND::TReconCluster>& recon,
                      int color, int baseColor) {
    if (!recon) return;
    ND::THandle<ND::TClusterState> state = recon->GetState();
    if (!state) {
        ND280Error("TClusterState missing!");
        return;
    }
    TVector3 var = state->GetPositionVariance().Vect();
    TVector3 pos = state->GetPosition().Vect();
    const ND::TReconCluster::MomentMatrix& moments = recon->GetMoments();

    ND280Log(std::setprecision(3)
             << "Cluster " << pos.X()/unit::cm
             << " " << ShowVariance(var.X(),unit::cm)
             << " cm  " << pos.Y()/unit::cm 
             << " " << ShowVariance(var.Y(),unit::cm)
             << " cm  " << pos.Z()/unit::cm 
             << " " << ShowVariance(var.Z(),unit::cm)
             << " cm  "
             << state->GetPosition().T()/unit::ns << " ns");

    ND::TND280Log::IncreaseIndentation();
#ifdef SHOW_CLUSTER_MOMENTS
    ND280Log("Moments " << std::endl
             << "\t\t " << moments(0,0) 
             << "\t\t " << moments(0,1) 
             << "\t\t " << moments(0,2) << std::endl
             << "\t\t " << moments(1,0) 
             << "\t\t " << moments(1,1) 
             << "\t\t " << moments(1,2) << std::endl
             << "\t\t " << moments(2,0) 
             << "\t\t " << moments(2,1) 
             << "\t\t " << moments(2,2));
#endif

    ND280Log("EDeposit " << state->GetEDeposit() 
             << " " << ShowVariance(state->GetEDeposit(),1)
             << " pe");

    ND::THandle<ND::THitSelection> hits = recon->GetHits();
    if (hits) {
        LoopOverTHits(graphicsDelete,*hits,color);
    }
    if (!ND::TCorrValues::IsFree(var.X()) 
        && !ND::TCorrValues::IsFree(var.Z())
        && !ND::TCorrValues::IsFixed(var.X()) 
        && !ND::TCorrValues::IsFixed(var.Z())) {
        double corr = moments(0,2)/std::sqrt(moments(0,0)*moments(2,2));
        double sqr = moments(0,0) + moments(2,2);
        sqr = sqr*sqr - 4*(1-corr*corr)*moments(0,0)*moments(2,2);
        if (sqr < 0.0) sqr = 0.0;
        double e1 = moments(0,0) + moments(2,2) + std::sqrt(sqr);
        double e2 = moments(0,0) + moments(2,2) - std::sqrt(sqr);
        double major = std::sqrt(std::max(e1,e2));
        double minor = std::sqrt(std::min(e1,e2));
        double angle = 0.5*TMath::RadToDeg()
            *std::atan2(2*corr*std::sqrt(moments(0,0)*moments(2,2)),
                        (double) moments(2,2)-moments(0,0));
        if (!(-180<angle && angle<180)) angle = 0;
        major = std::max(1*unit::mm,std::min(major,150*unit::cm));
        minor = std::max(1*unit::mm,std::min(minor,150*unit::cm));
        ND280Log("X-Z " << std::fixed << std::setprecision(2)
                 << "Major: " << major/unit::cm << " cm " 
                 << "   Minor: " << minor/unit::cm << " cm " 
                 << "   Angle: " << angle << " deg");
        TEllipse* ellipse 
            = new TEllipse(pos.Z()/unit::cm, pos.X()/unit::cm, 
                           major/unit::cm, minor/unit::cm,
                           0.0, 360.0, angle);
        graphicsDelete.push_back(ellipse);
        canvas::main->cd(1);
        ellipse->SetLineStyle(1);
        ellipse->SetLineColor(color);
        ellipse->SetFillStyle(0);
        ellipse->Draw();
    }

    if (!ND::TCorrValues::IsFree(var.Y())
        && !ND::TCorrValues::IsFree(var.Z())
        && !ND::TCorrValues::IsFixed(var.Y())
        && !ND::TCorrValues::IsFixed(var.Z())) {
        double corr = moments(1,2)/std::sqrt(moments(1,1)*moments(2,2));
        double sqr = moments(1,1) + moments(2,2);
        sqr = sqr*sqr - 4*(1-corr*corr)*moments(1,1)*moments(2,2);
        if (sqr < 0.0) sqr = 0.0;
        double e1 = moments(1,1) + moments(2,2) + std::sqrt(sqr);
        double e2 = moments(1,1) + moments(2,2) - std::sqrt(sqr);
        double major = std::sqrt(std::max(e1,e2));
        double minor = std::sqrt(std::min(e1,e2));
        double angle = 0.5*TMath::RadToDeg()
            *std::atan2(2*corr*std::sqrt(moments(1,1)*moments(2,2)),
                        (double) moments(2,2)-moments(1,1));
        if (!(-180<angle && angle<180)) angle = 0;
        major = std::max(1*unit::mm,std::min(major,150*unit::cm));
        minor = std::max(1*unit::mm,std::min(minor,150*unit::cm));
        ND280Log("Y-Z " << std::fixed << std::setprecision(2)
                 << "Major: " << major/unit::cm << " cm " 
                 << "   Minor: " << minor/unit::cm << " cm " 
                 << "   Angle: " << angle << " deg");
        TEllipse* ellipse 
            = new TEllipse(pos.Z()/unit::cm, pos.Y()/unit::cm, 
                           major/unit::cm, minor/unit::cm,
                           0.0, 360.0, angle);
        graphicsDelete.push_back(ellipse);
        canvas::main->cd(2);
        ellipse->SetLineStyle(1);
        ellipse->SetLineColor(color);
        ellipse->SetFillStyle(0);
        ellipse->Draw();
    }
    ND::TND280Log::DecreaseIndentation();
}

void ShowReconVertex(TControlPanel::CleanupVector& graphicsDelete,
                     ND::THandle<ND::TReconVertex>& recon,
                     int color, int baseColor) {
    if (!recon) return;
    ND::THandle<ND::TVertexState> state = recon->GetState();
    if (!state) {
        ND280Error("TVertexState missing!");
        return;
    }
    TVector3 var = state->GetPositionVariance().Vect();
    TVector3 pos = state->GetPosition().Vect();

    ND280Log(std::setprecision(3)
             << "Vertex " << pos.X()/unit::cm 
             << " " << ShowVariance(var.X(),unit::cm)
             << " cm  " << pos.Y()/unit::cm 
             << " " << ShowVariance(var.Y(),unit::cm)
             << " cm  " << pos.Z()/unit::cm 
             << " " << ShowVariance(var.Z(),unit::cm)
             << " cm  "
             << state->GetPosition().T()/unit::ns << " ns");
    ND::TND280Log::IncreaseIndentation();
    
    ND::TCorrValues posCorrValue = ND::TMPositionState::ProjectState(state);
#if BEFORE_oaEvent(8,7,0)
    const ND::TCorrValues::Matrix& covar = posCorrValue.GetCovariance();
#else
    const TMatrixTSym<ND::TCorrValues::Element>& covar 
        = posCorrValue.GetCovariance();
#endif

    if (!ND::TCorrValues::IsFree(var.X()) 
        && !ND::TCorrValues::IsFree(var.Z())
        && !ND::TCorrValues::IsFixed(var.X()) 
        && !ND::TCorrValues::IsFixed(var.Z())) {
        double corr = covar(0,2)/std::sqrt(covar(0,0)*covar(2,2));
        double sqr = covar(0,0) + covar(2,2);
        sqr = sqr*sqr - 4*(1-corr*corr)*covar(0,0)*covar(2,2);
        if (sqr < 0.0) sqr = 0.0;
        double e1 = covar(0,0) + covar(2,2) + std::sqrt(sqr);
        double e2 = covar(0,0) + covar(2,2) - std::sqrt(sqr);
        double major = std::sqrt(std::max(e1,e2));
        double minor = std::sqrt(std::min(e1,e2));
        double angle = 0.5*TMath::RadToDeg()
            *std::atan2(2*corr*std::sqrt(covar(0,0)*covar(2,2)),
                        (double) covar(2,2)-covar(0,0));
        major = std::max(1*unit::mm,std::min(major,150*unit::cm));
        minor = std::max(1*unit::mm,std::min(minor,150*unit::cm));
        ND280Log("X-Z "
                 << "Major: " << major/unit::cm << " cm " 
                 << "   Minor: " << minor/unit::cm << " cm " 
                 << "   Angle: " << angle << " deg");
        TEllipse* ellipse 
            = new TEllipse(pos.Z()/unit::cm, pos.X()/unit::cm, 
                           major/unit::cm, minor/unit::cm,
                           0.0, 360.0, angle);
        graphicsDelete.push_back(ellipse);
        canvas::main->cd(1);
        ellipse->SetLineStyle(1);
        ellipse->SetLineColor(color);
        ellipse->SetFillStyle(1);
        ellipse->Draw();
    }

    if (!ND::TCorrValues::IsFree(var.Y())
        && !ND::TCorrValues::IsFree(var.Z())
        && !ND::TCorrValues::IsFixed(var.Y())
        && !ND::TCorrValues::IsFixed(var.Z())) {
        double corr = covar(1,2)/std::sqrt(covar(1,1)*covar(2,2));
        double sqr = covar(1,1) + covar(2,2);
        sqr = sqr*sqr - 4*(1-corr*corr)*covar(1,1)*covar(2,2);
        if (sqr < 0.0) sqr = 0.0;
        double e1 = covar(1,1) + covar(2,2) + std::sqrt(sqr);
        double e2 = covar(1,1) + covar(2,2) - std::sqrt(sqr);
        double major = std::sqrt(std::max(e1,e2));
        double minor = std::sqrt(std::min(e1,e2));
        double angle = 0.5*TMath::RadToDeg()
            *std::atan2(2*corr*std::sqrt(covar(1,1)*covar(2,2)),
                        (double) covar(2,2)-covar(1,1));
        major = std::max(1*unit::mm,std::min(major,150*unit::cm));
        minor = std::max(1*unit::mm,std::min(minor,150*unit::cm));
        ND280Log("Y-Z "
                 << "Major: " << major/unit::cm << " cm " 
                 << "   Minor: " << minor/unit::cm << " cm " 
                 << "   Angle: " << angle << " deg");
        TEllipse* ellipse 
            = new TEllipse(pos.Z()/unit::cm, pos.Y()/unit::cm, 
                           major/unit::cm, minor/unit::cm,
                           0.0, 360.0, angle);
        graphicsDelete.push_back(ellipse);
        canvas::main->cd(2);
        ellipse->SetLineStyle(1);
        ellipse->SetLineColor(color);
        ellipse->SetFillStyle(1);
        ellipse->Draw();
    }

    if (!ND::TCorrValues::IsFree(var.X()) 
        && !ND::TCorrValues::IsFree(var.Z())) {
        TMarker* marker = new TMarker(pos.Z()/unit::cm, pos.X()/unit::cm, 28);
        graphicsDelete.push_back(marker);
        canvas::main->cd(1);
        marker->SetMarkerColor(color);
        marker->Draw();
    }

    if (!ND::TCorrValues::IsFree(var.Y()) 
        && !ND::TCorrValues::IsFree(var.Z())) {
        TMarker* marker = new TMarker(pos.Z()/unit::cm, pos.Y()/unit::cm, 28);
        graphicsDelete.push_back(marker);
        canvas::main->cd(2);
        marker->SetMarkerColor(color);
        marker->Draw();
    }

    ND::THandle<ND::TReconObjectContainer> constituents
        = recon->GetConstituents();
    if (constituents) {
        ND280Log("Constituent objects: " << constituents->size());
        ND::TND280Log::IncreaseIndentation();
        ShowReconObjects(graphicsDelete, constituents, color, false);
        ND::TND280Log::DecreaseIndentation();
    }

    ND::TND280Log::DecreaseIndentation();
}

void ShowReconObject(TControlPanel::CleanupVector& graphicsDelete,
                     ND::THandle<ND::TReconBase> recon,
                     int color, int baseColor) {
    if (recon) {
        ND::THandle<ND::THitSelection> hits = recon->GetHits();
        if (hits) {
            double time = AverageHitTime(*hits);
            if (time>0) gSelectedTimes.push_back(time);
        }
    }

    ND::THandle<ND::TReconTrack> track = recon;
    if (track) {
        ShowReconTrack(graphicsDelete, track, color, baseColor);
        return;
    }
    ND::THandle<ND::TReconShower> shower = recon;
    if (shower) {
        ShowReconShower(graphicsDelete, shower, color, baseColor);
        return;
    }
    ND::THandle<ND::TReconCluster> cluster = recon;
    if (cluster) {
        ShowReconCluster(graphicsDelete, cluster, color, baseColor);
        return;
    }
    ND::THandle<ND::TReconVertex> vertex = recon;
    if (vertex) {
        ShowReconVertex(graphicsDelete, vertex, color, baseColor);
        return;
    }
    ND::THandle<ND::TReconPID> pid = recon;
    if (pid) {
        ShowReconPID(graphicsDelete, pid, color, baseColor);
        return;
    }
}

int ShowReconObjects(TControlPanel::CleanupVector& graphicsDelete,
                     ND::THandle<ND::TReconObjectContainer>& recon,
                     int color, bool increm) {
    int baseColor = color;
    if (!recon) return color;
    
    // Find all of the "checked" recon object containers and draw them.
    for (ND::TReconObjectContainer::iterator obj = recon->begin();
         obj != recon->end();
         ++obj) {
        ShowReconObject(graphicsDelete,*obj,color,baseColor);
        ND::THandle<ND::TReconTrack> track = *obj;
        color = increment_color(color);
        if (increm) baseColor = color;
    }

    return increment_color(baseColor);
}

void ShowResults(TControlPanel::CleanupVector& graphicsDelete,
                 ND::TND280Event& event,
                 const std::vector<std::string>& results) {
    gSelectedTimes.clear();
    int color = increment_color(0);
    ND::TND280Log::ResetIndentation();
    for (std::vector<std::string>::const_iterator name = results.begin();
         name != results.end();
         ++name) {
        ND::THandle<ND::TDatum> datum = event.Get<ND::TDatum>(name->c_str());
        ND::THandle<ND::TReconObjectContainer> recon = datum;
        if (recon) {
            std::cout << "Recon objects found " << *name << std::endl;
            color = ShowReconObjects(graphicsDelete, recon, color);
            continue;
        }
        std::cout << "Result not found: " << *name << std::endl;
    }
    ND::TND280Log::ResetIndentation();
    canvas::main->Update();
}

TControlPanel::TControlPanel(const TGWindow *p, UInt_t w, UInt_t h)
    : TGMainFrame(p,w,h) {

    TGHorizontalFrame* horizontalFrame = new TGHorizontalFrame(this);

    TGVerticalFrame* leftSide = new TGVerticalFrame(horizontalFrame);

    TGVerticalFrame* rightSide = new TGVerticalFrame(horizontalFrame);
    
    // Create the buttons on the left side.
    TGFrame* widget;
    TGLayoutHints *layout;

    widget = new TGTextButton(this, "&Draw", kDraw);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);
    
    widget = new TGTextButton(this, "&Next", kNext);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);

    widget = new TGTextButton(this, "&Back", kBack);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);

    widget = new TGTextButton(this, "&Skip10", kSkip10);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);


    widget = new TGTextButton(this, "&Skip100", kSkip100);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);

    widget = new TGTextButton(this, "&Skip1000", kSkip1000);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);

    widget = new TGTextButton(this, "&Skip-100", kSkipn100);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);
    
    widget = new TGTextButton(this, "&Save", kSave);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);

    widget = new TGTextButton(this, "&Reject", kReject);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);

    widget = new TGTextButton(this, "&QUIT", kQuit);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    leftSide->AddFrame(widget, layout);

    leftSide->Layout();
    leftSide->Resize(100,300);
    layout = new TGLayoutHints(kLHintsLeft |  kLHintsExpandY);
    horizontalFrame->AddFrame(leftSide,layout);

    // Create the listbox for the results.
    fResultList = new TGListBox(rightSide);
    fResultList->SetMultipleSelections(true);
    layout = new TGLayoutHints(kLHintsCenterX
                               | kLHintsCenterY
                               | kLHintsExpandX 
                               | kLHintsExpandY);
    rightSide->AddFrame(fResultList,layout);

    rightSide->Layout();
    layout = new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY);
    horizontalFrame->AddFrame(rightSide,layout);

    layout = new TGLayoutHints(kLHintsCenterX 
                               | kLHintsCenterY
                               | kLHintsExpandX
                               | kLHintsExpandY);
    AddFrame(horizontalFrame,layout);
    
    SetCleanup(kDeepCleanup);
    SetWindowName("Button Control");
    SetIconName("Button Control ");
    MapSubwindows();
    Layout();
    MapWindow();

    fInputFile = NULL;
    fOutputFile = NULL;
    fRejectFile = NULL;
    fCurrentEvent = NULL;
}

TControlPanel::~TControlPanel(void) {
    Cleanup();
}

void TControlPanel::OpenInput(const std::string& file) {
    fInputFile = new ND::TND280Input(file.c_str(),"OLD");
    if (!fInputFile->IsOpen()) exit(1);
    fCurrentEvent = fInputFile->FirstEvent();
}

void TControlPanel::OpenOutput(const std::string& file) {
    std::cout << "Opening Output File " << file << std::endl;
    fOutputFile = new ND::TND280Output(file.c_str(), "NEW");
    if (!fOutputFile->IsOpen()) exit(1);
}

void TControlPanel::OpenReject(const std::string& file) {
    std::cout << "Opening Rejected Event File " << file << std::endl;
    fRejectFile = new ND::TND280Output(file.c_str(), "NEW");
    if (!fRejectFile->IsOpen()) exit(1);
}

Bool_t TControlPanel::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {
    // Process events generated by the buttons in the frame.
    std::cout << "Message " << msg << "  " << parm1 << std::endl;
    switch (GET_MSG(msg)) {
    case kC_COMMAND:
        switch (GET_SUBMSG(msg)) {
        case kCM_BUTTON:
            printf("text button id %ld pressed\n", parm1);
            if( parm1 == kDraw ) DrawEventHits();
            if( parm1 == kNext ) {NextEvent(); DrawNextEventHits();}
            if( parm1 == kBack ) {PreviousEvent(); DrawNextEventHits();}
            if( parm1 == kSkip10 ) {Skip10Event(); DrawEventHits();}
            if( parm1 == kSkip100 ) {Skip100Event(); DrawEventHits();}
            if( parm1 == kSkip1000 ) {Skip1000Event(); DrawEventHits();}
            if( parm1 == kSkipn100 ) {Skipn100Event(); DrawEventHits();}
            if( parm1 == kSave ) {SaveEvent(); DrawNextEventHits();}
            if( parm1 == kReject ) {SaveReject(); DrawNextEventHits();}
            if( parm1 == kQuit ) QuitProgram();
	  
	default:
            break;
	}
    } 

    gROOT->Reset();
  
    return kTRUE;
}

void TControlPanel::CleanCanvas() {
    canvas::main->cd(1);
    gPad->Clear();
    canvas::main->cd(2);
    gPad->Clear();
    for (CleanupVector::iterator o = fCleanup.begin();
         o!=fCleanup.end();
         ++o) {
        delete *o;
    }
    fCleanup.clear();

    canvas::main->cd(1);
    if (canvas::topView) canvas::topView->Draw();
    if (canvas::topMagnet) canvas::topMagnet->Draw();
    if (canvas::topP0D) canvas::topP0D->Draw();
    if (canvas::topTPC1) canvas::topTPC1->Draw();
    if (canvas::topTPC2) canvas::topTPC2->Draw();
    if (canvas::topTPC3) canvas::topTPC3->Draw();
    if (canvas::topFGD1) canvas::topFGD1->Draw();
    if (canvas::topFGD2) canvas::topFGD2->Draw();
    if (canvas::topDsECal) canvas::topDsECal->Draw();
    if (canvas::topLTP0DECal) canvas::topLTP0DECal->Draw();
    if (canvas::topLSP0DECal) canvas::topLSP0DECal->Draw();
    if (canvas::topLBP0DECal) canvas::topLBP0DECal->Draw();
    if (canvas::topRTP0DECal) canvas::topRTP0DECal->Draw();
    if (canvas::topRSP0DECal) canvas::topRSP0DECal->Draw();
    if (canvas::topRBP0DECal) canvas::topRBP0DECal->Draw();
    if (canvas::topLTBrlECal) canvas::topLTBrlECal->Draw();
    if (canvas::topLSBrlECal) canvas::topLSBrlECal->Draw();
    if (canvas::topLBBrlECal) canvas::topLBBrlECal->Draw();
    if (canvas::topRTBrlECal) canvas::topRTBrlECal->Draw();
    if (canvas::topRSBrlECal) canvas::topRSBrlECal->Draw();
    if (canvas::topRBBrlECal) canvas::topRBBrlECal->Draw();

    canvas::main->cd(2);
    if (canvas::sideView) canvas::sideView->Draw();
    if (canvas::sideMagnet) canvas::sideMagnet->Draw();
    if (canvas::sideP0D) canvas::sideP0D->Draw();
    if (canvas::sideTPC1) canvas::sideTPC1->Draw();
    if (canvas::sideTPC2) canvas::sideTPC2->Draw();
    if (canvas::sideTPC3) canvas::sideTPC3->Draw();
    if (canvas::sideFGD1) canvas::sideFGD1->Draw();
    if (canvas::sideFGD2) canvas::sideFGD2->Draw();
    if (canvas::sideDsECal) canvas::sideDsECal->Draw();
    if (canvas::sideLTP0DECal) canvas::sideLTP0DECal->Draw();
    if (canvas::sideLSP0DECal) canvas::sideLSP0DECal->Draw();
    if (canvas::sideLBP0DECal) canvas::sideLBP0DECal->Draw();
    if (canvas::sideRTP0DECal) canvas::sideRTP0DECal->Draw();
    if (canvas::sideRSP0DECal) canvas::sideRSP0DECal->Draw();
    if (canvas::sideRBP0DECal) canvas::sideRBP0DECal->Draw();
    if (canvas::sideLTBrlECal) canvas::sideLTBrlECal->Draw();
    if (canvas::sideLSBrlECal) canvas::sideLSBrlECal->Draw();
    if (canvas::sideLBBrlECal) canvas::sideLBBrlECal->Draw();
    if (canvas::sideRTBrlECal) canvas::sideRTBrlECal->Draw();
    if (canvas::sideRSBrlECal) canvas::sideRSBrlECal->Draw();
    if (canvas::sideRBBrlECal) canvas::sideRBBrlECal->Draw();

}

void TControlPanel::DrawNextEventHits() {
    DrawEventHits();
}

void TControlPanel::DrawEventHits(void) {
    CleanCanvas();
    if (!fCurrentEvent) return;

    DisplayEventInfo(*fCurrentEvent);

    DrawAllHits(fCleanup,*fCurrentEvent);
    ShowResults(fCleanup,*fCurrentEvent,GetResultEntries());

}

void DisplayEventInfo(ND::TND280Event& event) {
    ND::TND280Log::ResetIndentation();
    ND280Log(event.GetContext());
    ND::TND280Log::IncreaseIndentation();
    
    ND::THandle<ND::TG4PrimaryVertexContainer> primaries 
        = event.Get<ND::TG4PrimaryVertexContainer>(
            "truth/G4PrimVertex00");
    if (primaries) {
        int i = 1;
        for (ND::TG4PrimaryVertexContainer::const_iterator v = primaries->begin();
             v != primaries->end();
             ++v) {
            TLorentzVector vertex = v->GetPosition();
            ND280Log("VERTEX(" << i << ") " 
                     << vertex.X()/unit::cm << " cm"
                     << " " << vertex.Y()/unit::cm << " cm"
                     << " " << vertex.Z()/unit::cm << " cm"
                     << " " << vertex.T()/unit::ns << " ns");
            const ND::TG4PrimaryParticleContainer& particles 
                = v->GetPrimaryParticles();
            int j = 1;
            ND::TND280Log::IncreaseIndentation();
            for (ND::TG4PrimaryParticleContainer::const_iterator p 
                     = particles.begin();
                 p != particles.end();
                 ++p) {
                const TLorentzVector& mom = p->GetMomentum();
                ND280Log(j << " " << p->GetTitle() 
                         << " with energy " << mom.E()/unit::MeV << " MeV"
                         << " and momentum " << mom.P()/unit::MeV << " MeV/c");
                ++j;
            }
            ND::TND280Log::DecreaseIndentation();
            ++i;
        }
    }
    ND::TND280Log::DecreaseIndentation();
}

void TControlPanel::SetResultEntries() {

    gSelectedTimes.clear();
    fResultList->RemoveAll();
    int id = 0;
    std::vector<int> gSelectedResults;
    // Forage the results...
    std::vector<ND::TDatum*> stack;
    std::vector<std::string> existingEntries;
    stack.push_back(fCurrentEvent);
    while (!stack.empty()) {
        ND::TDatum* current = stack.back();
        stack.pop_back();
        ND::TReconObjectContainer* rc 
            = dynamic_cast<ND::TReconObjectContainer*>(current);
        if (rc) {
            std::string fullName(rc->GetFullName());
            if (std::find(existingEntries.begin(),
                          existingEntries.end(),
                          fullName) != existingEntries.end()) continue;
            existingEntries.push_back(fullName);
            fResultList->AddEntry(rc->GetFullName(),++id);
            for (std::vector<std::string>::iterator s 
                     = gDefaultAlgorithmNames.begin();
                 s != gDefaultAlgorithmNames.end();
                 ++s) {
                if (fullName.find(*s)!=std::string::npos) {
                    fResultList->Select(id);
                    break;
                }
            }
            continue;
        }
        ND::TDataVector* dv = dynamic_cast<ND::TDataVector*>(current);
        if (dv) {
            for (ND::TDataVector::iterator d = dv->begin();
                 d != dv->end();
                 ++d) {
                stack.push_back(*d);
            }
        }
    }
    fResultList->Layout();
    fResultList->MapSubwindows();
}

std::vector<std::string> TControlPanel::GetResultEntries() {
    std::vector<std::string> results;
    TList list;
    fResultList->GetSelectedEntries(&list);
    TIter next(&list);
    while (TObject* obj = next()) {
        TGTextLBEntry* entry = dynamic_cast<TGTextLBEntry*>(obj);
        if (entry) results.push_back(entry->GetTitle());
    }
    return results;
}

void TControlPanel::NextEvent(void) {
    CleanCanvas();
    if (!fInputFile) {
        ND280Log("No input file");
        return;
    }
    // if (fInputFile->EndOfFile()) {
    // ND280Log("Past end of input file");
    // return;
    // }
    ND::TND280Event* nextEvent = fInputFile->NextEvent();
    if (!nextEvent) nextEvent = fInputFile->PreviousEvent();
    if (nextEvent) {
        delete fCurrentEvent;
        fCurrentEvent = nextEvent;
    }

    SetResultEntries();
}

void TControlPanel::PreviousEvent(void) {
    CleanCanvas();
    if (!fInputFile) {
        ND280Log("No input file");
        return;
    }
    // if (fInputFile->EndOfFile()) {
    //     ND280Log("Past end of input file");
    //     return;
    // }
    ND::TND280Event* nextEvent = fInputFile->PreviousEvent();
    if (!nextEvent) nextEvent = fInputFile->NextEvent();
    if (nextEvent) {
        delete fCurrentEvent;
        fCurrentEvent = nextEvent;
    }
    SetResultEntries();
}

void TControlPanel::Skip10Event(void) {
    CleanCanvas();
    if (!fInputFile) return;
    std::auto_ptr<ND::TND280Event> nextEvent(NULL);
    nextEvent.reset(fInputFile->NextEvent(10));
    if (!nextEvent.get()) nextEvent.reset(fInputFile->PreviousEvent());
    if (nextEvent.get()) {
        delete fCurrentEvent;
        fCurrentEvent = nextEvent.release();
    }
    SetResultEntries();
}

void TControlPanel::Skip100Event(void) {
    CleanCanvas();
    if (!fInputFile) return;
    std::auto_ptr<ND::TND280Event> nextEvent(NULL);
    nextEvent.reset(fInputFile->NextEvent(100));
    if (!nextEvent.get()) nextEvent.reset(fInputFile->PreviousEvent());
    if (nextEvent.get()) {
        delete fCurrentEvent;
        fCurrentEvent = nextEvent.release();
    }
    SetResultEntries();
}

void TControlPanel::Skip1000Event(void) {
    CleanCanvas();
    if (!fInputFile) return;
    std::auto_ptr<ND::TND280Event> nextEvent(NULL);
    nextEvent.reset(fInputFile->NextEvent(1000));
    if (!nextEvent.get()) nextEvent.reset(fInputFile->PreviousEvent());
    if (nextEvent.get()) {
        delete fCurrentEvent;
        fCurrentEvent = nextEvent.release();
    }
    SetResultEntries();
}

void TControlPanel::Skipn100Event(void) {
    CleanCanvas();
    if (!fInputFile) return;
    std::auto_ptr<ND::TND280Event> nextEvent(NULL);
    nextEvent.reset(fInputFile->PreviousEvent(100));
    if (!nextEvent.get()) nextEvent.reset(fInputFile->NextEvent());
    if (nextEvent.get()) {
        delete fCurrentEvent;
        fCurrentEvent = nextEvent.release();
    }
    SetResultEntries();
}

void TControlPanel::SaveEvent(void) {
    if (fCurrentEvent) {
        if (!fOutputFile) {
            std::cout << " OUTPUT FILE IS NOT OPEN"
                      << std::endl;
            return;
        }
        if (!fOutputFile->IsOpen()) {
            std::cout << " OUTPUT FILE IS NOT OPEN"
                      << std::endl;
            return;
        }
        if (!fOutputFile->GeometryWritten()) {
            fOutputFile->WriteGeometry(ND::TOADatabase::Get().Geometry());
        }
        fOutputFile->WriteEvent(*fCurrentEvent);
    }
    NextEvent();
}

void TControlPanel::SaveReject(void) {
    if (fCurrentEvent) {
        if (!fRejectFile) {
            std::cout << " REJECT FILE IS NOT CREATED"
                      << std::endl;
            return;
        }
        if (!fRejectFile->IsOpen()) {
            std::cout << " REJECT FILE IS NOT OPEN"
                      << std::endl;
            return;
        }
        if (!fRejectFile->GeometryWritten()) {
            fRejectFile->WriteGeometry(ND::TOADatabase::Get().Geometry());
        }
        fRejectFile->WriteEvent(*fCurrentEvent);
    }
    NextEvent();
}
            
void TControlPanel::QuitProgram(void) {
    if (fOutputFile) fOutputFile->Close();
    if (fRejectFile) fRejectFile->Close();
    if (fInputFile) fInputFile->Close();
    exit(1);
}

int increment_color(int index) {
    switch(index) {
    case 2: index = 3; break;
    case 3: index = 4; break;
    case 4: index = 5; break;
    case 5: index = 6; break;
    case 6: index = 7; break;
    case 7: index = 8; break;
    case 8: index = 9; break;
    case 9: index = 25; break;
    case 25: index = 28; break;
    case 28: index = 29; break;
    case 29: index = 30; break;
    case 30: index = 32; break;
    case 32: index = 34; break;
    case 34: index = 36; break;
    case 36: index = 38; break;
    case 38: index = 39; break;
    case 39: index = 40; break;
    case 40: index = 42; break;
    case 42: index = 46; break;
    case 46: index = 49; break;
    case 49: index = 50; break;
    default:
        index = 2;
        break;
    }
    return index;
}

TBox* BoundingBoxYZ(const std::pair<TVector3,TVector3>& bnd) {
    TBox* box = NULL;
    if (bnd.first.Y()+10*unit::cm < bnd.second.Y()
        && bnd.first.Z()+10*unit::cm < bnd.second.Z()) {
        box = new TBox(bnd.first.Z()/unit::cm,
                       bnd.first.Y()/unit::cm,
                       bnd.second.Z()/unit::cm,
                       bnd.second.Y()/unit::cm);
        box->SetLineStyle(3);
        box->SetLineColor(15);
        box->SetLineWidth(1);
        box->SetFillStyle(0);
    }
    return box;
}

TBox* BoundingBoxXZ(const std::pair<TVector3,TVector3>& bnd) {
    TBox* box = NULL;
    if (bnd.first.Y()+10*unit::cm < bnd.second.Y()
        && bnd.first.Z()+10*unit::cm < bnd.second.Z()) {
        box = new TBox(bnd.first.Z()/unit::cm,
                       bnd.first.X()/unit::cm,
                       bnd.second.Z()/unit::cm,
                       bnd.second.X()/unit::cm);
        box->SetLineStyle(3);
        box->SetLineColor(15);
        box->SetLineWidth(1);
        box->SetFillStyle(0);
    }
    return box;
}

