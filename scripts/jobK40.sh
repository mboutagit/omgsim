#!/bin/bash
#for local test
#export SGE_TASK_ID=1
#mkdir work
#cd work

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

mkdir common
${PROG_DIR}/OMGsim -fa -evt ${PROG_DIR}/scripts/vis-K40-DOM.mac K40_DOM_${SGE_TASK_ID}.root 
mv K40_DOM_${SGE_TASK_ID}.root ${PROG_DIR}/prod
gzip output.evt
mv output.evt.gz ${PROG_DIR}/prod K40_DOM_${SGE_TASK_ID}.evt.gz
cd ../
rm -r ${TEMPDIR}
