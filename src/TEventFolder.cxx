// $Id: TEventFolder.cxx,v 1.8 2011/05/18 11:35:12 mcgrew Exp $
//
//  Implement an type of folder specifically for holding TEvents.

#include <stdlib.h>
#include <iostream>

#include <TROOT.h>
#include <TBrowser.h>
#include <TFolder.h>
#include <TQObject.h>

#include "TEvent.hxx"
#include "TEventFolder.hxx"
#include "TCaptLog.hxx"

CP::TEventFolder* CP::TEventFolder::fEventFolder = NULL;
CP::TEvent* CP::TEventFolder::fCurrentEvent = NULL;

CP::TEventFolder::TEventFolder() {
    fFolderOfEvents = NULL;
}

CP::TEventFolder::~TEventFolder() {
    fFolderOfEvents = NULL;
    fEventFolder = NULL;
    fCurrentEvent = NULL;
}

TFolder* CP::TEventFolder::GetFolder(void) const {
    if (!fFolderOfEvents) return NULL;
    return fFolderOfEvents;
}

CP::TEventFolder* CP::TEventFolder::GetEventFolder(void) {
    if (!fEventFolder) {
        fEventFolder = new CP::TEventFolder();
    }
    if (!fEventFolder->fFolderOfEvents) {
        fEventFolder->fFolderOfEvents
            = gROOT->GetRootFolder()->AddFolder("Events","ND280 Events");
        gROOT->GetListOfBrowsables()->Add(fEventFolder->fFolderOfEvents,
                                          "Events");
        gROOT->GetListOfCleanups()->Add(fEventFolder->fFolderOfEvents);
    }
    #define CONNECT_SLOT
    #ifdef CONNECT_SLOT
    TQObject::Connect("TEvent","Selected(CP::TDatum*)",
                      "TEventFolder",fEventFolder,"EventSelected(TObject*)");
    #endif

    return fEventFolder;
}

CP::TEvent* CP::TEventFolder::GetEvent(int indx) const {
    if (!fFolderOfEvents) return NULL;
    TList* folder = dynamic_cast<TList*>(fFolderOfEvents->GetListOfFolders());
    if (!folder) return NULL;
    CP::TEvent* current = NULL;
    int count = 0;

    for ( TObjLink* objlink = folder->LastLink();
          objlink != NULL;
          objlink = objlink->Prev()) {
        current = dynamic_cast<CP::TEvent*>(objlink->GetObject());
        if (current) ++count;
        if (count > indx) break;
    }
    if (current) fCurrentEvent = current;
    return current;
}

void CP::TEventFolder::SetCurrentEvent(CP::TEvent* event) {
    if (!event) return;
    if (!fFolderOfEvents) return;
    TList* folder = dynamic_cast<TList*>(fFolderOfEvents->GetListOfFolders());
    if (!folder) return;

    CP::TEvent* oldEvent = fCurrentEvent;
    for (TObjLink* objlink = folder->LastLink();
          objlink != NULL;
          objlink = objlink->Prev()) {
        CP::TEvent* inList = dynamic_cast<CP::TEvent*>(objlink->GetObject());
        if (inList == event) {
            fCurrentEvent = event;
        }
    }
    CaptInfo("Current event changed from " << oldEvent
              << " to " << fCurrentEvent);
}

CP::TEvent* CP::TEventFolder::FindEvent(int, int) const {
    return NULL;
}

CP::TEvent* CP::TEventFolder::GetCurrentEvent(void) {
    return fCurrentEvent;
}

void CP::TEventFolder::RegisterEvent(CP::TEvent* event) {
    fCurrentEvent = event;
    if (fCurrentEvent && fEventFolder && fEventFolder->fFolderOfEvents) {
        fEventFolder->fFolderOfEvents->Add(event);
        event->SetBit(kMustCleanup);
    }
}

void CP::TEventFolder::RemoveEvent(CP::TEvent* event) {
    // Check if the event is the current event.
    if (fCurrentEvent == event) fCurrentEvent = NULL;

    // Check if the folder is active.  If not, then just return.
    if (!fEventFolder) return;
    if (!fEventFolder->GetFolder()) return;
    TList* folder = 
        dynamic_cast<TList*>(fEventFolder->GetFolder()->GetListOfFolders());
    if (!folder) return;

    // The folder is active, so find another event in the folder and make it
    // the current event.
    for (TObjLink* objlink = folder->LastLink(); 
         objlink != NULL;       
         objlink = objlink->Prev()) {
        fCurrentEvent = dynamic_cast<CP::TEvent*>(objlink->GetObject());
        // Any event is OK as long as it's not the event being removed.
        if (fCurrentEvent != event) break;
    }
}

void CP::TEventFolder::EventSelected(TObject* theObject) {
    CaptInfo("EventFolder had " << theObject->ClassName()  
              << " at " << theObject << " selected");
    CP::TEvent* event = dynamic_cast<CP::TEvent*>(theObject);
    SetCurrentEvent(event);
}


void CP::TEventFolder::ls(const Option_t* opt) const {
    if (fFolderOfEvents) fFolderOfEvents->ls(opt);
    else std::cout << "No Event Folder" << std::endl;
}

ClassImp(CP::TEventFolder);
