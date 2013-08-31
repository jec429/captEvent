
#include "TReconNode.hxx"

ClassImp(CP::TReconNode);

CP::TReconNode::TReconNode()
    : fQuality(0.) { }

CP::TReconNode::~TReconNode() {}

void CP::TReconNode::ls(Option_t *opt) const {
    std::string option(opt);

    TROOT::IndentLevel();
    std::cout << ClassName() << "(" << this << ")::" << std::endl;
    TROOT::IncreaseDirLevel();
    TROOT::IndentLevel();
    std::cout << "Quality: " << GetQuality() << std::endl;
    if (fState) {
        fState->ls(opt);
    }
    else {
        TROOT::IndentLevel();
        std::cout << "State Information is missing" << std::endl;
    }
    if (fObject) {
        if (option.find("dump") != std::string::npos) {
            fObject->ls(opt);
        }
        else if (option.find("recon") != std::string::npos) {
            // Dump the top level object, but not any children.
            fObject->ls("");
        }
        else {
            TROOT::IndentLevel();
            std::cout << "Object Information not shown" << std::endl;
        }
    }
    else {
        TROOT::IndentLevel();
        std::cout << "Object Information is missing" << std::endl;
    }
    TROOT::DecreaseDirLevel();
}

ClassImp(CP::TReconNodeContainer);

CP::TReconNodeContainer::TReconNodeContainer() {}

CP::TReconNodeContainer::~TReconNodeContainer() {}

void CP::TReconNodeContainer::ls(Option_t *opt) const {
    TROOT::IndentLevel();
    std::cout << ClassName() << "(" << this << ")::" << std::endl;
    TROOT::IncreaseDirLevel();
    for (CP::TReconNodeContainer::const_iterator n = begin();
         n != end();
         ++n) {
        (*n)->ls(opt);
    }
    TROOT::DecreaseDirLevel();
}

