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
#shift=`echo "scale=3;2.5+(${SGE_TASK_ID}-1)*5" | bc -l`
let shift=SGE_TASK_ID-1

sed "s|%shift|${shift}|g" ${PROG_DIR}/scripts/vis-KM3scan.mac > vis-KM3scan-${shift}mm.mac
${PROG_DIR}/OMGsim -fa vis-KM3scan-${shift}mm.mac KM3scan_${shift}mm.root
mv KM3scan_${shift}mm.root ${PROG_DIR}/prod

cd ../
#rm -r ${TEMPDIR}
