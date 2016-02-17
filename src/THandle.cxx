#include <iostream>
#include <map>
#include <set>

#include <TROOT.h>
#include <TClass.h>

#include "THandle.hxx"
#include "THandleHack.hxx"
#include "TCaptLog.hxx"

namespace {
    int gHandleBaseCount = 0;
    int gLastHandleCount = 0;
    std::set<CP::THandleBase*> *gHandleSet = NULL;
}

ClassImp(CP::THandleBase);
CP::THandleBase::THandleBase() : fCount(0), fHandleCount(0) {
    ++gHandleBaseCount;
    if (gHandleSet) gHandleSet->insert(this);
}
CP::THandleBase::~THandleBase() {
    --gHandleBaseCount;
    if (gHandleSet) gHandleSet->erase(this);
}

ClassImp(CP::THandleBaseDeletable);
CP::THandleBaseDeletable::THandleBaseDeletable() 
    : fObject(NULL) { }
CP::THandleBaseDeletable::THandleBaseDeletable(TObject* pointee)
    : fObject(pointee) { }
CP::THandleBaseDeletable::~THandleBaseDeletable() {
    DeleteObject();
}
void CP::THandleBaseDeletable::DeleteObject() {
    if (!fObject) return;
    // Actually delete the object.
    if (IsOwner()) delete fObject;
    fObject = NULL;
}

ClassImp(CP::THandleBaseUndeletable);
CP::THandleBaseUndeletable::THandleBaseUndeletable() : fObject(NULL) { }
CP::THandleBaseUndeletable::THandleBaseUndeletable(TObject* pointee)
    : fObject(pointee) { }
CP::THandleBaseUndeletable::~THandleBaseUndeletable() {
    DeleteObject();
}
void CP::THandleBaseUndeletable::DeleteObject() {
    fObject = NULL;  // Just set the object pointer to NULL;
}

bool CP::CleanHandleRegistry(bool) {
    bool result = (gHandleBaseCount==gLastHandleCount);
    if (!result) {
        CaptLog("CleanHandleRegistry::"
                 << " Handle Count: " << gHandleBaseCount 
                 << " Change: " << gHandleBaseCount - gLastHandleCount);
        gLastHandleCount = gHandleBaseCount;
    }
    return result;
}

void CP::DumpHandleRegistry() {
    if (!gHandleSet) return;
    if (gHandleSet->empty()) return;
    CaptLog("Existing handles: " << gHandleSet->size());
    CP::TCaptLog::IncreaseIndentation();
    for (std::set<CP::THandleBase*>::iterator h = gHandleSet->begin();
         h != gHandleSet->end();
         ++h) {
        CP::THandleBase* handleBase = *h;
        CaptLog(std::hex << "(0x" << handleBase << ")");
        if (handleBase) {
            CP::TCaptLog::IncreaseIndentation();
            TObject *object = handleBase->GetObject();
            CaptLog(std::hex << "-> (0x" << object << ")");
            if (object) {
                CP::TCaptLog::IncreaseIndentation();
                CaptLog("Class: " << object->ClassName());
                CaptLog("Name: " << object->GetName());
                CP::TCaptLog::DecreaseIndentation();
                if (CP::TCaptLog::GetDebugLevel()>CP::TCaptLog::ErrorLevel) {
                    object->ls();
                }
            }
            CP::TCaptLog::DecreaseIndentation();
        }
    }
    CP::TCaptLog::DecreaseIndentation();
} 

void CP::EnableHandleRegistry(bool enable) {
    if (enable && !gHandleSet) {
        CaptLog("Enable the handle registry");
        gHandleSet = new std::set<CP::THandleBase*>;
    }
    else { 
        CaptLog("Disable the handle registry");
        delete gHandleSet;
        gHandleSet = NULL;
    }
}

ClassImp(CP::TVHandle);
CP::TVHandle::TVHandle() {Default(NULL);}
CP::TVHandle::~TVHandle() {}

void CP::TVHandle::Default(CP::THandleBase* handle) {
    fHandle = handle;
    if (fHandle) fHandle->IncrementReferenceCount();
}

void CP::TVHandle::Link(const CP::TVHandle& rhs) {
    // Copy the handle.
    fHandle = rhs.fHandle;
    // Increment the reference count
    if (fHandle) fHandle->IncrementReferenceCount();
}

void CP::TVHandle::Unlink() {
    if (!fHandle) return;
    if (IsWeak()) fHandle->DecrementHandleCount();
    else fHandle->DecrementReferenceCount();
    if (fHandle->GetHandleCount() < 1) DeleteHandle();
    else if (fHandle->GetReferenceCount() < 1) DeleteObject();
}

void CP::TVHandle::DeleteHandle(void) {
    DeleteObject();

    // Delete the handle.
    if (fHandle) delete fHandle;
    
    // And mark the current object as invalid.
    fHandle = NULL;
}

void CP::TVHandle::DeleteObject(void) {
    if (!fHandle) return;
    fHandle->DeleteObject();
}

TObject* CP::TVHandle::GetPointerValue() const {
    if (!fHandle) return NULL;
    return fHandle->GetObject();
}

void CP::TVHandle::Release(void) {
    if (!fHandle) return;
    fHandle->Release();
}

bool CP::TVHandle::operator == (const CP::TVHandle& rhs) const {
    if (fHandle == rhs.fHandle) return true;
    if (!fHandle) return false;
    if (!rhs.fHandle) return false;
    return (fHandle->GetObject() == rhs.fHandle->GetObject());
}

void CP::TVHandle::ls(Option_t *opt) const {
    TROOT::IndentLevel();
    std::cout << ClassName() << "(" << this << "):: ";
    if (strstr(opt,"size")) {
        TClass* cls = Class();
        if (!cls) return;
        std::cout << " (" << cls->Size() << " b)";
    }
    if (fHandle) {
        std::cout << " Refs: " << fHandle->GetReferenceCount();
        if (fHandle->IsOwner()) std::cout << " (owner)";
        else std::cout << " (released)";
    }
    std::cout << std::endl;
    TROOT::IncreaseDirLevel();
    const TObject* ptr = GetPointerValue();
    if (ptr) ptr->ls(opt);
    TROOT::DecreaseDirLevel();
}
