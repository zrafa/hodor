#!/bin/bash

ssh odor@172.16.0.10 "killall apriltag_demo"


export x_dest=$1 
export y_dest=$2

echo $x_dest

echo "Localizando..."

param=$(ssh odor@172.16.0.10 apriltag_demo | stdbuf -o 0 grep distancia | 
./localiza ) 
./paseo.sh $param $x_dest $y_dest
