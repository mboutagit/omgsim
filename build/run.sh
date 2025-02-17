#!/bin/bash

if [ $# != 1 ]; then
echo "Wrong number of arguments."
echo "Synatxe: $0 nb_of_runs"
echo "Exiting.."
exit
fi

runNb=$1;

here=`pwd`
resDir="${here}/results"
jobDir="${resDir}/job"

if [ ! -d $resDir ]; then 
mkdir -pv $resDir
fi

if [ ! -d $jobDir ]; then 
mkdir -pv $jobDir
fi


for run in $(seq 1 $runNb); do
#nohup ./OMGsim -s -evt -fa -ka scripts/DOMgamma.mac "${resDir}/res_$run.root"  > ${jobDir}/log_$run.log 2>&1 &
./OMGsim -s -evt -fa -ka scripts/DOMgamma.mac "${resDir}/res_$run.root"  > ${jobDir}/log_$run.log 2>&1
done
