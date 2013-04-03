#include "TND280Log.hxx"
#include "TEventLoopFunction.hxx"

CP::TEventLoopFunction::TEventLoopFunction() {}

CP::TEventLoopFunction::~TEventLoopFunction() {}

bool CP::TEventLoopFunction::operator () (TEvent& event) {
    ND280Warn("Event loop without processing operator " 
              << event.GetName());
    return true;
}

int CP::TEventLoopFunction::Process(TEvent& event, int outputFiles) {
    if (operator()(event)) return 0;
    return -1;
}

void CP::TEventLoopFunction::Initialize(void) {}

void CP::TEventLoopFunction::BeginFile(TVInputFile *const) {}

void CP::TEventLoopFunction::EndFile(TVInputFile *const) {}

void CP::TEventLoopFunction::Finalize(TRootOutput * const value) {}

void CP::TEventLoopFunction::Usage(void) {}

bool CP::TEventLoopFunction::SetOption(std::string option,
                                            std::string value) {
    ND280Warn("Empty set option called with " << option << "=" << value);
    return false;
}
