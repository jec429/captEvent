#include <TROOT.h>

#include <eventLoop.hxx>

class TDumpEvent: public CP::TEventLoopFunction {
public:
    TDumpEvent() {
        fLSOption = "";
        fQuiet = false;
        fPrintList = false;
    }

    virtual ~TDumpEvent() {};

    void Usage(void) {
        std::cout << "    -O quiet    Don't call TObject::ls()"
                  << std::endl;
        std::cout << "    -O list     List events in the file"
                  << std::endl;
        std::cout << "    -O <option> Set the option for TObject::ls()"
                  << std::endl;
    }

    virtual bool SetOption(std::string option,std::string value="") {
        if (value != "") return false;
        if (option == "quiet") fQuiet = true;
        else if (option == "list") {
            fPrintList = true;
        }
        else fLSOption = option;
        return true;
    }

    bool operator () (CP::TEvent& event) {
        if (CP::TND280Log::GetLogLevel()>CP::TND280Log::QuietLevel && !fQuiet) {
            event.ls(fLSOption.c_str());
        }
        if (fPrintList) {
            ND280Log("Event " << event.GetContext());
        }
        return true;
    }

    // Do nothing... This is here to test compiler warnings.  The warning
    // can be prevented by adding
    //
    // using CP::TEventLoopFunction::Finalize;
    void Finalize(CP::TRootOutput*const output) {
    }

private:
    std::string fLSOption;
    bool fQuiet;
    bool fPrintList;
};

int main(int argc, char **argv) {
    TDumpEvent userCode;
    CP::eventLoop(argc,argv,userCode,1);
}

