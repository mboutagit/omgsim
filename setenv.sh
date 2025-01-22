if [ -f /opt/geant4.10.01.p03-install/bin/geant4.sh ]; then
    #gcc 5 on CentOS7
    . /opt/rh/devtoolset-7/enable

    #local ROOT installation compiled with gcc 5
    #. ${HOME}/OMGsim/root/bin/thisroot.sh
    . /usr/local/root/bin/thisroot.sh

    cd /opt/geant4.10.01.p03-install/bin/
    source ./geant4.sh
    cd -

    #libconfig
    export LIBCONFIG_ROOT=/opt/libconfig-1.7.2

elif [ -f $KM3NET_THRONG_DIR/share/geant/v10.01.p03/bin/geant4.sh ]; then
    #Geant4
    cd $KM3NET_THRONG_DIR/share/geant/v10.01.p03/bin/
    source geant4.sh
    cd -

    #Libconfig
    export LIBCONFIG_ROOT=/sps/km3net/users/hugon/sw/libconfig-1.5    
    #GCC 5.10
    export GCC_ROOT=/pbs/software/centos-7-x86_64/gcc/5.5.0
    export LD_LIBRARY_PATH=${GCC_ROOT}/lib:${GCC_ROOT}/lib64:${LD_LIBRARY_PATH}
    export PATH=${GCC_ROOT}/bin:${PATH}

    #ROOT
    module load root/6.20.04 cmake/3.11.0

    #only to compile (comment out after make)
    export CC=${GCC_ROOT}/bin/gcc
    export CXX=${GCC_ROOT}/bin/g++
fi


export LD_LIBRARY_PATH=${LIBCONFIG_ROOT}/lib:${LD_LIBRARY_PATH}
export CPLUS_INCLUDE_PATH=${LIBCONFIG_ROOT}/include

export PROG_DIR=`pwd`/build
export PATH=${PROG_DIR}:${PATH}

