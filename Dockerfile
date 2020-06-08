 FROM docker.km3net.de/base/omgsim:4.10.01
  MAINTAINER Vladimir Kulikovskiy <vkulikovskiy@km3net.de>
 
 RUN mkdir -p /builds/simulation/
 ADD . /builds/simulation/omgsim

 RUN yum -y install zsh
 SHELL ["/bin/zsh", "-c"]
 RUN echo $'\
CWD=$(pwd)\n\
cd /builds/simulation/omgsim && source ./setenv.sh\n\
cd $CWD' \
    >> /etc/profile.d/omgsim_env.sh
