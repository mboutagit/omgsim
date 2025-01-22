#!/bin/bash 
echo PROG_DIR is $PROG_DIR
mkdir ${PROG_DIR}/prod/log/
qsub -P P_km3net -N KM3scan -l ct=6000 -l os="cl7" -o ${PROG_DIR}/prod/log/log.txt -e ${PROG_DIR}/prod/log/log.txt -l sps=1 -t 1-46:1 ./job_KM3scan.sh
