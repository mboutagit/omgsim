#!/bin/zsh
script=${0##*/}

# ------------------------------------------------------------------------------------------
#
#                         Script to use OMGsim MC production to produce fit model
#
# ------------------------------------------------------------------------------------------

export JPP_DIR=/pbs/throng/km3net/src/Jpp/v11.0.0/
export PROD_DIR=/sps/km3net/users/kulikovs/OMGsim/gittrunk/PostAnalysis/scripts/prod

if [ -z $JPP_DIR ]; then
    echo "Variable JPP_DIR undefined."
    exit
fi

source $JPP_DIR/setenv.sh $JPP_DIR
set_variable DEBUG  3

set_variable LIGHT_SCALE       1       #Multiply number of hits production in KM3 and JSirene
set_variable BACKGROUND_HZ     0 0 0 0 0
set_variable DETECTOR /sps/km3net/users/kulikovs/data_processing/data_processing_clean/trunk/singleDOM_OMGsim.detx
set_variable DETECTORB $DETECTOR
set_variable PAR_FILE trigger/PMT_K40.txt
set_variable TRIGGER_PAR_FILE TRIGGER_K40.txt
set_variable USE_TAILS '-b tails -B "16 20"'
set_variable QESCALE ${QESCALE:-1.0}


set_variable WORKDIR `pwd`/tmp
mkdir $WORKDIR
cd $WORKDIR

#notice "Checking out JEP script."
#svn co http://svn.km3net.de/data_processing/trunk/trigger
cp -r /sps/km3net/users/kulikovs/OMGsim/gittrunk/PostAnalysis/scripts/trigger ./

if [ -z $QESCALE ]; then
	fatal "QESCALE is not defined"
fi

print_variable QESCALE
set_variable QESCALE_NORM `echo "${QESCALE}/2." | bc -l`

print_variable QESCALE_NORM
set_variable FACTOR ${QESCALE_NORM}
set_variable EVENTPROCESSOR_FACTOR ${QESCALE_NORM}

extract_light () {
	cp $LIGHT_FILE ${LIGHT_FILE:t}
	set_variable LIGHT_FILE ${LIGHT_FILE:t:r}
	gunzip ${LIGHT_FILE}.gz
}

get_livetime_water () {
	set_variable NGEN 0
	for i in `cat $LIGHT_FILE | grep norma | cut -f 3 -d " "`; do
		set_variable NGEN `echo "$NGEN+$i" | bc -l`
	done
	set_variable NEVT `cat $LIGHT_FILE | grep start_event | wc -l`
	set_variable EVENTPROCESSOR_RATE `echo "$NEVT/($NGEN/(${ACTIVITY_BQPERM3}*4/3.*3.14159265359*10^3))" | bc -l`
	set_variable LIVETIME_S `echo "$NGEN/(${ACTIVITY_BQPERM3}*4/3.*3.14159265359*10^3)" | bc -l`
	print_variable NGEN NEVT EVENTPROCESSOR_RATE LIVETIME_S
}

get_livetime_glass () {
	echo "running get_livetime_glass"
        set_variable NGEN 0
        for i in `cat $LIGHT_FILE | grep norma | cut -f 3 -d " "`; do
                set_variable NGEN `echo "$NGEN+$i" | bc -l`
        done
	print_variable NGEN
        set_variable NEVT `cat $LIGHT_FILE | grep start_event | wc -l`
        set_variable EVENTPROCESSOR_RATE `echo "$NEVT/($NGEN/(17.2*${ACTIVITY_BQPERKG}))" | bc -l`
        set_variable LIVETIME_S `echo "$NGEN/(17.2*${ACTIVITY_BQPERKG})" | bc -l`
        print_variable NEVT EVENTPROCESSOR_RATE LIVETIME_S
}
run_component () {
	set_variable TRIGGER_FILE ${LIGHT_FILE:t:r:r}.jep_${QESCALE}.root
	set_variable JETW_PIPE ${TRIGGER_FILE:r}.pipe.dat
	set_variable RETRIGGER_FILE ${TRIGGER_FILE}
	set_variable JMCK40_FILE ${RETRIGGER_FILE:r}.jmon.jrate.root
	if true; then
		./trigger/JEP.zsh
		JCalibrateK40 -M "2 31" -T 20 -V "0 100000" -a ${DETECTOR} -f ${RETRIGGER_FILE} -o ${RETRIGGER_FILE:r}.jmon.root -C JDAQTimesliceL1
	fi
	JMergeCalibrateK40 -f ${RETRIGGER_FILE:r}.jmon.root -o ${JMCK40_FILE}
	cp ${TRIGGER_FILE} ${JMCK40_FILE} ${RETRIGGER_FILE:r}.jmon.root ${PROD_DIR}
}


#K40 in water
#set_variable LIGHT_FILE /sps/km3net/users/kulikovs/OMGsim/gittrunk/build/prod/DOMK40Water_QE2_hadd1000files.evt.gz
set_variable LIGHT_FILE /sps/km3net/users/kulikovs/OMGsim/gittrunk/buildnew/prod/DOMK40Water_fixwater_QE2_100files.evt.gz
set_variable ACTIVITY_BQPERM3 13750
extract_light
get_livetime_water
run_component
set_variable WATER_K40_FILE $JMCK40_FILE
#set_variable WATER_K40_FILE ${PROD_DIR}/DOMK40Water_QE2_hadd1000files.jep_${QESCALE}.jmon.jrate.root
notice "Water only"
#JFitK40 -w -a ${DETECTOR} -f ${WATER_K40_FILE} -o WaterK40.qe${QESCALE}.jrate.jfit.root -! "-1 -1" -O WL
#

if false; then

#K40 in glass
set_variable LIGHT_FILE /sps/km3net/users/kulikovs/OMGsim/gittrunk/build/prod/DOMK40Glass_QE2_hadd100files.evt.gz
set_variable ACTIVITY_BQPERKG 60
extract_light
get_livetime_glass
run_component
set_variable GLASS_K40_FILE $JMCK40_FILE
#set_variable GLASS_K40_FILE ${PROD_DIR}/DOMK40Glass_QE2_hadd100files.jep_${QESCALE}.jmon.jrate.root
notice "Water + GlassK40"
echo JOpera2D -f "${WATER_K40_FILE}:1.2R ${GLASS_K40_FILE}:1.2R" -o "WaterK40GlassK40.qe${QESCALE}.jopera.root" -O "1.2R"
JOpera2D -f "${WATER_K40_FILE}:1.2R ${GLASS_K40_FILE}:1.2R" -o "WaterK40GlassK40.qe${QESCALE}.jopera.root" -O "1.2R"
#JFitK40 -w -a ${DETECTOR} -f WaterK40GlassK40.qe${QESCALE}.jopera.root -o  WaterK40GlassK40.qe${QESCALE}.jopera.jfit.root -! "-1 -1" -O WL


#U238 in glass
set_variable LIGHT_FILE /sps/km3net/users/kulikovs/OMGsim/gittrunk/build/prod/DOMU238Glass_QE2_hadd100files.evt.gz
set_variable ACTIVITY_BQPERKG 5
extract_light
get_livetime_glass
run_component
set_variable U238_FILE $JMCK40_FILE
#set_variable U238_FILE  ${PROD_DIR}/DOMU238Glass_QE2_hadd100files.jep_${QESCALE}.jmon.jrate.root
notice "Water + GlassK40 + U238"
JOpera2D -f "WaterK40GlassK40.qe${QESCALE}.jopera.root:1.2R ${U238_FILE}:1.2R" -o "WaterK40GlassK40U238.qe${QESCALE}.jopera.root" -O "1.2R"
#JFitK40 -w -a ${DETECTOR} -f WaterK40GlassK40U238.qe${QESCALE}.jopera.root -o WaterK40GlassK40U238.qe${QESCALE}.jopera.jfit.root -! "-1 -1" -O WL

#Th232 in glass
set_variable LIGHT_FILE /sps/km3net/users/kulikovs/OMGsim/gittrunk/build/prod/DOMTh232Glass_QE2_hadd100files.evt.gz
set_variable ACTIVITY_BQPERKG 1.35
extract_light
get_livetime_glass
run_component
set_variable TH232_FILE $JMCK40_FILE
#set_variable TH232_FILE ${PROD_DIR}/DOMTh232Glass_QE2_hadd100files.jep_${QESCALE}.jmon.jrate.root
notice "Water + GlassK40 + U238 +Th232"
JOpera2D -f "WaterK40GlassK40U238.qe${QESCALE}.jopera.root:1.2R ${TH232_FILE}:1.2R" -o "WaterK40GlassK40U238Th232.qe${QESCALE}.jopera.root" -O "1.2R"

#U238 in water
set_variable LIGHT_FILE /sps/km3net/users/kulikovs/OMGsim/gittrunk/build/prod/DOMU238Water_QE2_hadd110e6.evt.gz
set_variable ACTIVITY_BQPERM3 40
extract_light
get_livetime_water
run_component
set_variable WATER_U238_FILE $JMCK40_FILE
#set_variable WATER_U238_FILE /sps/km3net/users/kulikovs/data_processing/data_processing_clean/trunk/DOMU238Water_100files.jep_${QESCALE}.jmon.jrate.root
notice "Water K40 + U238 + Glass K40 + U238 +Th232"
JOpera2D -f "WaterK40GlassK40U238Th232.qe${QESCALE}.jopera.root:1.2R ${WATER_U238_FILE}:1.2R" -o "WaterK40U238GlassK40U238Th232.qe${QESCALE}.jopera.root" -O "1.2R"

echo fitting back
JFitK40 -w -E 0.0000001 -@ "Rate_Hz=17.905; p1=3.1653; p2=-1.3200; p3=0.8994; p4=1.1155" -a ${DETECTOR} -f WaterK40U238GlassK40U238Th232.qe${QESCALE}.jopera.root -o WaterK40U238GlassK40U238Th232.qe${QESCALE}.jopera.jfit.root -! "-1 -1" -O WL

#fitting for model
#JFitK40 -w -M -a ${DETECTOR} -f WaterK40U238GlassK40U238Th232.qe${QESCALE}.jopera.root -o WaterK40U238GlassK40U238Th232.qe${QESCALE}.jopera.jfit.root -! "-1 -1" -O WL

#cp $WATER_K40_FILE $GLASS_K40_FILE $U238_FILE $TH232_FILE ${PROD_DIR}
cp WaterK40GlassK40.qe${QESCALE}.jopera.root WaterK40GlassK40U238.qe${QESCALE}.jopera.root WaterK40GlassK40U238Th232.qe${QESCALE}.jopera.root WaterK40U238GlassK40U238Th232.qe${QESCALE}.jopera.root WaterK40U238GlassK40U238Th232.qe${QESCALE}.jopera.jfit.root ${PROD_DIR} ${PROD_DIR}

fi
