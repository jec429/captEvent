#include <iostream>
#include <sstream>
#include <limits>
#include <memory>
#include <unistd.h>
#include <csignal>
#include <string>

#include <TROOT.h>

#include <eventLoop.hxx>

class TTestSplit: public CP::TEventLoopFunction {
public:
    TTestSplit() {
        fCount = 0;
    }

    virtual ~TTestSplit() {};

    virtual int Process(CP::TEvent& event, int outputFiles) {
        int temp = fCount;
        if (outputFiles>1) fCount = (fCount+1) % outputFiles;
        return temp;
    }

private:
    int fCount;
};

int main(int argc, char **argv) {
    TTestSplit userCode;
    CP::eventLoop(argc,argv,userCode);
}


