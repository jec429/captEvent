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
    std::set<ND::THandleBase*> *gHandleSet = NULL;
}

ClassImp(ND::THandleBase);
ND::THandleBase::THandleBase() : fCount(0) {
    ++gHandleBaseCount;
    if (gHandleSet) gHandleSet->insert(this);
}
ND::THandleBase::~THandleBase() {
    --gHandleBaseCount;
    if (gHandleSet) gHandleSet->erase(this);
}

ClassImp(ND::THandleBaseDeletable);
ND::THandleBaseDeletable::THandleBaseDeletable() 
    : fObject(NULL) { }
ND::THandleBaseDeletable::THandleBaseDeletable(TObject* pointee)
    : fObject(pointee) { }
ND::THandleBaseDeletable::~THandleBaseDeletable() {
    if (!fObject) return;
    // Actually delete the object.
    if (IsOwner()) delete fObject;
}

ClassImp(ND::THandleBaseUndeletable);
ND::THandleBaseUndeletable::THandleBaseUndeletable() : fObject(NULL) { }
ND::THandleBaseUndeletable::THandleBaseUndeletable(TObject* pointee)
    : fObject(pointee) { }
ND::THandleBaseUndeletable::~THandleBaseUndeletable() {
    // Absolutely nothing to do.
}

bool ND::CleanHandleRegistry(bool) {
    bool result = (gHandleBaseCount==gLastHandleCount);
    if (!result) {
        ND280Log("CleanHandleRegistry::"
                 << " Handle Count: " << gHandleBaseCount 
                 << " Change: " << gHandleBaseCount - gLastHandleCount);
        gLastHandleCount = gHandleBaseCount;
    }
    return result;
}

void ND::DumpHandleRegistry() {
    if (!gHandleSet) return;
    if (gHandleSet->empty()) return;
    ND280Log("Existing handles: " << gHandleSet->size());
    ND::TND280Log::IncreaseIndentation();
    for (std::set<ND::THandleBase*>::iterator h = gHandleSet->begin();
         h != gHandleSet->end();
         ++h) {
        ND::THandleBase* handleBase = *h;
        ND280Log(std::hex << "(0x" << handleBase << ")");
        if (handleBase) {
            ND::TND280Log::IncreaseIndentation();
            TObject *object = handleBase->GetObject();
            ND280Log(std::hex << "-> (0x" << object << ")");
            if (object) {
                ND::TND280Log::IncreaseIndentation();
                ND280Log("Class: " << object->ClassName());
                ND280Log("Name: " << object->GetName());
                ND::TND280Log::DecreaseIndentation();
                if (ND::TND280Log::GetDebugLevel()>ND::TND280Log::ErrorLevel) {
                    object->ls();
                }
            }
            ND::TND280Log::DecreaseIndentation();
        }
    }
    ND::TND280Log::DecreaseIndentation();
} 

void ND::EnableHandleRegistry(bool enable) {
    if (enable && !gHandleSet) {
        ND280Log("Enable the handle registry");
        gHandleSet = new std::set<ND::THandleBase*>;
    }
    else { 
        ND280Log("Disable the handle registry");
        delete gHandleSet;
        gHandleSet = NULL;
    }
}

ClassImp(ND::TVHandle);
ND::TVHandle::TVHandle() {Default(NULL);}
ND::TVHandle::~TVHandle() {}

void ND::TVHandle::Default(ND::THandleBase* handle) {
    fHandle = handle;
    if (fHandle) fHandle->IncrementReferenceCount();
}

void ND::TVHandle::Link(const ND::TVHandle& rhs) {
    // Copy the handle.
    fHandle = rhs.fHandle;
    // Increment the reference count
    if (fHandle) fHandle->IncrementReferenceCount();
}

bool ND::TVHandle::Unlink() {
    if (!fHandle) return false;
    fHandle->DecrementReferenceCount();
    if (fHandle->GetReferenceCount() < 1) return true;
    return false;
}

void ND::TVHandle::Destroy(void) {
    // Save the value of the handle
    ND::THandleBase* target = fHandle;
    
    // But, mark the current object as invalid.
    fHandle = NULL;
    
    // Is the target a valid handle?
    if (!target) return;
    
    // Try to delete the object.  The target is a THandleBase and its
    // distructor will decide if the object is deletable.
    delete target;
}

TObject* ND::TVHandle::GetPointerValue() const {
    if (!fHandle) return NULL;
    return fHandle->GetObject();
}

void ND::TVHandle::Release(void) {
    if (!fHandle) return;
    fHandle->Release();
}

bool ND::TVHandle::operator == (const ND::TVHandle& rhs) const {
    if (fHandle == rhs.fHandle) return true;
    return fHandle && (fHandle->GetObject() == rhs.fHandle->GetObject());
}

void ND::TVHandle::ls(Option_t *opt) const {
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
