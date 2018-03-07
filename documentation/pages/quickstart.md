Quick start
-----------

This short tutorial aims to introduce the using of OMGSim with some few examples.


##Make it run! now!

The base installation contains basics script and material/OM/PMT/detector descriptions and can be run immediatly.
The binary is OMGsim:

    ./OMGsim -s scripts/vis-AA2D_local.mac [outputfile.root]

The -s is to open a GEANT4 session, unavoidable to draw the simulation (to be specified with your favorite way in the .mac file). 

Also you can use "-fa" option is to get a "full analysis", just meaning that the output file will contain the full detected events (vertex, energy, impact point...) instead of the simplified events summary only (typicaly number of detected events).

##Custumize it a bit


###The detector data files


The [vis-xpTest.mac](vis-xpTest_8mac_source.html) contains the basics to do a dumb K40 test with a DOM inside a box. The mac file additionnal options are developed here(to write), but for now, the important element is the /KM3/det/DetectorTypeName value.
It reffers to the [tester detector](KM3DetTester_8dat_source.html) as an example. This example shows how to place passive volumes and a DOM.
The detector data files are placed in common/data, should start with KM3Det, dans finish with .dat.
You can add any predefined volume (see further), DOM and PMT, the nomenclature should be like DOM1 DOM2 etc.

###The Passive Volumes

The passive volumes are determined by basics geant4 solids or boolean operation. In this example boxes are created if [files](KM3VolWaterTankInside_8dat_source.html) starting with KM3Vol and finishing by .dat. The used syntax is JSON.
Those files contain the name, the material and the volume kind. Refer to the [source code documentation](structkm3net_1_1KM3PassiveVolumeMgr_1_1PassiveVolumeParameters.html) to see the possible volume and the parameter names.

###The DOM and OM
To build a DOM or OM, you have a bunch of parameter that allow you to define the size of the components and the materials.
[The parameters to define a DOM are listed here](structkm3net_1_1KM3OpticalModuleMgr_1_1OpticalModuleParameters.html) and should be implemented in JSON as in this [DOM example](KM3OMkm3net_8dat_source.html) and [OM example](KM3OMAntares_8dat_source.html).
In the case of the DOM, the position and number of PMTs is not predifined, and is up to the PMTPositions table.

###The PMT

[The parameters to define a phototube](structkm3net_1_1KM3PhotoTubeMgr_1_1PhotoTubeParameters.html) usage is illustrated in the [3 inches](KM3PMT3inches_8dat_source.html) and [10 inches](KM3PMT10inches_8dat_source.html) PMT files.
![Caption text](../images/pmpar.svg "Image title")

###The Detector
Detector files are stored as KM3Det<DET_NAME>.dat and they can be composed by several passive volumes described as:

    Volume1:                           //incrementing index
    {
            Name = "WLSSphere";        //Name inside its KM3Vol...dat files
            ID = 1;                    //incrementing index of the passive volume of this type
            X = 0.;
            Y = 0.;
            Z = 0.;
            Theta = 0;
            Phi = 0.;
    };

And (or only) optical modules which are described as:

    OM1:                               //incrementing index
    {
            Name = "KM3DOMTest";       //Name inside its KM3OM...dat file
            ID = 1;
            X = 0.;
            Y = 0.;
            Z = 0.;
            Theta = 3.14159265358979312;
            Phi = 0.;
            MotherVolume="WLSSphere"   //Optional parameter - you can specify MotherVolume if you want to put OM inside the passive volume
    };


###The Materials

Due to the "tabular like" structure of the material parameters an different configuration file structure is used.
The first element is the

    CREATE [name]
    [...]
    CREATE

Inside this the components flags, the density and components list are defined

    DENSITY [value in g/cm3]

    COMPONENTS
    [G4_component] relative density
    [...]
    COMPONENTS

Then the list of properties can be given if the form

    PROPERTY [name]
    [energy 1] [value 1]
    [energy 2] [value 2]
    ...

[The material of the photocathode](KM3MatPMT3inches_8dat_source.html) is a good example.
