/* config.h: configuracion para v4l2 y para apriltag_demo camera localization */

//#define FRAME_HEIGHT 120
//#define FRAME_WIDTH 160

/* para v4l2.c */
//1080 1920
#define FRAME_HEIGHT 720 
#define FRAME_WIDTH  1280 
#define FRAME_INTERVAL (1000/30)
#define PACK_SIZE 4096 //udp pack size; note that OSX limits < 8100 bytes
#define ENCODE_QUALITY 80

/* para apriltag_demo camera localization */
#define CAM_WIDTH 1280	/* lo obtuvo Ale */
#define CAM_HEIGHT 720	/* lo obtuvo Ale */

// Logitech c505 es 1415 o 1416
#define FOCAL_FX 1415	/* lo obtuvo Ale 816 */
#define FOCAL_FY 1415	/* lo obtuvo Ale 816 */



//#define FOCAL_FX 390	/* lo obtuvo Ale 816 */
//#define FOCAL_FY 390	/* lo obtuvo Ale 816 */
//#define TAGSIZE 0.054	/* en centimetros */
//#define TAGSIZE 16.1	/* en centimetros */
#define TAGSIZE 20	/* en centimetros */
// #define TAGSIZE 5.4	/* en centimetros */
