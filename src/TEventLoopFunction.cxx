#include "TND280Log.hxx"
#include "TEventLoopFunction.hxx"

CP::TEventLoopFunction::TEventLoopFunction() {}

CP::TEventLoopFunction::~TEventLoopFunction() {}

bool CP::TEventLoopFunction::operator () (TND280Event& event) {
    ND280Warn("Event loop without processing operator " 
              << event.GetName());
    return true;
}

int CP::TEventLoopFunction::Process(TND280Event& event, int outputFiles) {
    if (operator()(event)) return 0;
    return -1;
}

void CP::TEventLoopFunction::Initialize(void) {}

void CP::TEventLoopFunction::BeginFile(TVInputFile *const) {}

void CP::TEventLoopFunction::EndFile(TVInputFile *const) {}

void CP::TEventLoopFunction::Finalize(TND280Output& value) {}

void CP::TEventLoopFunction::Finalize(TND280Output * const value) {
    // The user program didn't define a Finalize method.  Check to see if
    // there is a valid output file, and if there is, try the old (bogus) form
    // of Finalize in case the program hasn't been updated. 
    if (value) Finalize(*value);
}

void CP::TEventLoopFunction::Usage(void) {}

bool CP::TEventLoopFunction::SetOption(std::string option,
                                            std::string value) {
    ND280Warn("Empty set option called with " << option << "=" << value);
    return false;
}
