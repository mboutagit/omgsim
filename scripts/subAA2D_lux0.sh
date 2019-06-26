#!/bin/bash 
echo PROG_DIR is $PROG_DIR
mkdir ${PROG_DIR}/prod/log/
qsub -P P_km3net -N DOMAA -l ct=10000 -o ${PROG_DIR}/prod/log/log.txt -e ${PROG_DIR}/prod/log/log.txt -l sps=1 -t 1:100 jobAA2D_lux0.sh
qsub -P P_km3net -N DOMAA -l ct=5000 -o ${PROG_DIR}/prod/log/log.txt -e ${PROG_DIR}/prod/log/log.txt -l sps=1 -t 101-181 jobAA2D_lux0.sh
