#ifdef __CINT__
#pragma link C++ class CP::TG4TrajectoryPoint+;
#pragma read sourceClass="CP::TG4TrajectoryPoint" version="[-4]"        \
     source="TLorentzVector fPosition"                                 \
     targetClass="CP::TG4TrajectoryPoint"                               \
     target="fPositionX"                                                \
     code="{fPositionX = onfile.fPosition.X();}"
#pragma read sourceClass="CP::TG4TrajectoryPoint" version="[-4]"        \
     source="TLorentzVector fPosition"                                 \
     targetClass="CP::TG4TrajectoryPoint"                               \
     target="fPositionY"                                                \
     code="{fPositionY = onfile.fPosition.Y();}"
#pragma read sourceClass="CP::TG4TrajectoryPoint" version="[-4]"        \
     source="TLorentzVector fPosition"                                 \
     targetClass="CP::TG4TrajectoryPoint"  target="fPositionZ"          \
     code="{fPositionZ = onfile.fPosition.Z();}"
#pragma read sourceClass="CP::TG4TrajectoryPoint" version="[-4]"        \
     source="TLorentzVector fPosition"                                 \
     targetClass="CP::TG4TrajectoryPoint"                               \
     target="fPositionT"                                                \
     code="{fPositionT = onfile.fPosition.T();}"
#pragma read sourceClass="CP::TG4TrajectoryPoint" version="[-4]"        \
     source="TVector3 fMomentum"                                       \
     targetClass="CP::TG4TrajectoryPoint" target="fMomentumX"           \
     code="{fMomentumX = onfile.fMomentum.X();}"
#pragma read sourceClass="CP::TG4TrajectoryPoint" version="[-4]"        \
     source="TVector3 fMomentum"                                       \
     targetClass="CP::TG4TrajectoryPoint" target="fMomentumY"           \
     code="{fMomentumY = onfile.fMomentum.Y();}"
#pragma read sourceClass="CP::TG4TrajectoryPoint" version="[-4]"        \
     source="TVector3 fMomentum"                                       \
     targetClass="CP::TG4TrajectoryPoint" target="fMomentumZ"           \
     code="{fMomentumZ = onfile.fMomentum.Z();}"
#pragma link C++ template std::vector<CP::TG4TrajectoryPoint>+;
#endif
