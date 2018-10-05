#!/bin/bash
#for local test
#export SGE_TASK_ID=1
#mkdir work
#cd work

if [[ -z $G4ROOT ]]
then
    export G4VERSION=4.10.0;for i in /sps/km3net/users/hugon/developpement/env/*.sh; do source $i; done
fi

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

mkdir common
${PROG_DIR}/OMGsim -fa ${PROG_DIR}/scripts/K40GlassInAirDOM.mac K40GlassInAir_DOM_${SGE_TASK_ID}.root 
mv K40GlassInAir_DOM_${SGE_TASK_ID}.root ${PROG_DIR}/prod
cd ../
rm -r ${TEMPDIR}
