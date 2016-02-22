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
    SetBit(kWeakHandle,false);
    if (fHandle) {
        fHandle->CheckHandle();
        fHandle->IncrementReferenceCount();
        fHandle->IncrementHandleCount();
    }
}

void CP::TVHandle::Link(const CP::TVHandle& rhs) {
    // Copy the handle.
    fHandle = rhs.fHandle;
    if (!fHandle) return;
    fHandle->CheckHandle();
    fHandle->IncrementHandleCount();
    if (IsWeak()) return;
    fHandle->IncrementReferenceCount();
}

void CP::TVHandle::Unlink() {
    if (!fHandle) return;
    fHandle->CheckHandle();
    if (!IsWeak()) fHandle->DecrementReferenceCount();
    fHandle->DecrementHandleCount();
    CheckSurvival();
    fHandle = NULL;
}

void CP::TVHandle::MakeWeak() {
    if (IsWeak()) return;
    SetBit(kWeakHandle,true);
    // Decrement the reference count to the object, but leave the handle count
    // unchanged.
    if (!fHandle) return;
    fHandle->CheckHandle();
    fHandle->DecrementReferenceCount();
    CheckSurvival();
}

void CP::TVHandle::MakeLock() {
    if (!IsWeak()) return;
    SetBit(kWeakHandle,false);
    // Increment the reference count to the object, but leave the handle count
    // unchanged, but only if there is a valid handle, and a valid object.
    if (!fHandle) return;
    if (!fHandle->GetObject()) return;
    fHandle->CheckHandle();
    fHandle->IncrementReferenceCount();
}

void CP::TVHandle::CheckSurvival() {
    // The handle doesn't exist, so just return.
    if (!fHandle) return;
    // Check for old handles.
    fHandle->CheckHandle();
    // The handle counter is zero so nothing (no strong, or weak handles) is
    // using this THandleBase and it should be deleted.  This also deletes the
    // object.
    if (fHandle->GetHandleCount() < 1) {
        fHandle->DeleteObject();
        delete fHandle;
        fHandle = NULL;
        return;
    }
    // The reference counter is zero, so no strong handles are referencing the
    // object.  Delete the object, but leave the THandleBase.
    if (fHandle->GetReferenceCount() < 1) {
        fHandle->DeleteObject();
    }
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
        std::cout << " (" << fHandle->GetHandleCount() << ")";
        if (IsWeak()) std::cout << " (weak)";
        else if (fHandle->IsOwner()) std::cout << " (owner)";
        else std::cout << " (released)";
    }
    std::cout << std::endl;
    TROOT::IncreaseDirLevel();
    const TObject* ptr = GetPointerValue();
    if (ptr) ptr->ls(opt);
    TROOT::DecreaseDirLevel();
}
