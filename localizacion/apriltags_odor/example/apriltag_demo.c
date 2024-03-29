/* Copyright (C) 2013-2016, The Regents of The University of Michigan.
All rights reserved.

This software was developed in the APRIL Robotics Lab under the
direction of Edwin Olson, ebolson@umich.edu. This software may be
available under alternative licensing terms; contact the address above.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the Regents of The University of Michigan.
*/

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

#include "apriltag.h"
#include "tag36h11.h"
/* RAFA: quitamos todos estas variantes que no utilizaremos
#include "tag25h9.h"
#include "tag16h5.h"
#include "tagCircle21h7.h"
#include "tagCircle49h12.h"
#include "tagCustom48h12.h"
#include "tagStandard41h12.h"
#include "tagStandard52h13.h"
*/

#include "common/getopt.h"
#include "common/image_u8.h"
#include "common/image_u8x4.h"
#include "common/pjpeg.h"
#include "common/zarray.h"


//RAFA
#include "common/homography.h"
#include "foo.h"
#include "config.h"
#include "v4l2.h"



int servo_get_angulo(void)
{
  int angulo;
  FILE* ptr;
  char str[50];

  /* Open the command for reading. */

  /* enviamos la solicitud de angulo */
  system("echo g > /dev/ttyUSB0");

  ptr = fopen("/dev/ttyUSB0", "r");

  if (NULL == ptr) {
        printf("file can't be opened \n");
  }

  int i = 0;
  str[i]=fgetc(ptr);
  while ( str[i] != '\n') {
	i++;
    	str[i]=fgetc(ptr);
  }
  str[i]='\0';

  fclose(ptr);

  angulo = atoi(str);
  return angulo;

}


int servo_i = 40;
int primer = 1;
void servo_buscar(void)
{
  int angulo;
      FILE* ptr;
    char str[50];

  /* Open the command for reading. */

    /*
	system("echo g > /dev/ttyUSB0");
    ptr = fopen("/dev/ttyUSB0", "r");

    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

    int i = 0;
    str[i]=fgetc(ptr);
    while ( str[i] != '\n') {
	i++;
    	str[i]=fgetc(ptr);
    }
    str[i]='\0';

    fclose(ptr);

    angulo = atoi(str);
    */
    angulo = servo_get_angulo();
    


    //printf("angulo : %i\n", angulo);
    if (angulo > 300)
	    servo_i = -40;
    else if ((angulo <= 40) && (angulo > 0))
	    servo_i = 40;

    if (angulo != 0) {
    if (servo_i == -40)
	system("echo d40 > /dev/ttyUSB0");
    else
	system("echo i40 > /dev/ttyUSB0");
    }
	
    if (angulo == 0)
	    system("echo i5 > /dev/ttyUSB0");
    //sleep(1);

  /* close */
}	

// Invoke:
//
// tagtest [options] input.pnm

