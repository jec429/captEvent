#include <iostream>
#include <memory>
#include <tut.h>

// Unbelievably ugly hack to let me test private methods.
#define private public
#define protected public
#include "TDigit.hxx"
#include "TDigitProxy.hxx"
#include "TDigitManager.hxx"
#undef private
#undef protected

#include "TEvent.hxx"
#include "TManager.hxx"
#include "TDataHit.hxx"
#include "TGeometryId.hxx"
#include "CaptGeomId.hxx"

namespace tut {
    struct baseTDigit {
        baseTDigit() {
            // Run before each test.
        }
        ~baseTDigit() {
            // Run after each test.
        }
    };

    // Declare the test
    typedef test_group<baseTDigit>::object testTDigit;
    test_group<baseTDigit> groupTDigit("TDigit");

    // Test the default constructor and destructor.
    template<> template<>
    void testTDigit::test<1> () {
        CP::TManager::Get().Digits();
    }

    // Test the TDigitProxy type setters and getters work
    template <> template <>
    void testTDigit::test<2> () {
        CP::TDigitProxy proxy;
        
        // Check that the uninitialized digit is invalid
        ensure("Uninitialized digit is invalid", !proxy.IsValid());

        // Check that we can set the full range of proxy types.
        for (int val = 0; val<32; ++val) {
            proxy.SetProxyType(val);
            int i = proxy.GetProxyType();
            int j = proxy.GetProxySalt();
            int k = proxy.GetProxyOffset();
            ensure_equals("ProxyType set", i, val);
            ensure_equals("ProxySalt not set", j, 0);
            ensure_equals("ProxyOffset not set", k, 0);
        }
    }

    // Test the TDigitProxy salt setters and getters work
    template <> template <>
    void testTDigit::test<3> () {
        CP::TDigitProxy proxy;
        
        // Check that we can set the full range of proxy types.
        for (int val = 0; val<3000; ++val) {
            proxy.SetProxySalt(val);
            int i = proxy.GetProxyType();
            int j = proxy.GetProxySalt();
            int k = proxy.GetProxyOffset();
            ensure_equals("ProxyType not set", i, 0);
            ensure_equals("ProxySalt set", j, val%1024);
            ensure_equals("ProxyOffset not set", k, 0);
        }
    }

    // Test the TDigitProxy offset setters and getters work
    template <> template <>
    void testTDigit::test<4> () {
        CP::TDigitProxy proxy;
        
        // Check that we can set the full range of proxy types.
        for (int val = 0; val<131071; val += 321) {
            proxy.SetProxyOffset(val);
            int i = proxy.GetProxyType();
            int j = proxy.GetProxySalt();
            int k = proxy.GetProxyOffset();
            ensure_equals("ProxyType not set", i, 0);
            ensure_equals("ProxySalt not set", j, 0);
            ensure_equals("ProxyOffset set", k, val);
        }
    }

    class FactoryForTest: public CP::TDigitFactory {
    public:
        FactoryForTest() : CP::TDigitFactory("test") {};
        ~FactoryForTest() {}
        CP::TDigitContainer* MakeDigits() {
            CP::TDigitContainer* digits = new CP::TDigitContainer();

            for (int i = 0; i<100; ++i) {
                CP::TDigit* digit 
                    = new CP::TDigit(CP::TChannelId(0xF0000000 + i));
                digits->push_back(digit);
            }

            return digits;
        }
    };

    // Test that the "test" TDigitFactory can be added to TDigitManager.
    template <> template <>
    void testTDigit::test<5> () {
        ensure("Test TDigitFactory is not available",
               !CP::TManager::Get().Digits().FactoryAvailable("test"));

        CP::TManager::Get().Digits().RegisterFactory(new FactoryForTest());

        ensure("Test TDigitFactory is available",
               CP::TManager::Get().Digits().FactoryAvailable("test"));
    }

    // Test that the "test" TDigitFactory digits are created in the right place.
    template <> template <>
    void testTDigit::test<6> () {
         ensure("Test TDigitFactory is available",
               CP::TManager::Get().Digits().FactoryAvailable("test"));

         CP::TEvent event;

         CP::THandle<CP::TDigitContainer> digits = 
             CP::TManager::Get().Digits().CacheDigits("test");

         ensure("Digit container was created",digits);

         ensure_greaterthan("Digit container has digits", digits->size(), 
                            (unsigned) 1);

         CP::THandle<CP::TDigitContainer> lookup
             = event.Get<CP::TDigitContainer>("~/digits/test");

         ensure("Digit container in correct place inside event", lookup);

         ensure_equals("Found digit container same as return",
                       CP::GetPointer(lookup), CP::GetPointer(digits));

         CP::THandle<CP::TDataVector> parent = 
             digits->Get<CP::TDataVector>("..");

         ensure("Digit container is in temporary store",
                parent->IsTemporary(digits));

         ensure_greaterthan("Signature is valid", digits->GetSignature(),
                            (unsigned)0);
    }

