#!/bin/bash

counter=0
for i in {225..471..3}
 do
    if [ "$counter" == "6" ]
    then
        counter=0
	echo "Waiting"
        wait
    fi
	echo 50cmData/7x19x500_colimator_$i.reco.sig.root
	./main.x -t root -f 50cmData/7x19x500_colimator_$i.reco.sig.root -b 1&
    echo "$counter"
    counter=$(($counter+1));
done 
