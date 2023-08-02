#!/bin/bash

./js | stdbuf -i0 -o0 -e0  egrep "derecha|stop|reversa|izquierda|adelante" 2>&1 | while read linea; do 
	if [[ $linea == "adelante" ]]; then 
		echo w ;
	elif [[ $linea == "reversa" ]]; then 
		echo s ;
	elif [[ $linea == "izquierda" ]]; then 
		echo a ;
	elif [[ $linea == "derecha" ]]; then 
		echo d ;
	elif [[ $linea == "stop" ]]; then 
		echo b ;
	fi 

done | ssh odor@172.16.0.10 "cd odor/motores-odor && ./robot.sh"



