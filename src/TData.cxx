// $Id: TData.cxx,v 1.7 2010/01/14 01:48:57 mcgrew Exp $
//
// Implement the CP::TData class.  This is a very simple class so almost
// all of the methods are implemented in line.  However the ClassImp
// macro must be in a separate file and the virtual distructor is also
// implemented here.

#include "TData.hxx"

// The default destructor.
CP::TData::~TData() { }

void CP::TData::AddDatum(CP::TDatum* val, const char* name) {
    if (!val) {
        ND280Severe("Datum pointer has null value");
        throw EBadInsertion();
    }
    if (name) val->SetName(name);
    val->SetBit(kCanDelete,false);
    InsertDatum(val);
}

void CP::TData::AddDatum(CP::THandle<CP::TDatum> handle, const char* name) {
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
    AddDatum(val,name);
}

// The default RemoveDatum.  This always aborts since it must have
// been implemented by the derived class.
CP::TDatum* CP::TData::RemoveDatum(CP::TDatum*) {
    AbstractMethod("TData::RemoveDatum");
    return NULL;
}

// The default FindDatum never finds the object since there is never
// an object to find.
CP::TDatum* CP::TData::FindDatum(const char*) {
    AbstractMethod("TData::FindDatum");
    return NULL;
}

// The default InsertDatum cannot really add data since there is no
// container in the base CP::TData.
void CP::TData::InsertDatum(CP::TDatum *) {
    AbstractMethod("TData::InsertDatum");
}

