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

cp  ${PROG_DIR}/KM3Sim ./
mkdir common
cp  -r ${PROG_DIR}/../common/data ./common
cp ${PROG_DIR}/scripts/vis-K40-DOM.mac ./
./KM3Sim -fa vis-K40-DOM.mac K40_DOM_${SGE_TASK_ID}.root 
mv K40_DOM_${SGE_TASK_ID}.root ${PROG_DIR}/prod
cd ../
rm -r ${TEMPDIR}
