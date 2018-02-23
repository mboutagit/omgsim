#!/bin/bash
#for local test
#export SGE_TASK_ID=1
#mkdir work
#cd work

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

mkdir common
${PROG_DIR}/OMGsim -fa ${PROG_DIR}/scripts/vis-K40-DOM.mac K40_DOM_${SGE_TASK_ID}.root 
mv K40_DOM_${SGE_TASK_ID}.root ${PROG_DIR}/prod
cd ../
rm -r ${TEMPDIR}
