#!/bin/bash

counter=0
for i in {16..286..30}
 do
    if [ "$counter" == "6" ]
    then
        counter=0
	echo "Waiting"
        wait
    fi
	echo 30cmData/mergedPositions/5x19x300_colimator_merged_$i.matchedHits.hits.root
	./main.x -t root -f 30cmData/mergedPositions/5x19x300_colimator_merged_$i.matchedHits.hits.root -b 1&
    echo "$counter"
    counter=$(($counter+1));
done 
