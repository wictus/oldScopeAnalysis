#!/bin/bash

counter=0
for i in {43..463..30}
 do
    if [ "$counter" == "7" ]
    then
        counter=0
	echo "Waiting"
        wait
    fi
	echo 50cmData/mergedPositions/7x19x500_colimator_merged_$i.matchedHits.hits.root
	./main.x -t root -f 50cmData/mergedPositions/7x19x500_colimator_merged_$i.matchedHits.hits.root -b 1&
    echo "$counter"
    counter=$(($counter+1));
done 
