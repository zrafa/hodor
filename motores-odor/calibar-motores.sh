
DEV=/dev/ttyACM0
stty -F $DEV speed 9600

declare -i vel
declare -i velder
vel=0
velder=0

while true; do 

	echo "teclas: q,a (motor izq) i,j (motor der), x salir"
read -n 1 c

echo $c

if [[ "$c" == "x" ]]; then
	exit
fi

if [[ "$c" == "q" ]]; then
	echo q igual a
	vel=vel+10
	if [[ vel -gt 120 ]]; then
		vel=120
	fi
	echo $vel $velder
elif [[ "$c" == "a" ]]; then
	echo a igual a
	vel=vel-10
	if [[ vel -lt 0 ]]; then
		vel=0
	fi
	echo $vel $velder
elif [[ "$c" == "i" ]]; then
	echo i igual a
	velder=velder+10
	if [[ velder -gt 120 ]]; then
		velder=120
	fi
	echo $vel $velder
elif [[ "$c" == "j" ]]; then
	echo j igual a
	velder=velder-10
	if [[ velder -lt 0 ]]; then
		velder=0
	fi
	echo $vel $velder
fi

# Horrible hack para evitar que envie un 0a (enter)
# 0a da problemas
if [[ vel -eq 10 ]]; then
	vel=11
fi
if [[ velder -eq 10 ]]; then
	velder=11
fi

V=`echo -n "$vel" | awk '{printf "%02x", $1}'`
VDER=`echo -n "$velder" | awk '{printf "%02x", $1}'`

echo "$V $VDER"
echo -en X"\x$V\x$VDER" >> $DEV
echo "presione x para salir"
done
