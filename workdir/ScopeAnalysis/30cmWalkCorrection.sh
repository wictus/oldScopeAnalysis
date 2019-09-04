#!/bin/bash

counter=0
for i in 30 96 150 210 264
 do
    if [ "$counter" == "6" ]
    then
        counter=0
	echo "Waiting"
        wait
    fi
	./main.x -t root -f DataForWalk/5x19x300_colimator_fivePositons_pos$i.reco.sig.root -b 1&
    echo "$counter"
    counter=$(($counter+1));
done 
