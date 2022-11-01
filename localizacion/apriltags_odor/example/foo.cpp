
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
	// Eigen::Vector3f eulerAngle;

	translation << t0, t1, t2;
	m << (-1)*m0, m1, m2, m3, (-1)*m4, (-1)*m5, (-1)*m6, m7, m8;

	// RAFA std::cout << "matrix:" << m << "\n";

	Eigen::Matrix3d F;
	F <<
		1,   0,  0,
		0,  -1,  0,
		0,   0,  1;

	Eigen::Matrix3d fixed_rot = F*m;
	double yaw, pitch, roll;
	// wRo_to_euler(m, yaw, pitch, roll);

	wRo_to_euler(fixed_rot, yaw, pitch, roll);

	// RAFA std::cout   << "  matrix2=" << fixed_rot << "\n";

	double distancia = (translation.norm());
        double x = (translation(0));
        double z = (translation(2));
        //double X = ((cos(pitch)*translation(0))-(sin(pitch)*translation(2)));
        //double Y = ((cos(pitch)*translation(1))-(sin(pitch)*translation(2)));
        //double Z = ((sin(pitch)*translation(0))+(cos(pitch)*translation(2)));
	double *XXX = (double *) malloc(sizeof(double));
	double *YYY = (double *) malloc(sizeof(double));

	//Agregado por ALE
	double  p = pitch*57.2958;
	double  y = yaw*57.2958;
	double  r = roll*57.2958;
	double  grado = asin(t0/distancia) * 57.2958;

	//rotacion((-1)*x, z, XXX, YYY, (pitch*57.2958));
	rotacion(x, z, XXX, YYY, pitch);
	//*XXX = (*XXX)*(-1); 
	int XXXX = *XXX;
	XXXX = XXXX*(-1) +100;

	// RAFA printf("\nXXX=%f,   YYY=%f\n", *XXX, *YYY);


	// RAFA std::cout   << "  yaw=" << (yaw*57.2958)  // convertido de radianes a grados
         // RAFA << ", pitch=" << (pitch*57.2958)
         // RAFA << ", roll=" << (roll*57.2958) << "\n";

	// RAFA std::cout << "  distance=" << (translation.norm()*100) // convertido a centimentros
         // RAFA << " cm, "
         // RAFA << "\n"
         // RAFA << "  x=" << (translation(0)*100)
         // RAFA << ", y=" << (translation(1)*100)
         // RAFA << ", z=" << (translation(2)*100)
         // RAFA << "\n";

	// *YYY *= (-1);
	// *XXX += 50;
	//printf("x=%i, y=%i, X'=%i, Y'=%i, X_TAG=%i, distancia=%i", (int) x, (int) z, (int) (*XXX)+100, (int) (*YYY)*(-1), (int) grado, (int) distancia);
	printf("x=%i, y=%i, X'=%i, Y'=%i, grado=%i, distancia=%i", (int) x, (int) z, (int) XXXX, (int) (*YYY)*(-1), (int) grado, (int) distancia);

	free(XXX);
	free(YYY);

}

} /* extern "C" */


