#!/bin/bash


createDirs() {
    for dir in $@; do 
        if [[ ! -d $dir ]]; then
            mkdir -pv $dir
        fi
    done
}

macDir="./macros"
outDir="./simOutputs"
nev=$1
vis=$2
runSim=$3
detConf="M2"

createDirs $macDir $outDir

for theta in 0 5 10 15 20 25 30 35 40 45 50 55 60; do 

macrofile=${macDir}"/tmp_za${theta}_nev${nev}.mac"
outRootFile=${outDir}"/omgsim_${detConf}_muons_za${theta}_nev${nev}.root"


AccAngle="${theta} deg"


echo "/KM3/XP/Type BA">> $macrofile

if [[ -f "$macrofile" ]]; then
    echo -e "File $macrofile already exists.\nDeleting it .. \n"
    rm -fv $macrofile
fi

echo "/KM3/XP/Type BA"                            >> $macrofile
echo "/KM3/det/DetInit Lactel"                    >> $macrofile
echo "/KM3/det/DetectorTypeName LactelOneCell"    >> $macrofile
echo "/KM3/det/setMeanCapRadius 5 cm"             >> $macrofile
echo "/KM3/det/setTargetScattering 1"             >> $macrofile
echo "/KM3/det/setTargetMaterial IdealWater"      >> $macrofile
echo "/KM3/det/setWorldLength 11.0 m "            >> $macrofile
echo "/KM3/det/setTargetLength  .5 m"             >> $macrofile
echo "/KM3/BDet/targetType ${detConf}"                    >> $macrofile
echo "/KM3/BDet/acceptanceAngle $AccAngle"        >> $macrofile
# echo "/KM3/BDet/OMPosition 0 0 -50 cm"           >> $macrofile



echo "/KM3/GenType 3"     >> $macrofile
echo "/run/initialize"    >> $macrofile

if [[ $vis = "OK" ]]; then
    echo "Visualization ON!"
    echo -e "\n#Visualization ON!"                    >> $macrofile
    echo "/vis/open OGL"                              >> $macrofile
    echo "/vis/viewer/set/style wireframe"            >> $macrofile
    echo "/vis/drawVolume"                            >> $macrofile
    echo "/vis/viewer/set/background 1.0 1.0 1.0 0.2" >> $macrofile
fi

if [[ "$nev" =~ ^-?[0-9]+$ ]]; then
    echo "Number of event to simulate: $nev"
    echo -e "\n#Number of event to simulate: $nev"     >> $macrofile
    echo "/run/beamOn $nev"                              >> $macrofile
fi


if [[ $runSim = "OK" && -n "$nev" && -x "./OMGsim" ]]; then
    echo "./OMGsim  -fa $macrofile -of ${outRootFile}"
    ./OMGsim $macrofile -of ${outRootFile}
fi

done