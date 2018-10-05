#!/bin/bash 
echo PROG_DIR is $PROG_DIR
mkdir ${PROG_DIR}/prod/log/ 

DATE=`date '+y%Ym%md%dh%Hm%Ms%S'`

COUNTER=0
while [  $COUNTER -lt 100 ]; do
	let C1=$COUNTER+1 
	let C2=$COUNTER+100
	qsub -P P_km3net -N K40DOM -l ct=100000 -l os="cl7" -o ${PROG_DIR}/prod/log/log_K40_${DATE}.txt -e ${PROG_DIR}/prod/log/log_K40_${DATE}.txt -l sps=1 -t $C1-$C2 jobK40.sh
	let COUNTER=COUNTER+100
done
