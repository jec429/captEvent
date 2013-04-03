#ifdef __CINT__
#pragma link C++ class CP::TG4Trajectory+;
#pragma link C++ class CP::TG4TrajectoryContainer+;
#pragma link C++ class CP::THandle<CP::TG4Trajectory>+;
#pragma link C++ class CP::THandle<CP::TG4TrajectoryContainer>+;
#pragma read sourceClass="CP::TG4Trajectory" version="[-3]"        \
     source="TLorentzVector fInitialMomentum"                                 \
     targetClass="CP::TG4Trajectory"                               \
     target="fInitialMomentumX"                                                \
     code="{fInitialMomentumX = onfile.fInitialMomentum.X();}"
#pragma read sourceClass="CP::TG4Trajectory" version="[-3]"        \
     source="TLorentzVector fInitialMomentum"                                 \
     targetClass="CP::TG4Trajectory"                               \
     target="fInitialMomentumY"                                                \
     code="{fInitialMomentumY = onfile.fInitialMomentum.Y();}"
#pragma read sourceClass="CP::TG4Trajectory" version="[-3]"        \
     source="TLorentzVector fInitialMomentum"                                 \
     targetClass="CP::TG4Trajectory"  target="fInitialMomentumZ"          \
     code="{fInitialMomentumZ = onfile.fInitialMomentum.Z();}"
#pragma read sourceClass="CP::TG4Trajectory" version="[-3]"        \
     source="TLorentzVector fInitialMomentum"                                 \
     targetClass="CP::TG4Trajectory"                               \
     target="fInitialMomentumT"                                                \
     code="{fInitialMomentumT = onfile.fInitialMomentum.T();}"
#endif
