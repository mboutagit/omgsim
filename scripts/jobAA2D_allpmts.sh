#!/bin/bash

#for local test
#export SGE_TASK_ID=1
#cd work

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

#qsub has no SGE_TASK_ID = 0
#PMT1 theta 0.9824 phi -1.5708

#let theta=SGE_TASK_ID-1
theta=`echo "$pmttheta/3.14159265*180+$SGE_TASK_ID-1" | bc -l`
phi=`echo "$pmtphi" | bc -l`
emin=1.7
emax=4.4
nsteps=27
step=`echo "scale=3;($emax-$emin)/$nsteps" | bc -l`
let nsteps+=1
ene=$emin
for i in `seq 1 $nsteps`;
do
	sed "s|%theta|${theta}|g;s|%ene|$ene|g;s|%phi|$phi|g" ${PROG_DIR}/scripts/vis-AA2D_allpmts.sh > vis-AA2D_allpmts-${theta}deg-${ene}eV.mac
	${PROG_DIR}/OMGsim -fa  vis-AA2D_allpmts-${theta}deg-${ene}eV.mac AAfull_DOM_${theta}deg-${ene}eV.root
	mv AAfull_DOM_${theta}deg-${ene}eV.root ${PROG_DIR}/prod
  ene=`echo "$ene+$step" | bc -l`
done

cd ../
#rm -r ${TEMPDIR}
