#!/bin/bash

while true; do
	read -n 1 c

	echo $c 

done | stdbuf -i0 -o0 -e0  ssh odor@172.16.0.10 "cd odor/motores-odor && ./robot.sh"



