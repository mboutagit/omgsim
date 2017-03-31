##Electron free path in photocathode


###Overview description

For semitransparent photocathode, such as in phototube, the photon is mostly absorbed by photoelectric effect (considered at 100% in this simulation). The electron gets enough energy to be transfered from the valence band to the conductive band. As described in [Photomultiplier Tubes Principles and Applications, by Photonis](http://www2.pv.infn.it/~debari/doc/Flyckt_Marmonier.pdf) the energy of the freed electron is at the electronvolt scale.

The phototube will give a signal only in the case in which the electron escape the photocathode on the vacuum side. Then it will be accelerated and eventually reach the first dynods to be multiplied. Three different approaches can be used, to the simpler to the most complete description:

1.  A constant probability. Indeed, as described in , the mean for an electron to escape the photocathode to the right side for an normal incident photon is 0.4.
2.  As described in [Lushan, G. J. of Elect. (1986) 3: 304](http://rd.springer.com/article/10.1007/BF02778857) you can assume that the electron direction is linear to the photon one, and consider an absorption length. In this case you can use the photon interaction point in the photocathode to calculate an escaping probability. This approach is more complete because it take in account the big angle effect, as described later.
3.  A new consideration is to assume that the electron is not linear to the photon and do not propagate straight at low energy (eV) in a somewhat dense medium (the photocathode), but that it has immediately a Brownian motion.

The three approaches are available in OMGSim and are configurable through the mac file's parameter

    /PMTOpticalModel/ElectrProdlevel i

with i = [0,2] for the three descriptions above.

###Absorption distance to the glass of the photon

The two most refined methods needs an interaction point of the photon in the photocathode. The calculation is done thanks to a random calculation for each absorbed event.
To do so we need to calculate the photon absorption length in the photocathode (that depends on the angle to the normal and the wavelength) to be able to get the absorption point.

To do so we dispose of three information provided by the simulation and input data: the absorption probability in the photocathode (\f$P_{abs}\f$), the photon direction angle to the normal right before the absorption (\f$\theta\f$) and the photocathode thickness (\f$l_{thick}\f$). So the absorption length can be determined by:

\f[  
\lambda_{abs} = \frac {l_{thick}/\sin (\theta)}{P_{abs}}
\f]

Then the next step is to directly calculate a random absorption point following the exponential probability law:

\f[
P_{abs}(x_0)=\exp^{-x_0/\lambda_{abs}}
\f]

The next step is to get the interaction length \f$l_{int}\f$ from the random value \f$rand\f$:
 \f[
 l_{int}=P^{-1}_{abs}(rand),\, rand\in]0,1]
 \f]

Finally the interaction distance \f$ D_{glass} \f$ can be expressed as a projection to the normal:

\f[
D_{glass}=l_{int} . \sin (\theta)
\f]


###The linear absorption approach, "ElectrProdlevel 1"


The first step is to determine an electron production point in the photocathode.













