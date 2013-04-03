// $Id: TDataVector.cxx,v 1.10 2010/01/14 01:48:57 mcgrew Exp $
//
// Implement the CP::TDataVector class.  This is a very simple class so almost
// all of the methods are implemented in line.  However the ClassImp
// macro must be in a separate file and the virtual distructor is also
// implemented here.
#include <algorithm>

#include <TBrowser.h>

#include "TDataVector.hxx"

ClassImp(CP::TDataVector);

// The default destructor.
CP::TDataVector::~TDataVector() {
    Clear();
}

CP::TDataVector::iterator CP::TDataVector::begin(void) {
    return iterator(this,fVector.begin());
}

CP::TDataVector::iterator CP::TDataVector::end(void) {
    return iterator(this,fTemporary.end());
}

CP::TDataVector::const_iterator CP::TDataVector::begin(void) const {
    return const_iterator(this,fVector.begin());
}

CP::TDataVector::const_iterator CP::TDataVector::end(void) const {
    return const_iterator(this,fTemporary.end());
}

CP::TDataVector::reverse_iterator CP::TDataVector::rbegin(void) {
    return reverse_iterator(end());
}

CP::TDataVector::reverse_iterator CP::TDataVector::rend(void) {
    return reverse_iterator(begin());
}

CP::TDataVector::const_reverse_iterator CP::TDataVector::rbegin(void) const {
    return const_reverse_iterator(end());
}

CP::TDataVector::const_reverse_iterator CP::TDataVector::rend(void) const {
    return const_reverse_iterator(begin());
}

CP::TDataVector::iterator CP::TDataVector::erase(TDatum* element) {
    if (element) {
        // Protect against trying to removing an element that not part of this
        // TDataVector.
        if (element->GetParentDatum() != this) throw ENoSuchElement();
        element->AssignParentDatum(NULL);
    }
 
    // Now make sure that the element is removed from this objects storage.
    TDatumVector::iterator i = std::find(fVector.begin(),fVector.end(),element);
    if (i != fVector.end()) return iterator(this,fVector.erase(i));

    i = std::find(fTemporary.begin(),fTemporary.end(),element);
    return iterator(this,fTemporary.erase(i));
}    

CP::TDataVector::iterator CP::TDataVector::erase(CP::THandle<CP::TDatum> e) {
    return erase(CP::GetPointer(e));
}

CP::TDataVector::iterator CP::TDataVector::erase(iterator position) {
    // Protect against passing an iterator to the end. (A common mistake).
    if (position == end()) return end();

    return erase(*position);
}    

CP::TDataVector::iterator CP::TDataVector::insert(iterator position, 
                                                  TDatum* element) {
    if (!element) return end();
    element->SetBit(kCanDelete,false);

    // Check if "position" is part of fVector, and insert the element at the
    // end of the permenant objects if it's not.
    CP::TDatumVector::iterator i
        = std::find(fVector.begin(),fVector.end(),*position);
    return iterator(this,fVector.insert(i,element));
}

void CP::TDataVector::push_back(TDatum* element) {
    AddDatum(element);
}

// Find a TDatum in the data vector. 
CP::TDatum* CP::TDataVector::FindDatum(const char* name) {
    TDatumVector::reverse_iterator i;
    
    // Check to see if the object is in the temporary storage.
    i = std::find_if(fTemporary.rbegin(),fTemporary.rend(),
                CP::TDatumCompareName(name));
    if (i != fTemporary.rend()) return (*i);

    // Check to see if the object is in the main storage.
    i = std::find_if(fVector.rbegin(),fVector.rend(),
                     CP::TDatumCompareName(name));
    if (i != fVector.rend()) return (*i);
    
    return NULL;
}

// Remove the element from this TDataVector object.
CP::TDatum* CP::TDataVector::RemoveDatum(CP::TDatum* element) {
    CP::TDataVector::iterator i = std::find(begin(),end(), element);
    if (i == end()) return NULL;
    erase(i);
    return element;
}

// Insert the element into the TDataVector object.
void CP::TDataVector::InsertDatum(CP::TDatum* val) {
    if (!val) return;
    val->ReassignParentDatum(this);
    fVector.push_back(val);
}

