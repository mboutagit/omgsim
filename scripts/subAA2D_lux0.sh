#!/bin/bash 
echo PROG_DIR is $PROG_DIR
mkdir ${PROG_DIR}/prod/log/
qsub -P P_km3net -N DOMAA -l ct=100000 -o ${PROG_DIR}/prod/log/log.txt -e ${PROG_DIR}/prod/log/log.txt -l sps=1 -t 1:100 jobAA2D_lux0.sh
qsub -P P_km3net -N DOMAA -l ct=100000 -o ${PROG_DIR}/prod/log/log.txt -e ${PROG_DIR}/prod/log/log.txt -l sps=1 -t 101-101 jobAA2D_lux0.sh
