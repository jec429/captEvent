#include <algorithm>
#include <iostream>
#include <sstream>

#include <tut.h>

#include "TDatum.hxx"
#include "TData.hxx"
#include "TDataVector.hxx"
#include "TDataSymLink.hxx"
#include "TRealDatum.hxx"

namespace tut {

    /// Test CP::TDatum and CP::TDataVector.  This tests the object access
    /// methods of both TDatum and TDataVector (the test is badly named.).
    struct baseTDatum {
        // A simple CP::TDataVector and a standard vector used to cross check.
        CP::TDataVector *fVector;
        std::vector<CP::TDatum*> fCheckVector;

        baseTDatum() {
            // Run before each test.
            fVector = new CP::TDataVector("parent");
            for (int i=0; i<5; ++i) {
                std::ostringstream n1;
                n1 << "child." << i;
                CP::TDataVector *v1 = new CP::TDataVector(n1.str().c_str());
                fVector->push_back(v1);
                fCheckVector.push_back(v1);
                for (int j=0; j<5; ++j) {
                    std::ostringstream n2;
                    n2 << "child." << i << "." << j;
                    CP::TDatum *d1 = new CP::TDatum(n2.str().c_str());
                    v1->push_back(d1);
                }
            }
        }

        ~baseTDatum() {
            // Run after each test.
            delete fVector;
        }
        
    };

    // Declare the test
    typedef test_group<baseTDatum>::object testTDatum;
    test_group<baseTDatum> groupTDatum("TDatum");

    // Test the basic constructor and destructor.
    template<> template <>
    void testTDatum::test<1> () {
        CP::TDatum *data = new CP::TDatum("data");
        CP::TDataVector *vect = new CP::TDataVector("vect");
        CP::TDataSymLink *link = new CP::TDataSymLink("lnk");

        ensure("TDatum constructor",data);
        ensure("TDataVector constructor", vect);
        ensure("TDataSymLink constructor", link);

        delete data;
        delete vect;
        delete link;
    }

    namespace {
        bool VectorsEqual(const std::vector<CP::TDatum*>& vect,
                          const CP::TDataVector& data) {
            if (vect.size() != data.size()) return false;
            std::vector<CP::TDatum*>::const_iterator v = vect.begin();
            CP::TDataVector::const_iterator d = data.begin();
            for (; v != vect.end(); ++v, ++d) {
                if (*v != *d) return false;
            }
            return true;
        }
    }

    // Test the index element access.
    template <> template <> 
    void testTDatum::test<2> () {
        ensure_equals("vector length", fCheckVector.size(), fVector->size());
        ensure("Order matches insertion order", 
               VectorsEqual(fCheckVector,*fVector));
        ensure_equals("TDataVector::At()", 
                      fCheckVector.at(2), fVector->At<CP::TDatum>(2));
        ensure_equals("TDataVector::operator[]", 
                      fCheckVector[2], fVector->operator[](2));
    }

    // Test insertion
    template <> template <> 
    void testTDatum::test<3> () {
        std::vector<CP::TDatum*>::iterator v = ++fCheckVector.begin();
        CP::TDataVector::iterator d = ++fVector->begin();

        // Check insertion.
        CP::TDataVector *newObject = new CP::TDataVector("newObject.1");
        fCheckVector.insert(v,newObject);

        // The new object is now owned by fVector.
        fVector->insert(d,newObject);

        ensure("Insertion into vector", VectorsEqual(fCheckVector,*fVector));

        v = ++fCheckVector.begin();
        d = ++fVector->begin();

        ensure_equals("Inserted correctly into std::vector",
                      newObject, *v);
        ensure_equals("Inserted correctly into TDataVector",
                      newObject, *d);
    }

    // Test erasure.
    template <> template <> 
    void testTDatum::test<4> () {
        std::vector<CP::TDatum*>::iterator v = ++fCheckVector.begin();

        CP::TDatum *erasure = *v;
        
        // Check erasure.
        fCheckVector.erase(v);
        fVector->erase(erasure);
        ensure("Erase from vector", VectorsEqual(fCheckVector,*fVector));
    }

    // Class to test that CP::TDataVector deletes children as well as parent.
    class testTDatumDelete: public CP::TDatum {
    public:
        static int count;
        testTDatumDelete() {++count;}
        testTDatumDelete(const char* name, const char* title = "testDelete")
            : CP::TDatum(name,title) {++count;}
        virtual ~testTDatumDelete() {--count;}
    };
    int testTDatumDelete::count = 0;

