
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>
 




enum colores {rojo, verde, azul};

void dot(SDL_Renderer* rend, int x, int y, enum colores c);

// DDA Function for line generation 
void draw_line(SDL_Renderer *rend, int X0, int Y0, int X1, int Y1) 
{ 
    // calculate dx & dy 
    int dx = X1 - X0; 
    int dy = Y1 - Y0; 
  
    // calculate steps required for generating pixels 
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy); 
  
    // calculate increment in x & y for each steps 
    float Xinc = dx / (float)steps; 
    float Yinc = dy / (float)steps; 
  
    // Put pixel for each step 
    float X = X0; 
    float Y = Y0; 
    for (int i = 0; i <= steps; i++) { 
  	dot(rend, (int) X, (int) Y, azul);
        //putpixel(round(X), round(Y), 
                 //RED); // put pixel at (X,Y) 
        X += Xinc; // increment in x at each step 
        Y += Yinc; // increment in y at each step 
    } 
} 


void mostrar_orientacion(SDL_Renderer* rend, int x, int y, int angle, int r)
{
	double x1, y1;
	double PI=3.1415926535;

            x1 = r * cos(angle * PI / 180);
            y1 = r * sin(angle * PI / 180);
		draw_line(rend, x, y, x+x1, y+y1);
		draw_line(rend, x+1, y, x+1+x1, y+y1);
		draw_line(rend, x-1, y, x-1+x1, y+y1);
		draw_line(rend, x, y+1, x+x1, y+1+y1);
		draw_line(rend, x, y-1, x+x1, y-1+y1);
}

void dot(SDL_Renderer* rend, int x, int y, enum colores c)
{
	SDL_Rect r;
    	r.x = x;
    	r.y = y;
    	r.w = 5;	/* tamaño del dot (punto) */
    	r.h = 5;

	if (c == rojo)
		SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
	else if (c == verde)
		SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
	else /* if (c == azul) */
		SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);

	SDL_RenderFillRect(rend, &r );

}

void cruz (SDL_Renderer* rend, int x, int y, enum colores c) 
{
	dot(rend, x, y, rojo);
	dot(rend, x-1, y, rojo);
	dot(rend, x-2, y, rojo);
	dot(rend, x+1, y, rojo);
	dot(rend, x+2, y, rojo);
	dot(rend, x, y-1, rojo);
	dot(rend, x, y-2, rojo);
	dot(rend, x, y+1, rojo);
	dot(rend, x, y+2, rojo);


}

/* posicion en pixeles del valor 5 en x */
int cen_x = 563;
int cen_y = 507;

void dibujar_obstaculo(SDL_Renderer* rend, int angulo, int radio)
{
	static const double PI = 3.1415926535;
	double x1, y1;

	radio = radio * 92 /100; /* 92 pixeles es el ancho de un cuadrado en la cuadricula */
	angulo = (-1) * (angulo-180); /* invertimos izq y derecha para que el dibujo coincida */

	x1 = radio * cos(angulo * PI / 180);
	y1 = radio * sin(angulo * PI / 180);
	dot(rend, cen_x - x1, cen_y - y1, rojo);
}


int calcular_orientacion_global(int angulo_servo, int hipotenusa, int cateto, int x)
{
	double PI = 3.14159265358979323846;
	double coseno_alfa = (double) cateto / (double) hipotenusa;
	double radianes = acos(coseno_alfa);
	int angulo_triang_rectangulo = radianes * (double) 180 / PI;
	int angulo_global;
	if (x<390) 
		angulo_global = 360 - (angulo_servo - 180 - angulo_triang_rectangulo);
	else
		angulo_global = 360 - (angulo_servo - 180 + angulo_triang_rectangulo);
	return angulo_global;
}


int main(int argc, char *argv[])
{
 
	int x2,y2;
	int angulo_tag, servo_angulo, distancia, orientacion_global;

    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("Lidar Lite sobre SERVO DEMO GRAFICO", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 618, 0);
                                       // 1000, 1000, 0);
 
    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
 
    // creates a surface to load an image into the main memory
    SDL_Surface* surface;
 
    // please provide a path for your image
    surface = IMG_Load("axis3.bmp");
 
    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
 
    // clears main-memory
    SDL_FreeSurface(surface);
 
    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;
 
    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
 
    // adjust height and width of our image box.
    dest.w /= 2;
    dest.h /= 2;
 
    // sets initial x-position of object
    dest.x = (1000 - dest.w) / 2;
 
    // sets initial y-position of object
    dest.y = (1000 - dest.h) / 2;
 
    // controls annimation loop
    int close = 0;
 
    // speed of box
    int speed = 300;
 
	double frotate;
        int x,y;
	SDL_GetRendererOutputSize(rend, &x, &y);
	SDL_Rect r;
    	r.x = x/2;
    	r.y = 0;
    	r.w = 10;
    	r.h = 10;
	
	int grados;
	int dist;

 char *line = NULL;
	line = malloc(80);
 //char line[80];
  size_t len = 0;
  ssize_t lineSize = 0;


     
    // annimation loop
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
 
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    dest.y -= speed / 30;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    dest.x -= speed / 30;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    dest.y += speed / 30;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dest.x += speed / 30;
                    break;
                default:
                    break;
                }
            }
        }
 
        // right boundary
        if (dest.x + dest.w > 1000)
            dest.x = 1000 - dest.w;
 
        // left boundary
        if (dest.x < 0)
            dest.x = 0;
 
        // bottom boundary
        if (dest.y + dest.h > 1000)
            dest.y = 1000 - dest.h;
 
        // upper boundary
        if (dest.y < 0)
            dest.y = 0;
 
  	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        // clears the screen
        // SDL_RenderClear(rend);

   lineSize = getline(&line, &len, stdin);

	if (strlen(line) < 8) continue;



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
	printf("x2=%i orientacion global (grados) = %i \n", x2, orientacion_global);

	frotate=0.02;
	printf("%f grados:%i, distancia:%i, servo angulo:%i\n", frotate, angulo_tag, distancia, servo_angulo);

		SDL_RenderCopyEx(rend, tex, NULL, NULL, frotate, NULL, SDL_FLIP_NONE);
		mostrar_orientacion(rend, 800-x2,y2, orientacion_global-90, 30);
	cruz(rend, 800-x2, y2, rojo);


	printf("x: %i, y: %i \n", 800-x2, y2);
 
        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
 
        // calculates to 60 fps
        // SDL_Delay(1000 / 60);
    }
 
  free(line);
    // destroy texture
    SDL_DestroyTexture(tex);
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
 
    return 0;
}
