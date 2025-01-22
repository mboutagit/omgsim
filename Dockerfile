 FROM git.km3net.de:4567/simulation/omgsim/base_container
  MAINTAINER Vladimir Kulikovskiy <vkulikovskiy@km3net.de>
 
 RUN yum install -y ca-certificates && yum clean -y all
 RUN mkdir -p /builds/simulation/
 COPY . /builds/simulation/omgsim
 RUN cd /builds/simulation/omgsim && rm -rf build && mkdir build && cd build &&  cmake .. && make

 RUN yum -y install zsh
 SHELL ["/bin/zsh", "-c"]

 ENV PATH /builds/simulation/omgsim/build:${PATH}
