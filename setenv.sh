#gcc 5 su CentOS7
. /opt/rh/devtoolset-4/enable

#local ROOT installation compiled with gcc 5
. ${HOME}/OMGsim/root/bin/thisroot.sh

#Geant4 installation compiled with gcc 5
cd ${HOME}/OMGsim/geant4.10.01.p03-install/bin/
source geant4.sh
cd -

#libconfig
export LIBCONFIG_ROOT=${HOME}/OMGsim/libconfig-1.7.2
export LD_LIBRARY_PATH=${LIBCONFIG_ROOT}/lib:${LD_LIBRARY_PATH}
export CPLUS_INCLUDE_PATH=${LIBCONFIG_ROOT}/include
