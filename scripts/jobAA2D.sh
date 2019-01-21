#!/bin/bash

#for local test
#export SGE_TASK_ID=1
#cd work

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

#qsub has no SGE_TASK_ID = 0
let angle=SGE_TASK_ID-1
emin=1.7
emax=4.4
nsteps=27
step=`echo "scale=1;($emax-$emin)/$nsteps" | bc -l`
let nsteps+=1
ene=$emin
for i in `seq 1 $nsteps`;
do
	echo $i $ene
	sed "s|%angle|${angle}|g;s|%ene|$ene|g" ${PROG_DIR}/scripts/DOMAA2D.mac > DOMAA-${angle}deg-${ene}eV.mac
	${PROG_DIR}/OMGsim DOMAA-${angle}deg-${ene}eV.mac AA_DOM_${angle}deg-${ene}eV.root
	mv AA_DOM_${angle}deg-${ene}eV.root ${PROG_DIR}/prod
	ene=`echo "$ene+$step" | bc -l`
done

cd ../
rm -r ${TEMPDIR}
