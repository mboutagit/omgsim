# What is LACTELSim package?

LACTELSim is package dedicated to simulate LAC-TEL cosmic ray telescope mainly with single muons. The package is built on top of [OMGSim](https://github.com/vkulikovskiy/OMGsim). 
Please, be awair of some early stage modification in OMGSim package with respect to the version in official repository. This changes were made to test and align the needs to LAC-TEL objectifs and constraints.

## Author(s)

- [Mohammed Bouta](mailto:bouta@cppm.in2p3.fr?subject=LACTELSim%20package)

## Documentation

The documentation for the whole package is still under developement, but the user can generate one using doxygen tool.

### The Package

This package might be divided in several categories of classes based on the task of each calss. In the first place, it comes the detector management class,  -BLactelDetector- 

# How it work?

The main program is OMGSim under programs/OMGSim.cxx. The compiled version has many options and it takes one macro file from which the Geant4 commands are invoked as well as the custum command under (`KM3/` and `LACTEL/` directories).
sample example on linux: `OMGSim -fa macro_file.mac` 

## OMGSim program Options

It is important to notice that OMGSim was initialy used for KM3NeT OMs and DOMs simulation and it is used in the simualtion chain of the KM3NeT neutrino telescope. It provides a detailed modeling of these optical modules and applies experimental properties to them. It is more convinient to make a LAC-TEL dedicated program simular to OMGSim based on LACTELSim package (this is foreseen). 

### Option available

 The options are based on argument list reading and mapping through a dedicated keywords that define the parameters. Each parameter is precedented by a predefined key starting with `-` or `--`

- `-d, --debug`: This option allows to use debug mode. A value different from 0 enables debug mode (default value is false).
- `-s, --session`: Activate session mode.
- `-ka, --keep-all`, Allows to keep track of all events steps in output file. This become obsolet with LACTELSim
- `-fa, full-analysis`: Perform full analysis (Obsolet here).
- `-evt`: output `.evt` file containi hits details.
- `-evtf`: specify the output file name.
- `-r, --run`: specify the run number for Geant4.
- `-dl`: debug-level, specify the debug level for the Logger.
