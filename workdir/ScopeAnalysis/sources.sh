#!/bin/bash

counter=0
for i in {1..9..1}
 do
    if [ "$counter" == "7" ]
    then
        counter=0
	echo "Waiting"
        wait
    fi
	./main.x -t root -f Data/5x19x300_source_$i.reco.sig.root -b 1&
    echo "$counter"
    counter=$(($counter+1));
done 
