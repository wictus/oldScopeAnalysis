#!/bin/bash

counter=0
for i in {3..300..3}
 do
    if [ "$counter" == "6" ]
    then
        counter=0
	echo "Waiting"
        wait
    fi
	./main.x -t root -f 30cmData/5x19x300_colimator_$i.reco.sig.root&
    echo ./main.x -t root -f 30cmData/5x19x300_colimator_$i.reco.sig.root
    echo "$counter"
    counter=$(($counter+1));
done 
