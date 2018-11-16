#!/bin/bash


graphFiles=`ls ./data/ | grep .gr`

for graph in $graphFiles
do
	filename=`echo $graph | cut -d'.' -f1`
	echo $graph $filename
	./RunExperiments ./data/$graph ./data/$filename.extra ./results/$filename_output.txt


done
