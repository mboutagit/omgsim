#!/bin/bash

#for local test
#export SGE_TASK_ID=1
#cd work

if [[ -z $G4ROOT ]]
then
    export G4VERSION=4.10.0;for i in /sps/km3net/users/hugon/developpement/env/*.sh; do source $i; done
fi

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

#qsub has no SGE_TASK_ID = 0
let angle=SGE_TASK_ID-1
emin=2.035
emax=4.135
nsteps=21
step=`echo "scale=3;($emax-$emin)/$nsteps" | bc -l`
let nsteps+=1
ene=$emin
for i in `seq 1 $nsteps`;
do
	sed "s|%angle|${angle}|g;s|%ene|$ene|g" ${PROG_DIR}/scripts/vis-AA2D.mac > vis-AA-${angle}deg-${ene}eV.mac
	${PROG_DIR}/OMGsim vis-AA-${angle}deg-${ene}eV.mac AA_ANTARES_${angle}deg-${ene}eV.root
	mv AA_ANTARES_${angle}deg-${ene}eV.root ${PROG_DIR}/prod
  ene=`echo "$ene+$step" | bc -l`
done

cd ../
rm -r ${TEMPDIR}
