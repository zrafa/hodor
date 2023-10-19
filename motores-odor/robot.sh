#!/bin/bash



DEV=/dev/ttyACM0

killall cat

sleep 1

stty -F $DEV speed 9600


cat /dev/ttyACM0 &



function motores {
	declare -i vizq
	declare -i vder

	vizq=$1
	vder=$2

	# Horrible hack para evitar que envie un 0a (enter)
	# 0a da problemas
	if [[ vizq -eq 10 ]]; then
		vizq=11
	fi
	if [[ vder -eq 10 ]]; then
		vder=11
	fi

	echo $vizq $vder

	VIZQ=`echo -n "$vizq" | awk '{printf "%02x", $1}'`
	VDER=`echo -n "$vder" | awk '{printf "%02x", $1}'`

	echo "$VIZQ $VDER"
	echo -en X"\x$VIZQ\x$VDER" >> $DEV
}

function adelante {
	echo $0
	motores velder velizq
}

function atras {
	echo $0
	motores velderA velizqA
}

function derecha {
	echo $0
	motores vel_giro_der_i vel_giro_der_d
}

function izquierda {
	echo $0
	motores vel_giro_izq_i vel_giro_izq_d
}

function stop {
	echo $0
	motores 0 0
	motores 0 0
}




declare -i velizq
declare -i velder
declare -i velizqA	# vel izq atras
declare -i velderA	# vel der atras
declare -i vel_giro_izq_i	# vel giro izq 
declare -i vel_giro_izq_d	# vel giro der 
declare -i vel_giro_der_i	# vel giro izq 
declare -i vel_giro_der_d	# vel giro der 
velderA=127
velizqA=127-30

velder=128
velizq=128+30

vel_giro_der_i=127-90
vel_giro_der_d=128+90

vel_giro_izq_i=128+25+50
vel_giro_izq_d=127-25-50


while true; do 

	echo "teclas: b (stop), a,d (izq, der) w,s (adelante, atras), x salir"
read -n 1 c

echo $c

if [[ "$c" == "x" ]]; then
	exit
fi

if [[ "$c" == "a" ]]; then
	izquierda
elif [[ "$c" == "d" ]]; then
	derecha
elif [[ "$c" == "w" ]]; then
	adelante
elif [[ "$c" == "s" ]]; then
	atras
elif [[ "$c" == "b" ]]; then
	stop
fi

done