    // Test that CP::TDataVector children are deleted when parent
    // CP::TDataVector is deleted.
    template <> template <>
    void testTDatum::test<5> () {
        int count = 0;
        CP::TDataVector *vect = new CP::TDataVector("parent");
        for (int i=0; i<4; ++i) {
            std::ostringstream name;
            name << "child." << i;
            CP::TDatum *c1 = new testTDatumDelete(name.str().c_str());
            ++count;
            vect->push_back(c1);
        }

        ensure("Some CP::TDataVector children created", (0<count));
        ensure_equals("TDataVector children created",
                      testTDatumDelete::count, count);

        delete vect;
        ensure_equals("All CP::TDataVector children deleted",
                      testTDatumDelete::count, 0);
    }

    // Test the Get member template
    template <> template <> 
    void testTDatum::test<6> () {
        CP::THandle<CP::TDatum> d1 = fVector->Get<CP::TDatum>("child.2");
        ensure("Name found", d1);
        ensure_equals("Name matchs", d1->GetName(), std::string("child.2"));
    }

    // Test the Get member template with a recursive search
    template <> template <> 
    void testTDatum::test<7> () {
        CP::THandle<CP::TDatum> d1 
            = fVector->Get<CP::TDatum>("child.2/child.2.3");
        ensure("Name found", d1);
        ensure_equals("Name matchs", d1->GetName(), std::string("child.2.3"));
    }

    // Test the Get member template rooted in the named parent.
    template <> template <> 
    void testTDatum::test<8> () {
        CP::THandle<CP::TDatum> d1 
            = fVector->Get<CP::TDatum>("child.2/child.2.3");
        CP::THandle<CP::TDatum> d2 
            = d1->Get<CP::TDatum>("//parent/child.3/child.3.2");
        ensure("Name found", d2);
        ensure_equals("Name matchs", d2->GetName(), std::string("child.3.2"));
    }

    // Test the Get member template from the top level parent (without name).
    template <> template <> 
    void testTDatum::test<9> () {
        CP::THandle<CP::TDatum> d1 
            = fVector->Get<CP::TDatum>("child.2/child.2.3");
        CP::THandle<CP::TDatum> d2 
            = d1->Get<CP::TDatum>("~/child.3/child.3.2");
        ensure("Name found", d2);
        ensure_equals("Name matchs", d2->GetName(), std::string("child.3.2"));
    }

    // Test the Get member template with a nearest parent.
    template <> template <> 
    void testTDatum::test<10> () {
        CP::THandle<CP::TDatum> d1 
            = fVector->Get<CP::TDatum>("child.2/child.2.3");
        CP::THandle<CP::TDatum> d2 
            = d1->Get<CP::TDatum>("/child.2/child.2.2");
        ensure("Name found", d2);
        ensure_equals("Name matchs", d2->GetName(), std::string("child.2.2"));
    }

    // Test the Get member template with a relative search.
    template <> template <> 
    void testTDatum::test<11> () {
        CP::THandle<CP::TDatum> d1 
            = fVector->Get<CP::TDatum>("child.2/child.2.3");
        CP::THandle<CP::TDatum> d2 
            = d1->Get<CP::TDatum>("../child.2.2");
        ensure("Name found", d2);
        ensure_equals("Name matchs", d2->GetName(), std::string("child.2.2"));
    }

    // Test getting the root parent.
    template <> template <> 
    void testTDatum::test<12> () {
        CP::THandle<CP::TDatum> d1 
            = fVector->Get<CP::TDatum>("child.2/child.2.3");
        CP::THandle<CP::TDatum> d2 
            = d1->Get<CP::TDatum>("~/");
        ensure("Name found", d2);
        ensure_equals("Name matchs", d2->GetName(), std::string("parent"));
    }

    // Test getting the root parent.
    template <> template <> 
    void testTDatum::test<13> () {
        CP::THandle<CP::TDatum> d1 
            = fVector->Get<CP::TDatum>("child.2/child.2.3");
        CP::THandle<CP::TDatum> d2 
            = d1->Get<CP::TDatum>("//");
        ensure("Name found", d2);
        ensure_equals("Name matchs", d2->GetName(), std::string("parent"));
    }

    // Test converting to an incorrect class.
    template <> template <> 
    void testTDatum::test<14> () {
        try {
            CP::THandle<CP::TDataVector> d1 
                = fVector->Get<CP::TDataVector>("child.2/child.2.3");
            fail("Bad data conversion should fail");
        }
        catch (CP::EBadConversion) {}
    }

    /// Test the Has member template.
    template <> template <> 
    void testTDatum::test<15> () {
        ensure("Has finds an contained element",
               fVector->Has<CP::TDataVector>("child.2"));
        ensure("Has reports a missing element",
               !fVector->Has<CP::TDataVector>("notThere"));
    }

