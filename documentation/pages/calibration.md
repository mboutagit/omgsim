##Calibration

##QE calibration
The most direct way to measure QE is with Direct Coupling to the PMT photocathode (DC). 
In this case every escaped electron participates in the signal.
The similar way is to measure the current from the first dynode. ECAP group (O. Kalekin) reports negligible difference (99% vs 100%) for the two method comparison for the 3-inch PMTs. The illumination of the PMT should be done to the central part with rather small beam size (~1 mm) to minimise reflection and other effects for the inclinated beams falling on the spherical PMT surface.

Alternative is the measure of the QE from the amplified anode signal. In this case the collection efficiency (CE) is also included in the measurement. To calculate the QE some collection efficiency should be assumed (90% is suggested for big PMTs).

In case of ANTARES PMTs the anode signal was used (QE+CE). In case of KM3NeT PMTs the DC is used (QE).

Starting from v3.0 the simulation uses complex refractive index (N=n+ik). Known n, k for every wavelength and the photocathode thickness allows absorption, transmission and reflection calculation for every wavelength and the inclination angle. The "k" index can be deduced from QE measurement if the photocathode thickness is known. 

The real thickness is not provided officially by Hamamatsu. Possibilities:
	30 nm 3-inch, 26 nm >= 8-inch (gaven to Oleg by unoficial discussion with Hamamatsu)
	20 nm best fit of Motta et al.
	26 nm typical of M.D.Lay meas. For 8-inch? (to verify with Christophe)

It was also verified that for every photocathode thickness (20,26,30) the measured average QE for 3-inch KM3NeT PMTs can be tuned. For the following it was decided to use 30 nm thickness for 3 inch PMTs.

To run the calibration, compile OMGsim with ENABLE_TESTING:
cd build
cmake .. -DCMAKE_CXX_COMPILER=g++-510 -DCMAKE_C_COMPILER=gcc-510 -DENABLE_TESTING
make

Run QuantumEfficiencyTester. Without options, it reads curring K-index in the data, simulates QE with decired precision (5% is the default) and compares it with the measured one. The precision comes from the electron production probability, which is a random Brownian process (this probability depends on the photocathod thickness and light penetration probability which is the function of k-index and wavelength, so it needs to be simulated with many electrons for every k-index). Precision can be changed with "-precision option". To calibrate the K-index, "-cal" option should be used. After several itarations (untill ratio of simulated QE to the measured stays inside the requested precision for every wavelength) the K-index table is printed. The following command was used for 3-inch KM3NeT calibration with 1% precision:

./QuantumEfficiencyTester -precision 0.01 -cal

###
