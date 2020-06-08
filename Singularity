Bootstrap: docker
From: docker.km3net.de/simulation/omgsim:latest

%labels
    Maintainer Vladimir Kulikovskiy <vkulikovskiy@km3net.de>

%environment
    CWD=$(pwd)
    cd /builds/simulation/omgsim && source ./setenv.sh
    cd $CWD
