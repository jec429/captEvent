#include <memory>
#include <iostream>

#include <nd280EventLoop.hxx>

/// User code for the event loop.  This can be copied and used as a skeleton
/// for generic user loop programs.
class TUserLoop: public CP::TND280EventLoopFunction {
public:
    /// Initialize any class specific variables, but most of the work can be
    /// done in Initialize.  Don't create histograms here!
    TUserLoop() {}
    virtual ~TUserLoop() {};

    /// Print a usage message.  This is generally called when there is a
    /// command line input error.
    void Usage(void) {
    }

    /// Set an option and return true if it is valid.  This is called by the
    /// event loop command line argument processing code for each "-O
    /// [name]=[value]" option found on the command line.  If the command line
    /// has "-O [name]" without a value, then the value string will be equal
    /// to "".  This must return false if the option was not correctly
    /// processed.
    virtual bool SetOption(std::string option,std::string value="") {
        return false;
    }

    /// Called for each event inside the event loop, and returns true if the
    /// event should be saved to the output file.  If the remainder of the
    /// current file should be skipped, this should through the
    /// ENextEventLoopFile exception.  If the daughter class defines this
    /// operator, it should not define the Process() method.
    bool operator () (CP::TND280Event& event) {
        return true;
    }

    /// Called after the arguments are processes by before reading the first
    /// event.  The output file is open so any histograms will be added to the
    /// output file.
    virtual void Initialize(void) {
    }

    /// Called after reading the last event.  The output file is still open,
    /// so you can add extra information.  Because of an ideosyncracy in the
    /// way root handles histograms, objects created in Initialize() will
    /// already be stored in the output file.
    virtual void Finalize(CP::TND280Output* output) {
    }

private:

};

int main(int argc, char **argv) {
    TUserLoop userLoop;
    nd280EventLoop(argc,argv,userLoop);
}
