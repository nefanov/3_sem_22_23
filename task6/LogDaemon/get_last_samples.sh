#! /bin/bash

for sample_name in `ls ./daemon_sample | sort -r -d | head -$1`
do 
    echo "$sample_name"
    patch -N -R -d ./daemon_closet < ./daemon_sample/$sample_name
done

