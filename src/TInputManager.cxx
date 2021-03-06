#include "TInputManager.hxx"
#include "TCaptLog.hxx"

CP::TVInputBuilder::TVInputBuilder(const char* name, const char* doc) 
    : fName(name), fDocumentation(doc) {}

CP::TVInputBuilder::~TVInputBuilder() {}

CP::TVInputFile* CP::TVInputBuilder::Open(const char* fileName) const {
    return NULL;
}

CP::TInputManager::TInputManager() {}

CP::TInputManager::~TInputManager() {}

void CP::TInputManager::Register(TVInputBuilder* builder) {
    fBuilders.push_back(builder);
}

const CP::TVInputBuilder& CP::TInputManager::Builder(const char* name) {
    std::string optionString(name);
    std::string nameString = optionString.substr(0,optionString.find("("));
    for (std::vector<const CP::TVInputBuilder*>::iterator b = fBuilders.begin();
         b != fBuilders.end(); ++b) {
        if ((*b)->GetName() != nameString) {
            continue;
        }
        const TVInputBuilder& builder = *(*b);
        builder.SetArguments(optionString.c_str());
        return builder;
    }
    CaptError("Builder not found for " << nameString);
    throw ENoInputBuilder();
}

const CP::TVInputBuilder& CP::TInputManager::Builder(int i) {
    return *(fBuilders[i]);
}

int CP::TInputManager::BuilderCount() const {
    return fBuilders.size();
}
