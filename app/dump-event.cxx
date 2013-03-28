#include <TROOT.h>

#include <nd280EventLoop.hxx>

class TDumpEvent: public ND::TND280EventLoopFunction {
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

    bool operator () (ND::TND280Event& event) {
        if (ND::TND280Log::GetLogLevel()>ND::TND280Log::QuietLevel && !fQuiet) {
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
    // using ND::TND280EventLoopFunction::Finalize;
    void Finalize(ND::TND280Output*const output) {
    }

private:
    std::string fLSOption;
    bool fQuiet;
    bool fPrintList;
};

int main(int argc, char **argv) {
    TDumpEvent userCode;
    ND::nd280EventLoop(argc,argv,userCode,1);
}

