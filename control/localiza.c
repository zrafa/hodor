
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 

#define N 5



int calcular_orientacion_global(int angulo_servo, int hipotenusa, int cateto, int x)
{
	double PI = 3.14159265358979323846;
	double coseno_alfa = (double) cateto / (double) hipotenusa;
	double radianes = acos(coseno_alfa);
	int angulo_triang_rectangulo = radianes * (double) 180 / PI;
	int angulo_global;
	double theta;
	/* alfa = angulo del servo - 180 */
	/* beta = angulo_triang_rectangulo */
	if (x<390) 
		theta = (angulo_servo - 180 - angulo_triang_rectangulo);
	else
		theta = (angulo_servo - 180 + angulo_triang_rectangulo);

	angulo_global = 360 - theta;
	return angulo_global;
}


int main(int argc, char *argv[])
{
 
	int x2,y2;
	int angulo_tag, servo_angulo, distancia, orientacion_global;

 
	int xa, ya, xb, yb;
        int x,y;
	int grados;
	int dist;

 char *line = NULL;
	line = malloc(80);
 //char line[80];

	int i;
	xa = 0; ya = 0; xb = 0; yb = 0;

	for (i=0; i<N; i++) {
  size_t len = 0;
  ssize_t lineSize = 0;


     

   lineSize = getline(&line, &len, stdin);

	if (strlen(line) < 8) exit(0);



	*(line+3)=0;
	x2=atoi(line);
	*(line+7)=0;
	y2=atoi((line+4));

	*(line+19)=0;
	angulo_tag=atoi((line+15));

	*(line+35)=0;
	distancia=atoi((line+31));

	*(line+53)=0;
	servo_angulo=atoi((line+50));
	
	orientacion_global = calcular_orientacion_global(servo_angulo+angulo_tag, distancia, y2, (800-x2));


        double x1, y1;
        double PI=3.1415926535;
	int r = 30; /* longitud de vector orientacion */
	int angle = orientacion_global - 90;

            x1 = r * cos(angle * PI / 180);
            y1 = r * sin(angle * PI / 180);
	xa = xa + 800 - x2;
	ya = ya + y2;
	xb = xb + (int)(800-x2)+(int)x1;
	yb = yb + (int)y2+(int)y1;

	
	}	/* end for */

	xa = xa / N;
	ya = ya / N;
	xb = xb / N;
	yb = yb / N;
	
	// printf("%i %i %i %i ", 800-x2, y2, (int)(800-x2)+(int)x1, (int)y2+(int)y1);
	printf("%i %i %i %i ", xa, ya, xb, yb);


 
 
	  free(line);
 
    return 0;
}