int main(int argc, char *argv[])
{
	int unico = 0;
	int chance = 0;

    getopt_t *getopt = getopt_create();

    getopt_add_bool(getopt, 'h', "help", 0, "Show this help");
    getopt_add_bool(getopt, 'd', "debug", 0, "Enable debugging output (slow)");
    getopt_add_bool(getopt, 'q', "quiet", 0, "Reduce output");
    getopt_add_string(getopt, 'f', "family", "tag36h11", "Tag family to use");
    getopt_add_int(getopt, 'i', "iters", "1", "Repeat processing on input set this many times");
    getopt_add_int(getopt, 't', "threads", "1", "Use this many CPU threads");
    getopt_add_int(getopt, 'a', "hamming", "1", "Detect tags with up to this many bit errors.");
    getopt_add_double(getopt, 'x', "decimate", "2.0", "Decimate input image by this factor");
    getopt_add_double(getopt, 'b', "blur", "0.0", "Apply low-pass blur to input; negative sharpens");
    getopt_add_bool(getopt, '0', "refine-edges", 1, "Spend more time trying to align edges of tags");

    if (!getopt_parse(getopt, argc, argv, 1) || getopt_get_bool(getopt, "help")) {
        printf("Usage: %s [options] <input files>\n", argv[0]);
        getopt_do_usage(getopt);
        exit(0);
    }

    const zarray_t *inputs = getopt_get_extra_args(getopt);

    apriltag_family_t *tf = NULL;
    const char *famname = getopt_get_string(getopt, "family");
    if (!strcmp(famname, "tag36h11")) {
        tf = tag36h11_create();
/*
    } else if (!strcmp(famname, "tag25h9")) {
        tf = tag25h9_create();
    } else if (!strcmp(famname, "tag16h5")) {
        tf = tag16h5_create();
    } else if (!strcmp(famname, "tagCircle21h7")) {
        tf = tagCircle21h7_create();
    } else if (!strcmp(famname, "tagCircle49h12")) {
        tf = tagCircle49h12_create();
    } else if (!strcmp(famname, "tagStandard41h12")) {
        tf = tagStandard41h12_create();
    } else if (!strcmp(famname, "tagStandard52h13")) {
        tf = tagStandard52h13_create();
    } else if (!strcmp(famname, "tagCustom48h12")) {
        tf = tagCustom48h12_create();
*/
    } else {
        printf("Unrecognized tag family name. Use e.g. \"tag36h11\".\n");
        exit(-1);
    }

    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family_bits(td, tf, getopt_get_int(getopt, "hamming"));
    td->quad_decimate = getopt_get_double(getopt, "decimate");
    td->quad_sigma = getopt_get_double(getopt, "blur");
    td->nthreads = getopt_get_int(getopt, "threads");
    td->debug = getopt_get_bool(getopt, "debug");
    td->refine_edges = getopt_get_bool(getopt, "refine-edges");

    int quiet = getopt_get_bool(getopt, "quiet");

    int maxiters = getopt_get_int(getopt, "iters");

    const int hamm_hist_max = 10;


    int c;
    char b;
    // RAFA video
	v4l2_init();
    // RAFA
    // RAFA while (1) {
    
//	double focal_rafa = 1400;
//
	int i;
	
    while(1) {
// RAFA  	c = 0;
// RAFA 	while ( c != 1 )
// RAFA 		c = scanf("%c", &b, 1);

	mainloop();

	int size_yuv_to_jpeg;


	int err = 0;

	size_yuv_to_jpeg = YUVtoJPEG();

	pjpeg_t *pjpeg = pjpeg_create_from_buffer(jpegbuffer_start, size_yuv_to_jpeg, 0, &err);
	// QUITAR rawWrite(jpegbuffer_start, size_yuv_to_jpeg, "salida2");
	rawWrite(jpegbuffer_start, size_yuv_to_jpeg, "/tmp/foto.jpg");
	// pjpeg_t *pjpeg = pjpeg_create_from_buffer(buffer_start, jpegsize(), 0, &err);
	if (pjpeg == NULL) {
		printf("pjpeg error %d\n", err);
		// continue;
		exit(1);
	}

	image_u8_t *im = NULL;
	im = pjpeg_to_u8_baseline(pjpeg);

    for (int iter = 0; iter < maxiters; iter++) {

        int total_quads = 0;
        int total_hamm_hist[hamm_hist_max];
        memset(total_hamm_hist, 0, sizeof(total_hamm_hist));
        double total_time = 0;

        if (maxiters > 1)
            printf("iter %d / %d\n", iter + 1, maxiters);

        for (int input = 0; input < zarray_size(inputs); input++) {

            int hamm_hist[hamm_hist_max];
            memset(hamm_hist, 0, sizeof(hamm_hist));

/* RAFA: quito esta porcion de codigo ya que la foto la obtenemos 
 *       desde la camara
 *
            char *path;
            zarray_get(inputs, input, &path);
            if (!quiet)
                printf("loading %s\n", path);
            else
                printf("%20s ", path);

            image_u8_t *im = NULL;
            if (str_ends_with(path, "pnm") || str_ends_with(path, "PNM") ||
                str_ends_with(path, "pgm") || str_ends_with(path, "PGM"))
                im = image_u8_create_from_pnm(path);
            else if (str_ends_with(path, "jpg") || str_ends_with(path, "JPG")) {
                int err = 0;
                pjpeg_t *pjpeg = pjpeg_create_from_file(path, 0, &err);
                if (pjpeg == NULL) {
                    printf("pjpeg error %d\n", err);
                    continue;
                }

                if (1) {
                    im = pjpeg_to_u8_baseline(pjpeg);
                } else {
                    printf("illumination invariant\n");

                    image_u8x3_t *imc =  pjpeg_to_u8x3_baseline(pjpeg);

                    im = image_u8_create(imc->width, imc->height);

                    for (int y = 0; y < imc->height; y++) {
                        for (int x = 0; x < imc->width; x++) {
                            double r = imc->buf[y*imc->stride + 3*x + 0] / 255.0;
                            double g = imc->buf[y*imc->stride + 3*x + 1] / 255.0;
                            double b = imc->buf[y*imc->stride + 3*x + 2] / 255.0;

                            double alpha = 0.42;
                            double v = 0.5 + log(g) - alpha*log(b) - (1-alpha)*log(r);
                            int iv = v * 255;
                            if (iv < 0)
                                iv = 0;
                            if (iv > 255)
                                iv = 255;

                            im->buf[y*im->stride + x] = iv;
                        }
                    }
                    image_u8x3_destroy(imc);
                    if (td->debug)
                        image_u8_write_pnm(im, "debug_invariant.pnm");
                }

                pjpeg_destroy(pjpeg);
            }
*/


		}

		// RAFA AGREGO
                pjpeg_destroy(pjpeg);

            if (im == NULL) {
                printf("couldn't load \n");
		exit(1);
                //printf("couldn't load %s\n", path);
                //continue;
            }

            zarray_t *detections = apriltag_detector_detect(td, im);

            for (i = 0; i < zarray_size(detections); i++) {
                apriltag_detection_t *det;
                zarray_get(detections, i, &det);

                if (!quiet)
                    printf("detection %3d: id (%2dx%2d)-%-4d, hamming %d, margin %8.3f\n",
                           i, det->family->nbits, det->family->h, det->id, det->hamming, det->decision_margin);

                // hamm_hist[det->hamming]++;
                // total_hamm_hist[det->hamming]++;


		// RAFA
		/* configuracion en config.h */
		// double width = FRAME_WIDTH;
		// double height = FRAME_HEIGHT;
		// double width = CAM_WIDTH;
		// double height = CAM_HEIGHT;
		double cx = FRAME_WIDTH/2;
		double cy = FRAME_HEIGHT/2;
		double fx = FOCAL_FX;
		double fy = FOCAL_FY;
		//double fx = focal_rafa;
		//double fy = focal_rafa;
		//focal_rafa++;
		//printf("  distancia-focal:%f \n", focal_rafa);
		printf("  \n");
		// double tagsize=0.0935;
		double tagsize=TAGSIZE;	 /* en centimetros */

		int j,k;

                matd_t *M = homography_to_pose(det->H, -fx, fy, cx, cy);
                double scale = tagsize / 2.0;

                MATD_EL(M, 0, 3) *= scale;
                MATD_EL(M, 1, 3) *= scale;
                MATD_EL(M, 2, 3) *= scale ;

/*
                for (k=0;k<=3;k++) {
                        for (j=0;j<=3;j++)
                                printf("[%i-%i]=%f ",k,j,MATD_EL(M,k,j));
                        printf("\n");
                }
*/


 		foo(MATD_EL(M,0,0), MATD_EL(M,0,1), MATD_EL(M,0,2), MATD_EL(M,1,0), MATD_EL(M,1,1), MATD_EL(M,1,2), MATD_EL(M,2,0), MATD_EL(M,2,1), MATD_EL(M,2,2),
            		MATD_EL(M,0,3), MATD_EL(M,1,3), MATD_EL(M,2,3));

		// FIN RAFA

		fflush(0);





        // RAFA printf("Summary 4\n");
            apriltag_detections_destroy(detections);

          //  if (!quiet) {
           //     timeprofile_display(td->tp);
            //}

            total_quads += td->nquads;

          //  if (!quiet)
           //     printf("hamm ");

            // RAFAfor (int i = 0; i < hamm_hist_max; i++)
                // RAFA printf("%5d ", hamm_hist[i]);

            double t =  timeprofile_total_utime(td->tp) / 1.0E3;
            total_time += t;
            // RAFA printf("%12.3f ", t);
            // RAFA printf("%5d", td->nquads);

            // RAFA printf("\n");

        // RAFA printf("Summary 3\n");
            // RAFA image_u8_destroy(im);
        }
	    if (i==0) {
		    unico = 1;
		    chance++;
		    if (chance > 2) {
		    	servo_buscar(); 
			chance = 0;
		    }
	    } else {
		    /*
		    if (unico) {
	    		if (servo_i == -40)
				system("echo i40 > /dev/ttyUSB0");
			else
				system("echo d40 > /dev/ttyUSB0");
		    	sleep(5);
		    	unico = 0;
		    }
		    */
		    //printf("i ES DISINTO! \n");
		    printf("servo_angulo=%03i \n", servo_get_angulo());
	    }

	    // system("echo i40 > /dev/ttyUSB0");
	    //if (i==0) system("echo i40 > /dev/ttyUSB0");


           // RAFA image_u8_destroy(im);
           image_u8_destroy(im);
        // RAFA printf("Summary\n");

        // RAFA printf("hamm ");

        // RAFA for (int i = 0; i < hamm_hist_max; i++)
        // RAFA     printf("%5d ", total_hamm_hist[i]);
        // RAFA printf("%12.3f ", total_time);
        // RAFA printf("%5d", total_quads);
        // RAFA printf("\n");

    }
  
  // RAFA printf("ANTES DE SALIR\n");
	fflush(0);
    }; 
  // RAFA } // RAFA fin del while (1) {

    // don't deallocate contents of inputs; those are the argv
    apriltag_detector_destroy(td);

    if (!strcmp(famname, "tag36h11")) {
        tag36h11_destroy(tf);
/*
    } else if (!strcmp(famname, "tag25h9")) {
        tag25h9_destroy(tf);
    } else if (!strcmp(famname, "tag16h5")) {
        tag16h5_destroy(tf);
    } else if (!strcmp(famname, "tagCircle21h7")) {
        tagCircle21h7_destroy(tf);
    } else if (!strcmp(famname, "tagCircle49h12")) {
        tagCircle49h12_destroy(tf);
    } else if (!strcmp(famname, "tagStandard41h12")) {
        tagStandard41h12_destroy(tf);
    } else if (!strcmp(famname, "tagStandard52h13")) {
        tagStandard52h13_destroy(tf);
    } else if (!strcmp(famname, "tagCustom48h12")) {
        tagCustom48h12_destroy(tf);
*/
    }

    getopt_destroy(getopt);

    return 0;
}
