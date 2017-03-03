#!/bin/bash

location=${0%/*}

ebrowse --output-file=$location/BROWSE */src/*.cc */include/*.hh */programs/*.cxx $G4ROOT/include/Geant4/*hh /home/chugon/Downloads/geant4.10.01/source/*/src/*.cc /home/chugon/Downloads/geant4.10.01/source/*/*/src/*.cc /home/chugon/Downloads/geant4.10.01/source/*/*/*/src/*.cc /home/chugon/Downloads/geant4.10.01/source/*/*/*/*/*/src/*.cc  /home/chugon/Downloads/geant4.10.01/source/*/*/*/*/src/*.cc
