#! /bin/bash

while getopts ":k:r" options;
do
  case "$options" in
    r)  patch_reverse=-R
        echo "$OPTARG"
        ;;
    k) sample_deep=${OPTARG};;
  esac
done

for sample_name in `ls ./SampleData | sort -r -d | head -$sample_deep`
do
    patch -s -N $patch_reverse -d ./DaemonData < ./SampleData/$sample_name > /dev/null
done