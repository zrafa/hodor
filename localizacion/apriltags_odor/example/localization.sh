#!/bin/sh

VAR=`./apriltag_demo -q | grep -v XXX | grep X`
X=`echo $VAR | sed -e "s/,.*//" -e "s/X=//"`
Y=`echo $VAR | sed -e "s/.*, //" -e "s/Y=//"`
clear
echo "**********"
echo $X
echo $Y
XX=$X
X=$((25-(X/4)))
echo "|--------------------------------------------------|"
for i in `seq 1 $X` ; do printf "\n" ; done
for i in `seq 1 $Y` ; do printf " " ; done
echo  "X"
for i in `seq $X 25` ; do printf "\n" ; done
echo "|--------------------------------------------------|"


X=$XX
X=$((90-X))
# echo $X
Y=$((Y/20))

X=$((X/22))
# echo $X

COLOR="AZUL"
if [ $Y -eq 1 ] || [ $Y -eq 3 ] ; then
	X=$((X+4))
fi
if [ $Y -gt 1 ] ; then
	COLOR="VERDE"
fi

echo $X $COLOR




# coreutils-stty
# stty -F /dev/ttyATH0 
# stty -F /dev/ttyATH0 speed 9600
# stty -F /dev/ttyATH0 
# speed 9600 baud; line = 0;
# root@OpenWrt:~# echo A > /dev/ttyATH0 
# root@OpenWrt:~# printf "A" > /dev/ttyATH0
# root@OpenWrt:~# while read a ; do echo $a ; done < /dev/ttyATH0 

