#!/bin/bash

counter=0
places=( 101  102  103  104  111  112  121  122  123  141  142  150  151  152  160  171  172  173 )
for i in "${places[@]}"
 do
    if [ "$counter" == "7" ]
    then
        counter=0
	echo "Waiting"
        wait
    fi
	./main.x -t root -f 50cmSources/7x19x500_sources_$i.reco.sig.root -b 1&
    echo "$counter"
    counter=$(($counter+1));
done 
