#!/bin/bash

for CTIME in 26 28 30 32; do
    for VTIME in 16 18 20 22 24; do
        echo $CTIME $VTIME
        ./psa_sim.py -c 5 --ctime $CTIME --vtime $VTIME
    done
done

             
