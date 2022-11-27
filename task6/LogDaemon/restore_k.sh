#! /bin/bash

for sample_name in `ls ./daemon_sample | sort -r -d | head -$2`
do
    patch -s -N -R -d $1 < ./daemon_sample/$sample_name > /dev/null
    rm ./daemon_sample/$sample_name
done

