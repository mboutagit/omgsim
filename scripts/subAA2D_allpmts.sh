#!/bin/bash 
echo PROG_DIR is $PROG_DIR
mkdir ${PROG_DIR}/prod/log/
qsub -P P_antares -N AntAA -l ct=6000 -l os="cl7" -o ${PROG_DIR}/prod/log/log.txt -e ${PROG_DIR}/prod/log/log.txt -l sps=1 -t 1:100 jobAA2D_allpmts.sh
qsub -P P_antares -N AntAA -l ct=2000 -l os="cl7" -o ${PROG_DIR}/prod/log/log.txt -e ${PROG_DIR}/prod/log/log.txt -l sps=1 -t 101-181 jobAA2D_allpmts.sh