    /// Test the Use member template.
    template <> template <> 
    void testTDatum::test<16> () {
        CP::TDataVector& child = fVector->Use<CP::TDataVector>("child.2");
        ensure_equals("The child.2 element was found",
                      child.GetName(), std::string("child.2"));
        try {
            fVector->Use<CP::TDataVector>("notThere").GetName();
            fail("Non-existent element should cause exception");
        } 
        catch (CP::ENoSuchElement) {}
        try {
            fVector->Use<CP::TRealDatum>("child.2").GetName();
            fail("Wrong class element should cause exception");
        } 
        catch (CP::EBadConversion) {}
    }

    // Test indexed access to the wrong data type.
    template <> template <> 
    void testTDatum::test<17> () {
        CP::TDataVector* d1 = fVector->At<CP::TDataVector>(2);
        ensure("Indexed TDataVector was found", d1);
        try {
            CP::TDataVector* d2 = d1->At<CP::TDataVector>(0);
            if (d2 != NULL) fail("Bad data conversion did not fail");
            fail("Bad data conversion did not throw exception");
        }
        catch (CP::EBadConversion) {}
    }

    // Test indexed access past end of vector.
    template <> template <> 
    void testTDatum::test<18> () {
        try {
            CP::TDatum* d1 = fVector->At<CP::TDatum>(fVector->size());
            if (d1 != NULL) fail("Access past end of vector did not fail.");
            fail("Access past end of vector did not throw exception.");
        }
        catch (CP::ENoSuchElement) {}
    }

    // Test that temporary objects can be added to the TDataVector.
    template <> template <>
    void testTDatum::test<19> () {
        CP::TDataVector *newObject = new CP::TDataVector("newObject.1");
        fVector->AddTemporary(newObject);
        ensure("Object was added to TDataVector.",
               fVector->end() != std::find(fVector->begin(),
                                          fVector->end(),
                                          newObject));
        ensure("Object is saved as non persistent",
               fVector->IsTemporary(newObject));
    }

    // Test that temporary objects found in the TDataVector.
    template <> template <>
    void testTDatum::test<20> () {
        CP::TDataVector *newObject = new CP::TDataVector("newObject.1");
        fVector->AddTemporary(newObject);
        CP::THandle<CP::TDataVector> obj 
            = fVector->Get<CP::TDataVector>("newObject.1");
        ensure("Temporary object was found in TDataVector.",obj);
    }

    // Test that temporary objects mask persistent objects in the TDataVector.
    template <> template <>
    void testTDatum::test<21> () {
        CP::THandle<CP::TDataVector> persist 
            = fVector->Get<CP::TDataVector>("child.1");
        ensure("Object was found in TDataVector.",persist);
        ensure("Object is persistent",!fVector->IsTemporary(persist));
        CP::TDataVector *newObject = new CP::TDataVector("child.1");
        fVector->AddTemporary(newObject);
        CP::THandle<CP::TDataVector> temp 
            = fVector->Get<CP::TDataVector>("child.1");
        ensure("Temporary masks persistent object.",
               fVector->IsTemporary(temp));
        ensure("Temporary and persistent objects are different",
               !(persist == temp));

        // Erase the non-persistent object from the vector and make sure that
        // the persistent one is visible.
        fVector->erase(temp);
        CP::THandle<CP::TDataVector> exposed 
            = fVector->Get<CP::TDataVector>("child.1");
        ensure("See original object after masking temporary is removed",
               exposed == persist);
    }

    // Test that size works with temporary objects
    template <> template <>
    void testTDatum::test<22> () {
        CP::THandle<CP::TDataVector> vector(new CP::TDataVector);
        // Insert stuff into the data vector
        vector->AddDatum(new CP::TDatum("A"));
        vector->AddDatum(new CP::TDatum("B"));
        vector->AddDatum(new CP::TDatum("C"));
        ensure_equals("Vector size matches number of inserts",
                      vector->size(),(unsigned) 3);
        vector->AddTemporary(new CP::TDatum("D"));
        vector->AddTemporary(new CP::TDatum("E"));
        vector->AddTemporary(new CP::TDatum("F"));
        ensure_equals("Vector size with temporaries matches number of inserts",
                      vector->size(),(unsigned)6);
    }

