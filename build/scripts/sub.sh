#!/bin/bash 
echo PROG_DIR is $PROG_DIR
mkdir ${PROG_DIR}/prod/log/ 

DATE=`date '+y%Ym%md%dh%Hm%Ms%S'`
export SCRIPTNAME=${1}
export CPU_TIME=${2} #20000 K40Glass, 200000 U238/Th232

COUNTER=0
while [  $COUNTER -lt ${3} ]; do
	let C1=$COUNTER+1 
	let C2=$COUNTER+100
	#qsub -P P_km3net -N ${SCRIPTNAME}  -l ct=20000 -l os="cl7" -o ${PROG_DIR}/prod/log/log_${SCRIPTNAME}_${DATE}.txt -e ${PROG_DIR}/prod/log/log_${SCRIPTNAME}_${DATE}.txt -l sps=1 -t $C1-$C2 "job.sh"
	qsub -P P_km3net -N ${SCRIPTNAME}  -l ct=${CPU_TIME} -l os="cl7" -o ${PROG_DIR}/prod/log/log_${SCRIPTNAME}_${DATE}.txt -e ${PROG_DIR}/prod/log/log_${SCRIPTNAME}_${DATE}.txt -l sps=1 -t $C1-$C2 "job.sh"

	let COUNTER=COUNTER+100
done
