##Electron free path in photocathode
##Overview description
In semitransparent photocathodes, such as in the phototube, the photon is mostly absorbed by photoelectric effect (considered at 100% in this simulation). The electron gets enough energy to be transferred from the valence band to the conductive band. As described in [Photomultiplier Tubes Principles and Applications, by Photonis](http://www2.pv.infn.it/~debari/doc/Flyckt_Marmonier.pdf) the energy of the freed electron is at the electronvolt scale.
The phototube will give a signal only when the electron escapes the photocathode on the vacuum side. Then it will be accelerated and eventually reach the first dynods to be multiplied. Three different approaches can be used, from the simpler to the most complete description. In the most complete description the main effect is due to the photon impact angle. Bigger is this angle, closer to the glass the electron production point is.
1.  A constant probability. Indeed, as described in [NIMA383 (1996) 495-505](https://www.sciencedirect.com/science/article/pii/S0168900296008613?via%3Dihub), the mean probability for an electron to escape the photocathode to the right side for a normal incident photon is 0.4.
2.  As described in [Lushan, G. J. of Elect. (1986) 3: 304](http://rd.springer.com/article/10.1007/BF02778857) one may assume that the electron direction is collinear to the photon one, and consider an absorption length. In this case you can use the photon interaction point in the photocathode to calculate an escaping probability. The emission angle of photoelectric effect is taken in account, this approach is more complete because it takes into account the angle effect, but is still an important approximation for the biggest angles.
3.  A new consideration is to assume that the electron is not linear to the photon and does not propagate straight at low energy (eV) in a somewhat dense medium (the photocathode), but that it has immediately a Brownian motion. Still using an approximation calculation it takes in account the drastic effect at the biggest angles.
The three approaches are available in OMGsim and are configurable through the mac file's parameter
    /PMTOpticalModel/ElectrProdlevel i
with i = [0,3] for the three descriptions below.
The last part will be about trying to resolve the exact description, and at the very end the Taylor approach will justify the approximations.
##Absorption distance to the glass of the photon
The refined methods need an interaction point of the photon in the photocathode. The calculation is done thanks to a random calculation for each absorbed event.
To do so we need to calculate the photon absorption length in the photocathode (that depends on the angle to the normal and the wavelength) to be able to get the absorption point.
To do so three information sets are provided by the simulation and input data: the absorption probability in the photocathode (\f$P_{abs}\f$), the photon direction angle to the normal right before the absorption (\f$\theta\f$) and the photocathode thickness (\f$l_{thick}\f$). So the absorption length can be determined by:
\f[
\lambda_{abs} = \frac {l_{thick}/\cos (\theta)}{P_{abs}}
\f]
Then the next step is to directly calculate a random absorption point following the exponential probability law:
\f[
P_{abs}(x_0)=\exp^{-x_0/\lambda_{abs}}
\f]
The next step is to get the interaction length \f$l_{int}\f$ from the random value \f$rand\f$:
 \f[
 l_{int}=P^{-1}_{abs}(rand),\, rand\in]0,1]
 \f]
Finally, the interaction distance from the glass \f$ D_{glass} \f$ can be expressed as a projection to the normal:
\f[
D_{glass}=l_{int} . \cos (\theta)
\f]
##The Constant approach, "ElectrProdlevel 0"
Here we consider that the probability to escape is just 40% as in [NIMA383 (1996) 495-505](https://www.sciencedirect.com/science/article/pii/S0168900296008613?via%3Dihub). It should be enough for most applications in which the photons have an incident angle close to the normal and in most PMTs in air cases (to be checked again).

##The absorption length approach, "ElectrProdlevel 1"
This approach is directly derived from [Lushan, G. J. of Elect. (1986) 3: 304](http://rd.springer.com/article/10.1007/BF02778857) using the electron production point as described in the previous section. The electron is considered as going to the vacuum direction, as the photon. The probability for the electron to escape from the photocathode can be determined as an exponential law with an effective absorption length \f$\lambda_{eff}\f$
\f[
P_{esc}(D_{vacuum})=\exp^{-D_{vacuum}/\lambda_{eff}}
\f]
where \f$D_{vacuum}=l_{thick}-D_{glass}\f$ is the distance from the production point to the vacuum.

##The linear escaping approach, "ElectrProdlevel 2"
This process is an intermediate consideration between an effective absorption length and the vicinity of the electron production point to the vacuum/glass. This arbitrary linear approximation was initially a proof of concept testing, but ended up to be a good enough approximation for most of in air PMT simulations.
The demonstration and justification can be found in the Taylor series development, at the end of this document.
The first step is to determine an electron production point as explained in the previous section. Then, considering the angular emission of [photoelectric effect symmetry](http://file.scirp.org/Html/2-7500424_6907.htm), you have probabilities that it goes to the glass and that it goes to the vacuum. This probability is defined as
\f[
P_{vacuum}=D_{glass}/D_{vacuum}
\f]
where \f$ D_{glass} \f$ is the distance from the electron production point to the glass, and \f$ D_{vacuum} \f$ is the distance from the electron production point to the vacuum.
Once the direction is determined the probability to effectively escape from the photocathode, defined by the exponential law with an effective absorption length \f$\lambda_{eff}\f$:
\f[
P_{esc}(D_{vacuum})=\exp^{-D_{vacuum}/\lambda_{eff}}
\f]
Beware that, since the process already reduces the initial escaping probability, the effective absorption length \f$\lambda_{eff}\f$ is much longer than in the process "ElectrProdlevel 1". This length should be calibrated to get about 40% for maximum QE of the photomultiplier ([NIMA383 (1996) 495-505](https://www.sciencedirect.com/science/article/pii/S0168900296008613?via%3Dihub))
##The approximated Brownian motion escaping approach, "ElectrProdlevel 3"
This approach considers that the freed electron has a too low energy (~1 eV) to go straight forward. It will immediately lose its energy and will follow a Brownian motion until it reaches the vacuum or the glass. The Brownian probability function can be defined as:
\f[
P(x,t)=\frac {1}{\sqrt{4\pi Dt}}.e^{\frac{-x^2}{4Dt}}
\f]
where \f$D\f$ is the electron mass diffusivity in the photocathode, \f$x\f$ and \f$t\f$ are the distance and the time.
From this probability distribution the simulation calculates a random time that the electron escape with the distances to the glass and the vacuum \f$D_{glass}\f$ and \f$D_{vacuum}\f$. Since they will be independent, we can neglect the normalization factor \f$\frac {1}{\sqrt{4\pi Dt}}\f$. Since we do not mind the time scale, we can calculate \f$T=4Dt\f$ instead of \f$t\f$. In this condition we can consider a reduced probability distribution:
\f[
p(x,T)=e^{-\frac{-x^2}{T}}
\f]
We can now calculate twice \f$T_1\f$, \f$T_2\f$ defined as:
\f[
T_1=p^{-1}(D_{vacuum},rand),\, rand\in]0,l_{thick}]
\f]
and
\f[
T_2=p^{-1}(D_{glass},rand),\, rand\in]0,l_{thick}]
\f]
with 
\f[
p^{-1}(x,rand)=x^2/\ln (rand)
\f]
and \f$l_{thick}\f$ photocathode thickness.
In this approach the probability to escape to closest side increases exponentially with the distance, which has a peculiarly strong effect for the biggest angles, reachable only if the PMTs are in a medium with an index close to the glass one.

In a full Brownian calculation we should not consider only the distance \f$D_{vacuum/glass}\f$, but the sum of all of the possible distances defined by \f$\int^{\pi/2}_0 D/\cos (\theta)d\theta\f$. Therefore this approach considers only the normal probabilities, i.e. using only the shortest distance from the electron production point to the interfaces. This approximation works if we consider that the escaping probability is dominated by angles close to the normal direction and correspond to the first term of the Taylor series described further in this document.
 Even if one can consider that this approximation is good enough, a calculation to evaluate how far we are from the full description is interesting. Some further investigations have been tested yet. The next sections will give some clues to a full descriptive (and reasonably fast) simulation.
##To go further, "ElectrProdlevel NaN"
###Solve the integral
The easiest solution to describe, but hardest to solve is to consider the integration of the probabilities to escape to a side (glass or vacuum). Let's start from the reduced probability density \f$p(x,t)\f$ and a distance to a side D. As said before the probability to escape to the shortest way is defined by \f$p(D,t)\f$. Meanwhile the probability to escape with an angle \f$\theta\f$ to the normal is \f$p(D/ \sin (\theta),t)\f$. Therefore the probability to escape at any angle \f$\theta\in[0,\pi/2[\f$ can be defined as:
\f[
p_{tot}(T)=4\pi\int_0^{\pi/2} e^{\frac{-D^2}{T.\cos^2(\theta)}}d\theta
\f]
Then the total analytical probability can be used in the "ElectrProdlevel 3" process.
If you get a clue to resolve it, go ahead.
###A try with the probability ratio
It's an ongoing calculation, I write it here to do not lose it, and to brainstorm myself but it might lead nowhere.
Let's consider that if \f$p_1(T_1)=4\pi\int_0^{\pi/2} e^{\frac{-D_1^2}{T_2.\sin^2(\theta)}}d\theta\f$ and \f$p_2(T_2)=4\pi\int_0^{\pi/2} e^{\frac{-D_2^2}{T_2.\sin^2(\theta)}}d\theta\f$. It can be shown that if \f$\not p=p_1/p_2>1\f$ then the electron escape to the side 1 (and vice versa). One can write
\f[
\not p = \frac {4\pi\int_0^{\pi/2} e^{\frac{-D_1^2}{T.\cos^2(\theta)}}d\theta}
               {4\pi\int_0^{\pi/2} e^{\frac{-D_2^2}{T.\cos^2(\theta)}}d\theta}
\f]
###The Taylor Series resolving.
The reduced probability of Brownian motion \f$p_{tot}(x,\theta,T)=e^{\frac{-x^2}{T.\cos^2(\theta)}}\f$ can be developed in Taylor’s series at 0 (Maclaurin series):
\f[
p_{McL}(D,\theta,T)=e^{-\frac{D^2}{T}}.(1-\frac{\theta^2 D^2}{T}+\frac{1D^2\theta^4}{2T^2}-\frac{2\theta^4D^2}{3T}[...])+O(\theta)
\f]
This function diverges from \f$\theta=1\f$, so the maximum considered value for integration will be 1, angle at which \f$p_{tot}\approx0\f$. This probability density can be integrated:
\f[
\int^{1}_0 p_{McL}(x,\theta,T)=\left[e^{-\frac{D^2}{T}} \left(1-\frac{14 D^2 T+3 D^4}{30 T^2}\right) [...]\right ]+\int^1_0 O(\theta)
\f]
This series first term is the used term for the case "ElectrProdlevel 3".
For "ElectrProdlevel 2" instead we use the local tendency (aka derivative) of "ElectrProdlevel 3" to get variations close to the means for normal incident photons:
\f[
T'=\frac{\delta}{\delta r \delta D}p^{-1}(x,r)=\frac{\delta}{\delta r \delta D}x^2/\ln (r) = 2D.r,\, r\in]0,l_{thick}]
\f]
with r a random value.