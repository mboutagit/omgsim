OMGsim documentation {#mainpage}
----------------------------------

##Introduction

OMGsim software ojective is to provide an easy to use simulation of the neutrino telescopes in general, ANTARES, NEMO and KM3NeT in particular. An important attention as been given to the simulation of the photocathode, using a dedicated thin layer and complex index simulation.

##Features
This simulation will allow you to:

- easily determine a precise geometry of the detector
- easily determine a pricise geometry of the phototubes and optical modules
- easily determine the different materials and properties of the detector and phototubes
- use the exact Petzold water scattering of light (with the base parameters measured for ANTARES)
- Test any experiment you would like to reproduce
     - Laboratory angular acceptance
     - 40K decay in water
     - Muon Cherenkov effect in the DOM
     - Test of the DOM is a water tank
     -...
     
##Description
- KM3NeT DOM Simulation [here](md_documentation_pages_DOM.html)
- Calibration [here](md_documentation_pages_calibration.html)
- Version changes [here](md_documentation_pages_changes.html)
- Visualization [here] (md_documentation_pages_visualization.html)


##Installation

###warning
This tool use some c++11 great features as auto declaration and in-header attribute initialization. It is strongly advised to have a newer than 4.6 gcc version (even more recent advised).

###Dependencies
The dependencies are already satisfied at the CC de Lyon. So, except if you strongly want to install it on your own computer, goes directly to the how to at Lyon.

- GCC >= 4.7
- cmake >= 3.2
- ROOT > 5.32
- libconfig
- GEANT4 > 10
- Doxygen > 1.8

###Download and compilation
###Warning
It is never easy to compile this stuff, we met some unuderstood ld problem under Ubuntu, so we advice you to compile it at lyon

###Lyon prerequises
The environment was prepared at Lyon to allow an easy installation. You just to source a couple of files:

    for i in /sps/km3net/users/hugon/sw/env/*sh; do source $i; done

This tool can be obtained via svn, using the command

    (Outdated) svn co --username km3net --password Capopassero http://svnserver.ge.infn.it/KM3Sim
    svn co http://svn.km3net.de/simulation/OMGsim/trunk/    (std login km3net and pwd are available)
    cd trunk

then to compile it

    mkdir build
    cd build
    cmake .. -DCMAKE_CXX_COMPILER=g++-510 -DCMAKE_C_COMPILER=gcc-510
    make

Then the binaries from the compilation will be in the folder "build".
The "programs" folder (might) contains the typical use of the tool, the test one contains some experimental programs.

##Quick start
A quick start howto is given [here](md_documentation_pages_quickstart.html)

##Documentation
###Build the documentation
The doxygen documentation is provided in the folder documentation.
It is easier to use it at your local machine.

Compile with:

    cd [your checkout path]/trunk
    doxygen documentation/config.dox
    cd documentation/latex
    make

Open (on linux):

    firefox documentation/html/index.html
    gnome-open latex/refman.pdf

Open (on Mac):

    open documentation/html/index.html 
    open latex/refman.pdf

Note that the Doxygen needs to be installed. 

On Mac type:

    brew install doxygen  

###Complete the documentation
To complete the documentation you have to observe some doxygen rules while you are comenting the .h code described [here](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html).

The easiest way could be to use the [doxymacs](https://github.com/emacsattic/doxymacs) script for emacs.
