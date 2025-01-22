#!/bin/bash

#for local test
#export SGE_TASK_ID=1
#cd work

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

#qsub has no SGE_TASK_ID = 0
#shift=`echo "scale=3;2.5+(${SGE_TASK_ID}-1)*5" | bc -l`
let shift=SGE_TASK_ID-1

for shift in `seq 0 1 45`; do
	echo $shift
	sed "s|%shift|${shift}|g" ${PROG_DIR}/scripts/vis-KM3scan_lux0.mac > vis-KM3scan-${shift}mm_lux0.mac
	${PROG_DIR}/OMGsim -fa vis-KM3scan-${shift}mm_lux0.mac KM3scan_lux0_${shift}mm.root
	mv KM3scan_lux0_${shift}mm.root ${PROG_DIR}/prod
done

cd ../
rm -r ${TEMPDIR}
