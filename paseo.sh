#!/bin/bash 

#DEBUG=DEBUG

function ayuda () {
cat << FIN
El software $0 fue diseñado para implementar planificación y
control del robot HODOR de FAI. El mismo espera como argumento
tres pares de coordenadas:
  * Posición del robot: ax, ay 
  * Extremo de orientación con respecto a ax, ay: bx, by  
  * Punto destino en el mapa: dx, dy

Invocación: $O [-v <vangular> ] [-l <vlineal>] ax ay bx by dx dy 


FIN
exit 1
}

while getopts ":v:l:" opt; do
  case ${opt} in
    v)
      vangular=$OPTARG
      ;;
    l)
      vlineal=$OPTARG
      ;;
    \?)
      echo "Opción no válida: -$OPTARG" 1>&2
      ayuda
      ;;
    :)
      echo "La opción -$OPTARG requiere un argumento." 1>&2
      ayuda
      ;;
  esac
done
shift $((OPTIND -1))

[[ $# -lt 6 ]] && ayuda 

# Constantes 
pi=$(echo "4*a(1)"|bc -l)
#Velocidad angular en grados/s predeterminada si no se provee 
: ${vangular:=36}
#Velocidad lineal en cm/s predeterminada si no se provee 
: ${vlineal=10}

function determinar_angulo {
# determinar el cuadrante y ángulo en radianes de un vector
x=$1
y=$2

if [[ $x -gt 0 ]] && [[ $y -gt 0 ]] ;then 
       	cuadrante=1
	angulo=$(echo "a($y/$x)"|bc -l)
elif [[ $x -lt 0 ]] && [[ $y -gt 0 ]] ;then 
        cuadrante=2
	angulo=$(echo "${pi}-a($y/(-1)*($x))"|bc -l)
elif [[ $x -lt 0 ]] && [[ $y -lt 0 ]] ;then
        cuadrante=3
	angulo=$(echo "${pi}+a((-1)*($y)/(-1)*($x))"|bc -l)
elif [[ $x -gt 0 ]] && [[ $y -lt 0 ]];then 
        cuadrante=4
	angulo=$(echo "2*${pi}-a((-1)*($y)/$x)"|bc -l)
else
        cuadrante="Origen"
fi 
    echo $angulo
}

function angulo {
#Calcula la diferencia angular entre 
#dos vectores pasados como argumento
if [ $# != 4 ] ;then 
    echo "Uso: $0 x1 y1 x2 y2"
    return
fi

x1=$1
y1=$2
x2=$3
y2=$4

# Procesar cada vector en grados
ang1=$(echo "($(determinar_angulo $x1 $y1)*180)/$pi"|bc -l)
ang2=$(echo "($(determinar_angulo $x2 $y2)*180)/$pi"|bc -l) 

difang=$(echo "($ang1)-($ang2)"|bc -l)
echo $difang

}


#Comienzo código principal 
declare -i ax=$1 ay=$2 bx=$3 by=$4 dx=$5 dy=$6 vx vy px py

#Trasnsporte al origen
vx=$bx-$ax
vy=$by-$ay
px=$dx-$ax
py=$dy-$ay 
# El movimiento de HODOR se compone de dos partes
# Una rotación que indica a HODOR cuánto debe girar para "mirar"
# hacia el destino y un avance en línea recta posterior a la
# rotación
[[ -v DEBUG ]] && echo $vx $vy $px $py
rotacion=$(angulo $vx $vy $px $py)
echo rotación necesaria calculada: $rotacion
if [[ "${rotacion:0:1}" = "-" ]]; then
       seg_angular=$(echo "(-1)*${rotacion}/${vangular}"|bc -l)
       direccion=izquierda 
else 
       seg_angular=$(echo "${rotacion}/${vangular}"|bc -l)
       direccion=derecha 	
fi 
echo Comenzando rotación a $direccion $seg_angular segundos
# Enviar comando para rotación a HODOR aquí
sleep $seg_angular
echo Rotación finalizada
# Enviar comando para FINALIZAR rotación a HODOR aquí

echo Comenzando avance lineal 
distancia=$(echo "sqrt(($ax-$dx)^2+($ay-$dy)^2)"|bc -l)
seg_dist=$(echo "$distancia/$vlineal" |bc -l)
echo Avanzando $seg_dist segundos. 
# Enviar comando para avance a HODOR aquí
sleep $seg_dist
echo Detener 
# Enviar comando para FINALIZAR avance a HODOR aquí
