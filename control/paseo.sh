#!/bin/bash 
# planificación y control 
#

# Entrada: tres pares de coordenadas, recibidas como argumento:
# Posición del robot: ax, ay 
# Extremo de orientación con respecto a ax, ay: bx, by 
# Punto destino en el mapa: dx, dy

vlineal=28
vangular_i=33
vangular_d=33

declare -i ax=$1 ay=$2 bx=$3 by=$4 dx=$5 dy=$6 vx vy px py

echo $ax, $ay, $bx, $by, $dx, $dy

#Trasnsporte al origen
vx=$bx-$ax
vy=$by-$ay
px=$dx-$ax
py=$dy-$ay 

echo $vx $vy $px $py
declare -i rotacion=$(./angulo.pl $vx $vy $px $py)
echo Rotación: $rotacion

#Control 

echo Comenzando rotación y avance

#Velocidad angular en grados/s
if [[ $rotacion -lt 0 ]];
then
	echo rot vang $rotacion $vangular_d
	seg_angular=$(echo -1*$rotacion/$vangular_d|bc -l)
	mov="d"
       echo Derecha 	
else 
	echo rot vang $rotacion $vangular_i
	seg_angular=$(echo $rotacion/$vangular_i|bc -l)
	mov="a"
       echo Izquierda 	
fi 

# sleep $seg_angular
echo Rotación $seg_angular segundos

#Velocidad lineal en cm/s
distancia=$(echo "sqrt(($ax-$dx)^2+($ay-$dy)^2)"|bc -l)
seg_dist=$(echo "$distancia/$vlineal" |bc -l)

(echo "b"; sleep 4; sleep 1; 
echo $mov ; echo "$mov" ; echo "$mov"; sleep $seg_angular
echo "b"; sleep 1; echo "b"; sleep 3; echo "w" ;  sleep $seg_dist;
echo "b";  sleep 2 ; echo "b" | sleep 1 | echo "x" ) | ssh odor@172.16.0.10 "cd odor/motores-odor && ./robot.sh"


#echo Avanzando $seg_dist segundos. 
#sleep $seg_dist
echo Detenido