// Insert the element into the temporary storage. 
void CP::TDataVector::InsertTemporary(CP::TDatum* val) {
    if (!val) return;
    val->ReassignParentDatum(this);
    fTemporary.push_back(val);
}

// Add a temporary datum to the TDataVector object.  The temporary object will
// be saved in fTemporary.
void CP::TDataVector::AddTemporary(CP::TDatum* val, const char* name) {
    if (!val) {
        ND280Severe("Datum pointer has null value");
        throw EBadInsertion();
    }
    if (name) val->SetName(name);
    val->SetBit(kCanDelete,false);
    InsertTemporary(val);
}

// Add a temporary datum to the TDataVector object.  The temporary object will
// be saved in fTemporary.
void CP::TDataVector::AddTemporary(CP::THandle<CP::TDatum> handle,
                                   const char* name) {
    // Make sure the handle is valid;
    if (!handle) throw EBadInsertion();
    // Make sure that handle isn't holding an object that already belongs
    // in the event.
    if (handle->GetParentDatum()) {
        ND280Severe("Datum appears to have already been inserted");
        throw EBadInsertion();
    }
    handle.Release();
    TDatum* val = GetPointer(handle);
    AddTemporary(val,name);
}

bool CP::TDataVector::IsTemporary(CP::TDatum* val) {
    return (fTemporary.end() 
            != std::find(fTemporary.begin(), fTemporary.end(),val));
}

bool CP::TDataVector::IsTemporary(CP::THandle<CP::TDatum> val) {
    return (fTemporary.end() 
            != std::find(fTemporary.begin(), fTemporary.end(),
                         CP::GetPointer(val)));
}

// Delete the contents of the CP::TDataVector.
void CP::TDataVector::Clear(Option_t*) {
    for (iterator i = begin(); i != end(); ++i) {
        (*i)->AssignParentDatum(NULL);
        delete (*i);
    }
}

// Add all of the contents to the browser.
void CP::TDataVector::Browse(TBrowser *b) {
    if (b) {
        for (iterator i = begin(); i != end(); ++i) {
            b->Add(*i);
        }
    }
    else {
        TObject::Browse(b);
    }
}

void CP::TDataVector::ls(Option_t* opt) const {
    CP::TData::ls(opt);
    TROOT::IncreaseDirLevel();
    for (const_iterator v = begin(); v != end(); ++v) {
        (*v)->ls(opt);
    };
    TROOT::DecreaseDirLevel();
}

#ifdef USE_THIS_HERE
CP::TDataVectorIterator::TDataVectorIterator(
    const CP::TDataVector* obj,
    const CP::TDatumVector::iterator current) {
    // Cast to a non-const version since both the const and non-const version
    // of begin are required.
    fObject = const_cast<CP::TDataVector*>(obj);
    fCurrent = current;
    if (fCurrent == fObject->fVector.end()) {
        fCurrent = fObject->fTemporary.begin();
    }
}

bool CP::TDataVectorIterator::operator==(const TDataVectorIterator& rhs) const {
    if (fObject != rhs.fObject) return false;
    if (fCurrent != rhs.fCurrent) return false;
    return true;
}

bool CP::TDataVectorIterator::operator!=(const TDataVectorIterator& rhs) const {
    return !((*this) == rhs);
}

CP::TDataVectorIterator&
CP::TDataVectorIterator::operator++() {
    ++fCurrent;
    if (fCurrent == fObject->fVector.end()) {
        fCurrent = fObject->fTemporary.begin();
    }
    return *this;
}

CP::TDataVectorIterator
CP::TDataVectorIterator::operator++(int) {
    CP::TDataVectorIterator tmp(*this);
    ++(*this);
    return tmp;
}

CP::TDataVectorIterator&
CP::TDataVectorIterator::operator--() {
    if (fCurrent == fObject->fTemporary.begin()) {
        fCurrent = fObject->fVector.end();
    }
    --fCurrent;
    return *this;
}

CP::TDataVectorIterator
CP::TDataVectorIterator::operator--(int) {
    CP::TDataVectorIterator tmp(*this);
    --(*this);
    return tmp;
}

CP::TDataVectorIterator::reference
CP::TDataVectorIterator::operator*() const {
    return *fCurrent;
}

CP::TDataVectorIterator::pointer 
CP::TDataVectorIterator::operator->() const {
    return &(*fCurrent);
}
#endif
