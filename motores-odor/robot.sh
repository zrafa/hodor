
DEV=/dev/ttyACM0
stty -F $DEV speed 9600


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
	motores velizq velder
}

function atras {
	echo $0
	motores velizqA velderA
}

function izquierda {
	echo $0
	motores velizqA velder
}

function derecha {
	echo $0
	motores velizq velderA
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
velizq=30
velder=30
velizqA=(128-velizq)+128
if [[ velizqA -lt 128 ]]; then
	velizqA=128
elif [[ velizqA -lt 255 ]]; then
	velizqA=255
fi

velderA=(128-velder)+128
if [[ velderA -lt 128 ]]; then
	velderA=128
elif [[ velderA -lt 255 ]]; then
	velderA=255
fi

while true; do 

	echo "teclas: a,s (izq, der) i,j (adelante, atras), x salir"
read -n 1 c

echo $c

if [[ "$c" == "x" ]]; then
	exit
fi

if [[ "$c" == "a" ]]; then
	izquierda
elif [[ "$c" == "s" ]]; then
	derecha
elif [[ "$c" == "i" ]]; then
	adelante
elif [[ "$c" == "j" ]]; then
	atras
elif [[ "$c" == "b" ]]; then
	stop
fi

done
