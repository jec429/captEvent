#include <iostream>
#include <map>
#include <set>

#include <TROOT.h>
#include <TClass.h>

#include "THandle.hxx"
#include "THandleHack.hxx"
#include "TND280Log.hxx"

namespace {
    int gHandleBaseCount = 0;
    int gLastHandleCount = 0;
    std::set<CP::THandleBase*> *gHandleSet = NULL;
}

ClassImp(CP::THandleBase);
CP::THandleBase::THandleBase() : fCount(0) {
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
    if (!fObject) return;
    // Actually delete the object.
    if (IsOwner()) delete fObject;
}

ClassImp(CP::THandleBaseUndeletable);
CP::THandleBaseUndeletable::THandleBaseUndeletable() : fObject(NULL) { }
CP::THandleBaseUndeletable::THandleBaseUndeletable(TObject* pointee)
    : fObject(pointee) { }
CP::THandleBaseUndeletable::~THandleBaseUndeletable() {
    // Absolutely nothing to do.
}

bool CP::CleanHandleRegistry(bool) {
    bool result = (gHandleBaseCount==gLastHandleCount);
    if (!result) {
        ND280Log("CleanHandleRegistry::"
                 << " Handle Count: " << gHandleBaseCount 
                 << " Change: " << gHandleBaseCount - gLastHandleCount);
        gLastHandleCount = gHandleBaseCount;
    }
    return result;
}

void CP::DumpHandleRegistry() {
    if (!gHandleSet) return;
    if (gHandleSet->empty()) return;
    ND280Log("Existing handles: " << gHandleSet->size());
    CP::TND280Log::IncreaseIndentation();
    for (std::set<CP::THandleBase*>::iterator h = gHandleSet->begin();
         h != gHandleSet->end();
         ++h) {
        CP::THandleBase* handleBase = *h;
        ND280Log(std::hex << "(0x" << handleBase << ")");
        if (handleBase) {
            CP::TND280Log::IncreaseIndentation();
            TObject *object = handleBase->GetObject();
            ND280Log(std::hex << "-> (0x" << object << ")");
            if (object) {
                CP::TND280Log::IncreaseIndentation();
                ND280Log("Class: " << object->ClassName());
                ND280Log("Name: " << object->GetName());
                CP::TND280Log::DecreaseIndentation();
                if (CP::TND280Log::GetDebugLevel()>CP::TND280Log::ErrorLevel) {
                    object->ls();
                }
            }
            CP::TND280Log::DecreaseIndentation();
        }
    }
    CP::TND280Log::DecreaseIndentation();
} 

void CP::EnableHandleRegistry(bool enable) {
    if (enable && !gHandleSet) {
        ND280Log("Enable the handle registry");
        gHandleSet = new std::set<CP::THandleBase*>;
    }
    else { 
        ND280Log("Disable the handle registry");
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

bool CP::TVHandle::Unlink() {
    if (!fHandle) return false;
    fHandle->DecrementReferenceCount();
    if (fHandle->GetReferenceCount() < 1) return true;
    return false;
}

void CP::TVHandle::Destroy(void) {
    // Save the value of the handle
    CP::THandleBase* target = fHandle;
    
    // But, mark the current object as invalid.
    fHandle = NULL;
    
    // Is the target a valid handle?
    if (!target) return;
    
    // Try to delete the object.  The target is a THandleBase and its
    // distructor will decide if the object is deletable.
    delete target;
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
    return fHandle && (fHandle->GetObject() == rhs.fHandle->GetObject());
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
