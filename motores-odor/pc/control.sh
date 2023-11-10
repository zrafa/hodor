#!/bin/bash


# adelante 224 cm en 10 segundos
# 360 grados a la izquierda toman 9,37 segundos
# 360 grados a la izquierda toman 10,7 segundos


( echo "b"; sleep 4; sleep 1; 
echo "d" ; sleep 10,5
echo "b";  sleep 2 ; echo "b" | sleep 1 | echo "x" ) | ssh odor@172.16.0.10 "cd odor/motores-odor && ./robot.sh"



