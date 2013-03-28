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

#include "TND280Event.hxx"
#include "TOADatabase.hxx"
#include "TDataHit.hxx"
#include "TGeometryId.hxx"
#include "ND280GeomId.hxx"

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
        ND::TOADatabase::Get().Digits();
    }

    // Test the TDigitProxy type setters and getters work
    template <> template <>
    void testTDigit::test<2> () {
        ND::TDigitProxy proxy;
        
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
        ND::TDigitProxy proxy;
        
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
        ND::TDigitProxy proxy;
        
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

    class FactoryForTest: public ND::TDigitFactory {
    public:
        FactoryForTest() : ND::TDigitFactory("test") {};
        ~FactoryForTest() {}
        ND::TDigitContainer* MakeDigits() {
            ND::TDigitContainer* digits = new ND::TDigitContainer();

            for (int i = 0; i<100; ++i) {
                ND::TDigit* digit 
                    = new ND::TDigit(ND::TChannelId(0xF0000000 + i));
                digits->push_back(digit);
            }

            return digits;
        }
    };

    // Test that the "test" TDigitFactory can be added to TDigitManager.
    template <> template <>
    void testTDigit::test<5> () {
        ensure("Test TDigitFactory is not available",
               !ND::TOADatabase::Get().Digits().FactoryAvailable("test"));

        ND::TOADatabase::Get().Digits().RegisterFactory(new FactoryForTest());

        ensure("Test TDigitFactory is available",
               ND::TOADatabase::Get().Digits().FactoryAvailable("test"));
    }

    // Test that the "test" TDigitFactory digits are created in the right place.
    template <> template <>
    void testTDigit::test<6> () {
         ensure("Test TDigitFactory is available",
               ND::TOADatabase::Get().Digits().FactoryAvailable("test"));

         ND::TND280Event event;

         ND::THandle<ND::TDigitContainer> digits = 
             ND::TOADatabase::Get().Digits().CacheDigits("test");

         ensure("Digit container was created",digits);

         ensure_greaterthan("Digit container has digits", digits->size(), 
                            (unsigned) 1);

         ND::THandle<ND::TDigitContainer> lookup
             = event.Get<ND::TDigitContainer>("~/digits/test");

         ensure("Digit container in correct place inside event", lookup);

         ensure_equals("Found digit container same as return",
                       ND::GetPointer(lookup), ND::GetPointer(digits));

         ND::THandle<ND::TDataVector> parent = 
             digits->Get<ND::TDataVector>("..");

         ensure("Digit container is in temporary store",
                parent->IsTemporary(digits));

         ensure_greaterthan("Signature is valid", digits->GetSignature(),
                            (unsigned)0);
    }

    // Test the TDigitProxy.
    template <> template <>
    void testTDigit::test<7> () {
         ND::TND280Event event;
         ND::THandle<ND::TDigitContainer> digits = event.GetDigits("test");
         ensure("Digit container was created",digits);

         std::vector<ND::TDigitProxy> proxies;
         for (unsigned int i = 0; i<digits->size(); ++i) {
             proxies.push_back(ND::TDigitProxy(*digits, i));
         }

         std::vector<ND::TDigitProxy>::iterator proxy = proxies.begin();
         ND::TDigitContainer::iterator digit = digits->begin();
         unsigned int offset = 0;
         while (proxy != proxies.end()) {
             ensure("Digits end not reached", digit != digits->end());
             ensure_equals("Proxy type is correct",
                           proxy->GetProxyType(), ND::TDigitProxy::kTest);
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
         ND::TND280Event event;
         ND::THandle<ND::TDigitContainer> digits = event.GetDigits("test");
         ensure("Digit container was created",digits);

         unsigned int chan = 0;
         for (ND::TDigitContainer::iterator it = digits->begin();
              it != digits->end();
              ++it) {
             ND::TDigit* digit = *it;
             ensure_equals("Digit valid",
                           digit->GetChannelId().AsUInt()&0xFFFF, chan++);
         }
    }

    // Test the digit proxies in THits.
    template <> template <>
    void testTDigit::test<9> () {
         ND::TND280Event event;
         ND::THandle<ND::TDigitContainer> digits = event.GetDigits("test");
         ND::THandle<ND::THitSelection> hits(new ND::THitSelection("test"));

         // Run a fake calibration to make a hits selection
         int barCount = 0;
         for (ND::TDigitContainer::iterator it = digits->begin();
              it != digits->end();
              ++it) {
             // Create the hit and save a proxy for this digit.
             std::auto_ptr<ND::TWritableDataHit> hit(new ND::TWritableDataHit);
             hit->SetDigit(ND::TDigitProxy(*digits,it));
             hit->SetGeomId(ND::GeomId::P0D::Bar(0,0,barCount++));

             // Save the new hit in the hit selection.
             hits->push_back(ND::THandle<ND::THit>(new ND::TDataHit(*hit)));
         }

         ND::THandle<ND::TDataVector> hitDir 
             = event.Get<ND::TDataVector>("hits"); 
         hitDir->AddDatum(hits);
         
         ND::THandle<ND::THitSelection> select = event.GetHitSelection("test");

         for (ND::THitSelection::iterator it = select->begin();
              it != select->end();
              ++it) {
             ND::TDigit* d1 = *(*it)->GetDigit();
             ND::TDigit* d2 = (*it)->GetDigit().As<ND::TDigit>();
             ND::TDigitProxy dp1 = (*it)->GetDigit();
             ND::TDigit* d3 = *dp1;
             const ND::TDigitProxy& dp2 = (*it)->GetDigit();
             
             ensure("valid digit",d1);
             ensure_equals("Digits are the same",d1,d2);
             ensure_equals("Digit proxy returns same value",d1,d3);
             ensure("Digit proxies are the same", dp1 == dp2);

             ND::TChannelId c1 = d1->GetChannelId();
             ND::TChannelId c3 = (*it)->GetChannelId();
             ensure_equals("Channels are the same",c1.AsUInt(),c3.AsUInt());
         }
    }

    // Test the TDigitProxy objects created with the two different
    // constructors are the same.
    template <> template <>
    void testTDigit::test<10> () {
         ND::TND280Event event;
         ND::THandle<ND::TDigitContainer> digits = event.GetDigits("test");
         ensure("Digit container was created",digits);

         std::vector<ND::TDigitProxy> offsetProxies;
         std::vector<ND::TDigitProxy> iteratorProxies;
         int digitOffset = 0;
         for (ND::TDigitContainer::iterator d = digits->begin();
              d != digits->end(); ++d) {
             iteratorProxies.push_back(ND::TDigitProxy(*digits, d));
             offsetProxies.push_back(ND::TDigitProxy(*digits, digitOffset++));
         }

         ensure_equals("Same number of offset and iterator proxies",
                       offsetProxies.size(), iteratorProxies.size());
         
         for (std::vector<ND::TDigitProxy>::size_type i=0; 
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
        ND::TDigitProxy proxy;
        proxy.SetProxyType(proxy.ConvertName("test"));
        proxy.SetProxyOffset(0x1FFFE);
        ensure("Proxy with offset 0x1FFFE is valid", proxy.IsValid());

        proxy.SetProxyOffset(0x1FFFF);
        ensure("Proxy with offset 0x1FFFF is invalid", !proxy.IsValid());
    }        
};

