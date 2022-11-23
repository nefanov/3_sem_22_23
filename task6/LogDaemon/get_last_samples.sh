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

for sample_name in `ls ./daemon_sample | sort -r -d | head -$sample_deep`
do 
    echo "$sample_name"
    patch -N $patch_reverse -d ./daemon_closet < ./daemon_sample/$sample_name
done

