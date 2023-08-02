
/* foo.cpp */

#include "foo.h"

#include <iostream>
// RAFA agregó la siguiente línea
#include <iostream>
#include <Eigen/Geometry>

#include <Eigen/Core>
#include <stdio.h>

using namespace Eigen;




extern "C" {




const double PI = 3.14159265358979323846;
const double TWOPI = 2.0*PI;

/**
 * Normalize angle to be within the interval [-pi,pi].
 */
inline double standardRad(double t) {
  if (t >= 0.) {
    t = fmod(t+PI, TWOPI) - PI;
  } else {
    t = fmod(t-PI, -TWOPI) + PI;
  }
  return t;
}



void wRo_to_euler(const Eigen::Matrix3d& wRo, double& yaw, double& pitch, double& roll) {
    yaw = standardRad(atan2(wRo(1,0), wRo(0,0)));
    double c = cos(yaw);
    double s = sin(yaw);
    pitch = standardRad(atan2(-wRo(2,0), wRo(0,0)*c + wRo(1,0)*s));
    roll  = standardRad(atan2(wRo(0,2)*s - wRo(1,2)*c, -wRo(0,1)*s + wRo(1,1)*c));
}



void rotacion(double x, double y, double *rx, double *ry, double angulo)
{
 //angulo = (PI*angulo)/180.0;
 *rx = x*cos(angulo) - y*sin(angulo);
 *ry = x*sin(angulo) + y*cos(angulo);
}


void foo(double m0, double m1, double m2, double m3, double m4, double m5, double m6, double m7, double m8, double t0, double t1, double t2) {

	float r_x =0, r_y=0, r_z=0;
	Eigen::Vector3d translation;
	Eigen::Matrix3d m;

	translation << t0, t1, t2;
	m << (-1)*m0, m1, m2, m3, (-1)*m4, (-1)*m5, (-1)*m6, m7, m8;


	Eigen::Matrix3d F;
	F <<
		1,   0,  0,
		0,  -1,  0,
		0,   0,  1;

	Eigen::Matrix3d fixed_rot = F*m;
	double yaw, pitch, roll;

	wRo_to_euler(fixed_rot, yaw, pitch, roll);


	double distancia = (translation.norm());
        double x = (translation(0));
        double z = (translation(2));

	double *XXX = (double *) malloc(sizeof(double));
	double *YYY = (double *) malloc(sizeof(double));

	//Agregado por ALE
	double  p = pitch*57.2958;
	double  y = yaw*57.2958;
	double  r = roll*57.2958;
	double  grado = asin(t0/distancia) * 57.2958;

	rotacion(x, z, XXX, YYY, pitch);
	int XXXX = *XXX;
	int YYYY = *YYY;
//	XXXX = XXXX*(-1) +100;
//	YYYY = YYYY*(-1) -100;

	printf("x=%i, y=%i, X'=%i, Y'=%i, x-lab=%i, grado=%i, distance=%i\n", (int) x, (int) z, (int) XXXX, (int) (*YYY)*(-1), XXXX+390, (int) grado, (int) distancia);
	printf("%03i,%03i, grado=%i, distancia=%i\n", XXXX+390, (int) (*YYY)*(-1), (int) grado, (int) distancia);
	int x_map;
	if (XXXX >= 0)
		x_map = 430 - XXXX;
	else
		x_map = 430 + (-1)*XXXX;

	int y_map = YYYY;


// //	printf("x=%i, y=%i, X'=%i, Y'=%i, grado=%i, distancia=%i", (int) x, (int) z, (int) XXXX, (int) (*YYY)*(-1), (int) grado, (int) distancia);
	
	// printf("x=%i, y=%i, x_map'=%i, y_map=%i, XXXX=%i, YYYY=%i, grado=%i, distancia=%i", (int) x, (int) z, x_map, y_map, XXXX, YYYY, (int) grado, (int) distancia);

	free(XXX);
	free(YYY);

}

} /* extern "C" */


