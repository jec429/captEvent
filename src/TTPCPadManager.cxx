#include <string>

#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoBBox.h>

#include "HEPUnits.hxx"
#include "TOADatabase.hxx"
#include "TTPCPadManager.hxx"
#include "TND280Log.hxx"

ClassImp(CP::TTPCPadManager);

CP::TTPCPadManager::~TTPCPadManager() { }

CP::TTPCPadManager::TTPCPadManager()
    : TNamed("ND280TPCPadManager","TPC Pad Information"),
      fPadRows(48), fPadColumns(36), 
      // this is not the pad size but the pad pitch !!!!
      fPadXPitch(9.8*unit::mm), fPadYPitch(7*unit::mm), 
      fPadGap(0.15*unit::mm) {
    fLocalXMax = 0.5*(fPadXPitch*(fPadColumns-1));
    fLocalYMax = 0.5*(fPadYPitch*(fPadRows-1));
}

int CP::TTPCPadManager::ChannelToPad(int, int key) const {
    return key;
}

TGeoNode* CP::TTPCPadManager::ChannelToModuleNode(int node, int) const {
    TGeoManager* geom = CP::TOADatabase::Get().Geometry();
    geom->PushPath();
    geom->CdNode(node);
    TGeoNode* moduleNode = geom->GetCurrentNode();
    geom->PopPath();
    std::string name(moduleNode->GetName());
    if (name.find("MM") == std::string::npos) return NULL;
    return moduleNode;
}

bool CP::TTPCPadManager::ChannelToGlobalXYZ(int node, int key,
                                            TVector3& global) const {
    TGeoManager* geom = CP::TOADatabase::Get().Geometry();
    TGeoNode* moduleNode = ChannelToModuleNode(node,key);
    if (!moduleNode) return false;
    int pad = ChannelToPad(node,key);
    double localX = PadToLocalX(pad);
    double localY = PadToLocalY(pad);
    TGeoBBox *shape 
        =  dynamic_cast<TGeoBBox*>(moduleNode->GetVolume()->GetShape());
    double local[3] = {localX, localY, -(shape->GetDZ()-0.01*unit::mm)};
    double master[3] = {0,0,0};
    geom->PushPath();
    geom->CdNode(node);
    geom->LocalToMaster(local,master);
    geom->PopPath();
    global.SetXYZ(master[0],master[1],master[2]);
    return true;
}

bool CP::TTPCPadManager::GlobalXYZToChannel(const TVector3& global, 
                                            int& node, int& key) const {
    node = -1;
    key = 0;
    TGeoManager* geom = CP::TOADatabase::Get().Geometry();
    // Find the node with this global position.
    geom->PushPath();
    TGeoNode* moduleNode = geom->FindNode(global.X(), global.Y(), global.Z());
    std::string moduleName(moduleNode->GetName());
    if (moduleName.find("MM_") == std::string::npos) {
        node = -1;
        geom->PopPath();
        return false;
    }
    // Make sure that the node is a micro-mega.
    node = geom->GetCurrentNodeId();
    double master[3] = {global.X(), global.Y(), global.Z()};
    double local[3];
    geom->MasterToLocal(master,local);
    geom->PopPath();
    // Generate the local key number.
    int row = LocalYToRow(local[1]);
    if (row<0 || GetPadRows()<=row) return false;
    int column = LocalXToColumn(local[0]);
    if (column<0 || GetPadColumns()<=column) return false;
    key = RowAndColumnToPad(row,column);
    if (key<0 || GetPadCount()<=key) return false;
    return true;
}

double CP::TTPCPadManager::ColumnToLocalX(int column) const {
    return fLocalXMax - column*fPadXPitch;
}

double CP::TTPCPadManager::RowToLocalY(int row) const {
    return fLocalYMax - row*fPadYPitch;
}

int CP::TTPCPadManager::LocalXToColumn(double X) const {
    return int(0.5+(fLocalXMax-X)/fPadXPitch);
}

int CP::TTPCPadManager::LocalYToRow(double Y) const {
    return int(0.5+(fLocalYMax-Y)/fPadYPitch);
}

bool CP::TTPCPadManager::LocalXYInPad(double x, double y, int pad) const {
    return ((std::abs(x-PadToLocalX(pad)) < 0.5*(fPadXPitch-fPadGap))
            && (std::abs(y-PadToLocalY(pad)) < 0.5*(fPadYPitch-fPadGap)));
}

int CP::TTPCPadManager::RowAndColumnToPad(int row, int column) const {
    return row + column*fPadRows;
}

int CP::TTPCPadManager::PadToRow(int pad) const {return pad % fPadRows;}

int CP::TTPCPadManager::PadToColumn(int pad) const {return pad / fPadRows;}

void CP::TTPCPadManager::SetPadXPitch(double xPitch) {
    fPadXPitch = xPitch;
    fLocalXMax = 0.5*(fPadXPitch*(fPadColumns-1));
}

void CP::TTPCPadManager::SetPadColumns(int columns) {
    fPadColumns = columns;
    fLocalXMax = 0.5*(fPadXPitch*(fPadColumns-1));
}

void CP::TTPCPadManager::SetPadYPitch(double yPitch) {
    fPadYPitch = yPitch;
    fLocalYMax = 0.5*(fPadYPitch*(fPadRows-1));
}

void CP::TTPCPadManager::SetPadRows(int rows) {
    fPadRows = rows;
    fLocalYMax = 0.5*(fPadYPitch*(fPadRows-1));
}    

void CP::TTPCPadManager::SetPadGap(double gap) {
    fPadGap = gap;
}

void CP::TTPCPadManager::SetLocalXMax(double x) {
    fLocalXMax = x;
}

void CP::TTPCPadManager::SetLocalYMax(double y) {
    fLocalYMax = y;
}
