// In freebsd
// cc x11.c -I /usr/local/include -L /usr/local/lib/ -l X11

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ray.h"

#define Q_KEY 0x18
#define W_KEY 0x19
#define A_KEY 0x26
#define S_KEY 0x27
#define D_KEY 0x28

#define UP_KEY 0x6f
#define DOWN_KEY 0x74
#define LEFT_KEY 0x71
#define RIGHT_KEY 0x72

// Make mapping so that it's possible to use RGB in Xlib
unsigned long _rgb(int r, int g, int b) {
  return b + (g << 8) + (r << 16);
}

int main(void) {
  // X window setup
  Display *disp;
  Window win;
  XEvent event;
  const char *msg = "Hello, World!";
  int screen;
  KeySym key;
  char text[255]; // Buffer for KeyPress events

  int w_height = 640;
  int w_width = 480;

  disp = XOpenDisplay(NULL);
  if (disp == NULL) {
     fprintf(stderr, "Cannot open display\n");
     exit(1);
  }

  screen = DefaultScreen(disp);
  win = XCreateSimpleWindow(disp, RootWindow(disp, screen), w_height, w_width, 100, 100, 1,
                           BlackPixel(disp, screen), WhitePixel(disp, screen));
  XSelectInput(disp, win, ExposureMask | KeyPressMask);
  XMapWindow(disp, win);
  // end X window setup

  // Ray setup
  vec3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(-2.0, 2.0, 0.0);
  // end Ray setup

  // for (int j = w_height - 1; j >= 0; j--) {
  //   for (int i = 0; i < w_width; i++) {
  //     float r = float(j) / float(w_width - 1);
  //     float g = float(i) / float(w_height - 1);
  //     float b = 0.25;

  //     int ir = int(255.999 * r);
  //     int ig = int(255.999 * g);
  //     int ib = int(255.999 * b);
  //   }
  // }
  
  while (1) {
     XNextEvent(disp, &event);
     if (event.type == Expose) {

       for (int j = w_height - 1; j >= 0; j--) {
	 for (int i = 0; i < w_width; i++) {
	   float r = float(j) / float(w_width - 1);
	   float g = float(i) / float(w_height - 1);
	   float b = 0.25;

	   int ir = int(255.999 * r);
	   int ig = int(255.999 * g);
	   int ib = int(255.999 * b);

	   // Set colors
	   XSetForeground(disp, DefaultGC(disp, screen), _rgb(ir, ig, ib));
	   // Actually draw a pixel at (i, j) coords
	   XDrawPoint(disp, win, DefaultGC(disp, screen), i, j);
	  }
       }
     }
     if (event.type == KeyPress) {
       //printf("Key: %x\n", event.xkey.keycode);
       if (event.xkey.keycode == W_KEY) {
	 XSetForeground(disp, DefaultGC(disp, screen), _rgb(0, 0, 0));
	 XFillRectangle(disp, win, DefaultGC(disp, screen), 30, 30, 10, 10);
       }

       if (event.xkey.keycode == Q_KEY) {
	 break;
       }
       //XSetForeground(disp, DefaultGC(disp, screen), _rgb(255, 0, 127));
       //XFillRectangle(disp, win, DefaultGC(disp, screen), 30, 30, 10, 10);
     }
  }
 
  XCloseDisplay(disp);
  return 0;
}