    // Test the TDigitProxy.
    template <> template <>
    void testTDigit::test<7> () {
         CP::TEvent event;
         CP::THandle<CP::TDigitContainer> digits = event.GetDigits("test");
         ensure("Digit container was created",digits);

         std::vector<CP::TDigitProxy> proxies;
         for (unsigned int i = 0; i<digits->size(); ++i) {
             proxies.push_back(CP::TDigitProxy(*digits, i));
         }

         std::vector<CP::TDigitProxy>::iterator proxy = proxies.begin();
         CP::TDigitContainer::iterator digit = digits->begin();
         unsigned int offset = 0;
         while (proxy != proxies.end()) {
             ensure("Digits end not reached", digit != digits->end());
             ensure_equals("Proxy type is correct",
                           proxy->GetProxyType(), CP::TDigitProxy::kTest);
             ensure_equals("Proxy offset is correct",
                           proxy->GetProxyOffset(), offset);
             ensure("Proxy salt matches",
                    proxy->CheckSalt(digits->GetSignature(),*digit));

             ensure("Proxy salt does not match wrong signature",
                    !proxy->CheckSalt(digits->GetSignature()+1,*digit));

             ensure_equals("Proxy points to right address.", *(*proxy), *digit);
             
             ++offset, ++proxy; ++digit;
         }
    }

    // Test the digit access routines for TDigitProxy.
    template <> template <>
    void testTDigit::test<8> () {
         CP::TEvent event;
         CP::THandle<CP::TDigitContainer> digits = event.GetDigits("test");
         ensure("Digit container was created",digits);

         unsigned int chan = 0;
         for (CP::TDigitContainer::iterator it = digits->begin();
              it != digits->end();
              ++it) {
             CP::TDigit* digit = *it;
             ensure_equals("Digit valid",
                           digit->GetChannelId().AsUInt()&0xFFFF, chan++);
         }
    }

    // Test the digit proxies in THits.
    template <> template <>
    void testTDigit::test<9> () {
         CP::TEvent event;
         CP::THandle<CP::TDigitContainer> digits = event.GetDigits("test");
         CP::THandle<CP::THitSelection> hits(new CP::THitSelection("test"));

         // Run a fake calibration to make a hits selection
         int wireCount = 0;
         for (CP::TDigitContainer::iterator it = digits->begin();
              it != digits->end();
              ++it) {
             // Create the hit and save a proxy for this digit.
             std::auto_ptr<CP::TWritableDataHit> hit(new CP::TWritableDataHit);
             hit->SetDigit(CP::TDigitProxy(*digits,it));
             hit->SetGeomId(CP::GeomId::Captain::Wire(0,wireCount++));

             // Save the new hit in the hit selection.
             hits->push_back(CP::THandle<CP::THit>(new CP::TDataHit(*hit)));
         }

         CP::THandle<CP::TDataVector> hitDir 
             = event.Get<CP::TDataVector>("hits"); 
         hitDir->AddDatum(hits);
         
         CP::THandle<CP::THitSelection> select = event.GetHitSelection("test");

         for (CP::THitSelection::iterator it = select->begin();
              it != select->end();
              ++it) {
             CP::TDigit* d1 = *(*it)->GetDigit();
             CP::TDigit* d2 = (*it)->GetDigit().As<CP::TDigit>();
             CP::TDigitProxy dp1 = (*it)->GetDigit();
             CP::TDigit* d3 = *dp1;
             const CP::TDigitProxy& dp2 = (*it)->GetDigit();
             
             ensure("valid digit",d1);
             ensure_equals("Digits are the same",d1,d2);
             ensure_equals("Digit proxy returns same value",d1,d3);
             ensure("Digit proxies are the same", dp1 == dp2);

             CP::TChannelId c1 = d1->GetChannelId();
             CP::TChannelId c3 = (*it)->GetChannelId();
             ensure_equals("Channels are the same",c1.AsUInt(),c3.AsUInt());
         }
    }

    // Test the TDigitProxy objects created with the two different
    // constructors are the same.
    template <> template <>
    void testTDigit::test<10> () {
         CP::TEvent event;
         CP::THandle<CP::TDigitContainer> digits = event.GetDigits("test");
         ensure("Digit container was created",digits);

         std::vector<CP::TDigitProxy> offsetProxies;
         std::vector<CP::TDigitProxy> iteratorProxies;
         int digitOffset = 0;
         for (CP::TDigitContainer::iterator d = digits->begin();
              d != digits->end(); ++d) {
             iteratorProxies.push_back(CP::TDigitProxy(*digits, d));
             offsetProxies.push_back(CP::TDigitProxy(*digits, digitOffset++));
         }

         ensure_equals("Same number of offset and iterator proxies",
                       offsetProxies.size(), iteratorProxies.size());
         
         for (std::vector<CP::TDigitProxy>::size_type i=0; 
              i<offsetProxies.size(); 
              ++i) {
             ensure_equals("Proxy types are the same",
                           offsetProxies[i].GetProxyType(),
                           iteratorProxies[i].GetProxyType());
             ensure_equals("Proxy offsets are the same",
                           offsetProxies[i].GetProxyOffset(),
                           iteratorProxies[i].GetProxyOffset());
             ensure_equals("Proxy salt are the same",
                           offsetProxies[i].GetProxySalt(),
                           iteratorProxies[i].GetProxySalt());
         }
    }

    // Test the TDigitProxy validity check.
    template <> template <>
    void testTDigit::test<11> () {
        CP::TDigitProxy proxy;
        proxy.SetProxyType(proxy.ConvertName("test"));
        proxy.SetProxyOffset(0x1FFFE);
        ensure("Proxy with offset 0x1FFFE is valid", proxy.IsValid());

        proxy.SetProxyOffset(0x1FFFF);
        ensure("Proxy with offset 0x1FFFF is invalid", !proxy.IsValid());
    }        
};

