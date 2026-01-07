#!/bin/bash

macro_file_in="./scripts/lactel_template.mac";
macro_file="./scripts/lactel.mac";
commands="-s -fa -evt"

visualisation=0
analysisType="AA"

sed "s/isVis/${visualisation}/g; s/anaType/${analysisType}/g" ${macro_file_in} > ${macro_file}

echo "./OMGsim ${commands} ${macro_file}"
./OMGsim ${commands} ${macro_file}