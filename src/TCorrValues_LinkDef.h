#ifdef __CINT__
#pragma link C++ class CP::TCorrValues+;
#pragma read sourceClass="CP::TCorrValues" version="[1]"              \
     source="TVectorT<double> fVector"                                  \
     targetClass="CP::TCorrValues" target="fVector"                     \
     code="{fVector=onfile.fVector;}"
#pragma read sourceClass="CP::TCorrValues" version="[1]"                \
     source="TMatrixTSym<double> fMatrix"                               \
     targetClass="CP::TCorrValues" target="fMatrix"                     \
     code="{fMatrix=onfile.fMatrix;}"
#endif

