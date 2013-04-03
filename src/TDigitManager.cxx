#include <algorithm>

#include "TEvent.hxx"
#include "TEventFolder.hxx"
#include "TOADatabase.hxx"
#include "TDigitManager.hxx"
#include "TND280Log.hxx"

CP::TDigitManager::TDigitManager() 
    : fPersistentDigits(false) {}

CP::TDigitManager::~TDigitManager() {}

void CP::TDigitManager::RegisterFactory(CP::TDigitFactory* factory) {
    std::string name = factory->GetName();
    if (fFactories.find(name) != fFactories.end()) {
        throw CP::EMultipleDigitFact();
    }
    fFactories[name] = factory;
}

bool CP::TDigitManager::FactoryAvailable(std::string type) const {
    return (fFactories.find(type) != fFactories.end());
}

CP::THandle<CP::TDigitContainer> 
CP::TDigitManager::CacheDigits(std::string type) {
    // Check to see if the digits already exist in the current event.
    CP::TEvent* event = TEventFolder::GetCurrentEvent();
    if (!event) {
        throw EDigitEventMissing();
    }
    return CacheDigits(*event,type);
}

CP::THandle<CP::TDigitContainer> 
CP::TDigitManager::CacheDigits(CP::TEvent& event,
                               std::string type) {

    CP::THandle<CP::TDigitContainer> digits = 
        event.Get<CP::TDigitContainer>("~/digits/"+type);
    if (digits) return digits;

    // The digits don't exist in the event, so try to generate them using the
    // TDigitFactory.
    CP::TDigitFactory* factory = fFactories[type];
    if (!factory) return CP::THandle<CP::TDigitContainer>();
    digits = CP::THandle<CP::TDigitContainer>(factory->MakeDigits());
    if (!digits) {
        ND280Warn("The " << factory->GetName() << " digit factory failed");
        return CP::THandle<CP::TDigitContainer>();
    }
    digits->SetName(type.c_str());

    // Save the digits in the current event.
    CP::THandle<CP::TDataVector> d = event.Get<CP::TDataVector>("~/digits");
    if (!fPersistentDigits) d->AddTemporary(digits);
    else d->AddDatum(digits);

    return digits;
}

CP::THandle<CP::TDigitContainer> 
CP::TDigitManager::FindDigits(const TDigitProxy& proxy) {
    std::string name = TDigitProxy::ConvertType(proxy.GetProxyType());

    // Check to see if the TDigitContainer is already part of the event.
    CP::THandle<CP::TDigitContainer> digits = CacheDigits(name);
    return digits;
}

CP::TDigit* CP::TDigitManager::GetDigit(const TDigitProxy& proxy) {
    std::string name;

    // Check to see if the proxy has already cached the pointer.
    TDigit* pointer = proxy.GetProxyCache();
    if (pointer) return pointer;

    // Find the digits in the event.  This will regenerate the TDigitContainer
    // if necessary (and possible).
    CP::THandle<CP::TDigitContainer> digits = FindDigits(proxy);
    if (!digits) throw EDigitNotAvailable();

    // Get the offset of the digit inside of the container and throw an
    // exception if the offset is invalid.
    unsigned int offset = proxy.GetProxyOffset();
    if (!(offset<digits->size())) throw EDigitNotFound();

    pointer = digits->at(offset);

    // Check that the proxy salt matches the container and digit.  Throw an
    // exception if the salt doesn't match.
    if (!proxy.CheckSalt(digits->GetSignature(),pointer))
        throw EDigitMismatch();

    /// Cache the container and digit for future lookup.
    proxy.SetProxyCache(pointer,CP::GetPointer(digits));

    return pointer;
}

CP::TDigitFactory::TDigitFactory(std::string name) 
    : fName(name) {}
    
CP::TDigitFactory::~TDigitFactory() {}

std::string CP::TDigitFactory::GetName() {
    return fName;
}
