#Geant4
cd $KM3NET_THRONG_DIR/share/geant/v10.01.p03/bin/
source geant4.sh
cd -

#Libconfig
export LIBCONFIG_ROOT=/sps/km3net/users/hugon/sw/libconfig-1.5
export LD_LIBRARY_PATH=${LIBCONFIG_ROOT}/lib:${LD_LIBRARY_PATH}
export CPLUS_INCLUDE_PATH=${LIBCONFIG_ROOT}/include

#GCC 5.10
#export GCC_ROOT=/pbs/software/x86_64_el7/gcc/5.5.0
export GCC_ROOT=/pbs/software/centos-7-x86_64/gcc/5.5.0
export LD_LIBRARY_PATH=${GCC_ROOT}/lib:${GCC_ROOT}/lib64:${LD_LIBRARY_PATH}
#export PATH=${GCC_ROOT}/bin:/sps/km3net/users/kulikovs/OMGsim/gcc:${PATH}
export PATH=${GCC_ROOT}/bin:${PATH}

#ROOT
cd /usr/local/root/*5.34.23
source bin/thisroot.sh
cd -

#only to compile (comment out after make)
export CC=${GCC_ROOT}/bin/gcc
export CXX=${GCC_ROOT}/bin/g++
