#!/bin/bash

acos ()
{
    if (( $(echo "$1 == 0" | bc -l) )); then
        thetarad=`echo "a(1)*2" | bc -l`
    elif (( $(echo "(-1 <= $1) && ($1 < 0)" | bc -l) )); then
        thetarad=`echo "a(1)*4 - a(sqrt((1/(($1)^2))-1))" | bc -l`
    elif (( $(echo "(0 < $1) && ($1 <= 1)" | bc -l) )); then
        thetarad=`echo "a(sqrt((1/(($1)^2))-1))" | bc -l`
    else
        echo "input out of range"
        return 1
    fi
    printf "%.4f" `echo "${thetarad}/a(1)*45" | bc -l`
}
#for local test
#export SGE_TASK_ID=1
#cd work

TEMPDIR=`pwd`/temp_${SGE_TASK_ID}
mkdir ${TEMPDIR}
cd ${TEMPDIR}

#qsub has no SGE_TASK_ID = 0
#let angle=SGE_TASK_ID-1
cosTheta=`echo "($SGE_TASK_ID-51)/50" | bc -l`
cosTheta=`printf "%.2f" $cosTheta`
angle=`acos $cosTheta`
echo "Angle $angle [deg] cos=$cosTheta"
wlmin=250
wlmax=750
nsteps=50
step=`echo "scale=1;($wlmax-$wlmin)/$nsteps" | bc -l`
let nsteps+=1
wl=$wlmin
for i in `seq 1 $nsteps`;
do
	eneprec=`echo "2*4*a(1)*197.327/$wl" | bc -l`
	ene=`printf "%.5f" $eneprec`
	echo $i $wl $ene
	sed "s|%angle|${angle}|g;s|%ene|$ene|g" ${PROG_DIR}/scripts/DOMAA2D_lux0.mac > DOMAA_lux0-${cosTheta}-${wl}nm.mac
	${PROG_DIR}/OMGsim DOMAA_lux0-${cosTheta}-${wl}nm.mac DOMAA_lux0_${cosTheta}-${wl}nm.root
	mv DOMAA_lux0_${cosTheta}-${wl}nm.root ${PROG_DIR}/prod
	wl=`echo "$wl+$step" | bc -l`
	wl=`printf "%.0f" $wl`
done

cd ../
rm -r ${TEMPDIR}