    // Test that iterator works
    template <> template <>
    void testTDatum::test<23> () {
        CP::THandle<CP::TDataVector> vector(new CP::TDataVector);
        // Insert stuff into the data vector
        vector->AddDatum(new CP::TDatum("A"));
        vector->AddDatum(new CP::TDatum("B"));
        vector->AddDatum(new CP::TDatum("C"));
        int count = 0;
        for (CP::TDataVector::iterator v = vector->begin(); 
             v != vector->end(); ++v) {
            ++count;
        }
        ensure_equals("Forward iterator went through all elements",count,3);

        CP::TDataVector::iterator v = vector->begin();
        ensure_equals("Element is A",
                      std::string((*v)->GetName()),std::string("A"));
        ++v;
        ensure_equals("Element is B",
                      std::string((*v)->GetName()),std::string("B"));
        ++v;
        ensure_equals("Element is C",
                      std::string((*v)->GetName()),std::string("C"));
        ++v;
        ensure("Element is end",v == vector->end());
    }

    // Test that reverse_iterator works
    template <> template <>
    void testTDatum::test<24> () {
        CP::THandle<CP::TDataVector> vector(new CP::TDataVector);
        // Insert stuff into the data vector
        vector->AddDatum(new CP::TDatum("A"));
        vector->AddDatum(new CP::TDatum("B"));
        vector->AddDatum(new CP::TDatum("C"));
        int count = 0;
        for (CP::TDataVector::reverse_iterator v = vector->rbegin(); 
             v != vector->rend(); ++v) {
            ++count;
        }
        ensure_equals("Reverse iterator went through all elements",count,3);

        CP::TDataVector::reverse_iterator v = vector->rbegin();
        ensure_equals("Element is C",
                      std::string((*v)->GetName()),std::string("C"));
        ++v;
        ensure_equals("Element is B",
                      std::string((*v)->GetName()),std::string("B"));
        ++v;
        ensure_equals("Element is A",
                      std::string((*v)->GetName()),std::string("A"));
        ++v;
        ensure("Element is end", v==vector->rend());
    }

    // Test that iterator works with temporary objects
    template <> template <>
    void testTDatum::test<25> () {
        CP::THandle<CP::TDataVector> vector(new CP::TDataVector);
        // Insert stuff into the data vector
        vector->AddDatum(new CP::TDatum("A"));
        vector->AddDatum(new CP::TDatum("B"));
        vector->AddDatum(new CP::TDatum("C"));
        vector->AddTemporary(new CP::TDatum("D"));
        vector->AddTemporary(new CP::TDatum("E"));
        vector->AddTemporary(new CP::TDatum("F"));
        int count = 0;
        for (CP::TDataVector::iterator v = vector->begin(); 
             v != vector->end(); ++v) {
            ++count;
        }
        ensure_equals("Forward iterator went through all elements",count,6);

        CP::TDataVector::iterator v = vector->begin();
        ensure_equals("Element is A",
                      std::string((*v)->GetName()),std::string("A"));
        ++v;
        ensure_equals("Element is B",
                      std::string((*v)->GetName()),std::string("B"));
        ++v;
        ensure_equals("Element is C",
                      std::string((*v)->GetName()),std::string("C"));
        ++v;
        ensure_equals("Element is D",
                      std::string((*v)->GetName()),std::string("D"));
        ++v;
        ensure_equals("Element is E",
                      std::string((*v)->GetName()),std::string("E"));
        ++v;
        ensure_equals("Element is F",
                      std::string((*v)->GetName()),std::string("F"));
        ++v;
        ensure("Element is end",v == vector->end());
    }

    // Test that reverse_iterator works with temporary objects
    template <> template <>
    void testTDatum::test<26> () {
        CP::THandle<CP::TDataVector> vector(new CP::TDataVector);
        // Insert stuff into the data vector
        vector->AddDatum(new CP::TDatum("A"));
        vector->AddDatum(new CP::TDatum("B"));
        vector->AddDatum(new CP::TDatum("C"));
        vector->AddTemporary(new CP::TDatum("D"));
        vector->AddTemporary(new CP::TDatum("E"));
        vector->AddTemporary(new CP::TDatum("F"));
        int count = 0;
        for (CP::TDataVector::reverse_iterator v = vector->rbegin(); 
             v != vector->rend(); ++v) {
            ++count;
        }
        ensure_equals("Reverse iterator went through all elements",count,6);

        CP::TDataVector::reverse_iterator v = vector->rbegin();
        ensure_equals("Element is F",
                      std::string((*v)->GetName()),std::string("F"));
        ++v;
        ensure_equals("Element is E",
                      std::string((*v)->GetName()),std::string("E"));
        ++v;
        ensure_equals("Element is D",
                      std::string((*v)->GetName()),std::string("D"));
        ++v;
        ensure_equals("Element is C",
                      std::string((*v)->GetName()),std::string("C"));
        ++v;
        ensure_equals("Element is B",
                      std::string((*v)->GetName()),std::string("B"));
        ++v;
        ensure_equals("Element is A",
                      std::string((*v)->GetName()),std::string("A"));
        ++v;
        ensure("Element is end",v == vector->rend());
    }
};
