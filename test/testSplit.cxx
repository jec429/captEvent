#include <iostream>
#include <sstream>
#include <limits>
#include <memory>
#include <unistd.h>
#include <csignal>
#include <string>

#include <TROOT.h>

#include <nd280EventLoop.hxx>

class TTestSplit: public CP::TND280EventLoopFunction {
public:
    TTestSplit() {
        fCount = 0;
    }

    virtual ~TTestSplit() {};

    virtual int Process(CP::TND280Event& event, int outputFiles) {
        int temp = fCount;
        if (outputFiles>1) fCount = (fCount+1) % outputFiles;
        return temp;
    }

private:
    int fCount;
};

int main(int argc, char **argv) {
    TTestSplit userCode;
    nd280EventLoop(argc,argv,userCode);
}


