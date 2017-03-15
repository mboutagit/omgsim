bs=${BASH_SOURCE[0]}
dn=$(dirname $bs)/..
echo "$dn"

if [[ -n $(echo $dn | grep /) ]]
then
    OMGSim_ROOT=${dn%/*}
fi

if [[ ${OMGSim_ROOT:0:1} != "/" ]]
then
    ROOTPATH=$PWD
fi

OMGSim_ROOT=$ROOTPATH/$OMGSim_ROOT/../
export OMGSim_ROOT
export LD_LIBRARY_PATH=$OMGSim_ROOT/build:$LD_LIBRARY_PATH
echo $OMGSim_ROOT