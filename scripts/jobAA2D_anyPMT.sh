#!/bin/bash

#for local test
export SGE_TASK_ID=1
cd work

if [[ -z $G4ROOT ]]
then
    export G4VERSION=4.10.0;for i in /sps/km3net/users/hugon/developpement/env/*.sh; do source $i; done
fi

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

#qsub has no SGE_TASK_ID = 0
#PMT1 theta 0.9824 phi -1.5708

#let theta=SGE_TASK_ID-1
theta=`echo "$pmttheta/3.14159265*180+$SGE_TASK_ID-1" | bc -l`
phi=`echo "$pmtphi" | bc -l`
emin=2.035
emax=4.135
nsteps=21
step=`echo "scale=3;($emax-$emin)/$nsteps" | bc -l`
let nsteps+=1
ene=$emin
for i in `seq 1 $nsteps`;
do
	sed "s|%theta|${theta}|g;s|%ene|$ene|g;s|%phi|$phi|g" ${PROG_DIR}/scripts/vis-AA2D.mac > vis-AA-${theta}deg-${ene}eV.mac
	${PROG_DIR}/OMGsim -s vis-AA-${theta}deg-${ene}eV.mac AA_ANTARES_${theta}deg-${ene}eV.root
	mv AA_ANTARES_${theta}deg-${ene}eV.root ${PROG_DIR}/prod
  ene=`echo "$ene+$step" | bc -l`
done

cd ../
#rm -r ${TEMPDIR}
