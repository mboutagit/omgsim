##Wavelength shifter to improve OM sensitivity


##Geant4 process
The process class name is G4OpWLS. From Geant4 documentation http://geant4.cern.ch/G4UsersDocuments/UsersGuides/ForApplicationDeveloper/html/TrackingAndPhysics/physicsProcess.html :

A WLS material is characterized by its photon absorption and photon emission spectrum and by a possible time delay between the absorption and re-emission of the photon. 
Wavelength Shifting may be simulated by specifying these empirical parameters for each WLS material in the simulation. 

It is sufficient to specify in the user's DetectorConstruction class a relative spectral distribution as a function of photon energy for the WLS material. 

- WLSABSLENGTH is the absorption length of the material as a function of the photon's momentum. 
- WLSCOMPONENT is the relative emission spectrum of the material as a function of the photon's momentum, and 
- WLSTIMECONSTANT accounts for any time delay which may occur between absorption and re-emission of the photon. An example is shown in source listing 5.2.8.

##Shifter data
The shifter is realised from p-terphenyl material. 
The data can be found in following open-access articles:

- https://www.sciencedirect.com/science/article/pii/S016890021102239X (broader measurement range)
- https://www.sciencedirect.com/science/article/pii/S0168900217307039

The distributions are stored in common/data/KM3MatWLSTest.dat They are rather correct although the absorption can be a bit extended, in principle.

The conversion to absorption length is following:

    Abs_length = 1./shifter_density/molar_extinction_coeff*cm;  

shifter_density should be in mol/liter and molar extinction coefficient in mol^−1*liter*cm^−1.

Article S0168900217307039 says density is 1.23 g/cm^3 and molecular weight of 230.30376 g/mol, so rho= 1.23[g/cm^3]/230[g/mol]=1230[g/liter]/230[g/mol]=5.4
Article S0168900217307039 says that absorption peak at 270 nm is 33800 mol-1 later cm-1, so units in our tables are these.

In fact, for the peak at 276 nm the absorption length is ~0.05mum. And article S0168900217307039 says that the absorption is starting to saturate at a material load of 50 mug/cm^2 = 40*10-6 cm = 0.4 mum, so about 10 absorption lengths for peak (less for valley) which is reasonable.

The optimal WLS thickness should be about 100-300 mug/cm2 = 0.8mum-2.4mum (article S0168900217307039).

The sphere of radius 215.901 mm which is 215.9mm (Nautilus sphere radus) + 1mum is realized in common/data/KM3VolWLSSphere.dat .
The DOM is placed in this spere in the detector files common/data/KM3DetDOMWLS.dat and common/data/KM3DetDOMWLSTest.dat (1 PMT only for AA scans).

Using it KM3NeT DOM start to have effective area below 300 nm. The area seems to be stable which is clear since the reemision has a constant spectrum and absorption is at saturation. It keeps stable up to 100nm in the simulation but it is, probably, not realistic. 

At some point absorption should drop (happens around 150 nm) WLS stop to work (so, we could fit the left absorption peak with Gaussian and use for extrapolation below 200nm).

Note, that article S016890021102239X suggest that a fluorescence quantum yield of p-terphenyl is as high as 0.9. In simulation instead it is considered 1 (all absorbed photons are converted to reemitted). So, resulting area below 300 nm should be scaled to 0.9, probably. Simulation change would involve G4OpWLS update.

Note, that since Cherenkov light grows with E^4 or Lambda^-4 the flat Angular Acceptance below 300 nm can give some significant increase in the signal.
