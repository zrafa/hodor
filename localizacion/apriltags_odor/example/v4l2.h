#ifndef _V4L2_H
#define _V4L2_H

void *buffer_start               = NULL;
void *jpegbuffer_start               = NULL;
void *jpegbuffer2_start               = NULL;
size_t length                    = 0;

long unsigned int YUYVtoJPEG(unsigned char* img, int width, int height, int jpegQuality, unsigned char* buf);
void send_frame(unsigned char * frame, int len);

// necessary to silent clang warning
static void errno_exit(const char *s) __attribute__((noreturn));

// void send_YUV();
long unsigned int YUVtoJPEG();
int jpegsize();
void send_MJPEG();
int read_frame();
void mainloop(void);
void stop_capturing(void);
void uninit_device(void);
void init_mmap(void);
void get_pixelformat();
void init_device(void);
void close_device(void);
void open_device(void);
void usage(FILE* fp, int argc, char** argv);
int v4l2_init();

#endif /* _V4L2_H */
