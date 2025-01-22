echo "OMGsim -fa scripts/DOMK40Glass_small.mac outputfile.root"
OMGsim -fa scripts/DOMK40Glass_small.mac outputfile.root
if [ ! -f outputfile.root ]; then >&2 echo ERROR: ouput file does not exist; exit 1; fi
if [ ! -s outputfile.root ]; then >&2 echo ERROR: ouput file is empty; exit 1; fi
echo "test finished"
