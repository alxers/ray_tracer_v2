// Freebsd
// cc x11.c -I /usr/local/include -L /usr/local/lib/ -l X11

// Linux
// cc x11.cpp -I /usr/local/include -L /usr/lib/x86_64-linux-gnu/X11 -l X11 -lm -lstdc++ -ggdb

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ray.h"
#include "hittable.h"
#include "sphere.h"

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

// Ray helper functions


vec3 color(const ray& r, struct sphere *spheres, size_t arr_size) {
  hit_record rec;
  hit_record temp_rec;
  float t_min = 0.0;
  float t_max = MAXFLOAT;

  bool hit_anything = false;
  double closest_so_far = t_max;
  for (int i = 0; i < arr_size; i++) {
    if (hit_sphere(&spheres[i], r, t_min, closest_so_far, &temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  if (hit_anything) {
    return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
  }
}


unsigned int w_width = 400;
unsigned int w_height = 200;

// Ray setup
vec3 lower_left_corner(-2.0, -1.0, -1.0);
vec3 horizontal(4.0, 0.0, 0.0);
vec3 vertical(0.0, 2.0, 0.0);
vec3 origin = vec3(0.0, 0.0, 0.0);
// end Ray setup

// Xlib variables
Display *disp;
Window win;
int screen;
// End Xlib variables

void draw() {
  struct sphere sp1 = { vec3(0, 0, -1), 0.5 };
  struct sphere sp2 = { vec3(0, -100.5, -1), 100 };

  struct sphere spheres[] = { sp1, sp2 };
  size_t arr_size = 2;

  for (int j = w_height - 1; j >= 0; --j) {
    for (int i = 0; i < w_width; ++i) {
      float u = float(i) / float(w_width - 1);
      float v = float(j) / float(w_height - 1);
      ray r(origin, lower_left_corner + u * horizontal + v * vertical);

      vec3 col = color(r, spheres, arr_size);

      int ir = int(255.999 * col[0]);
      int ig = int(255.999 * col[1]);
      int ib = int(255.999 * col[2]);

      // Set colors
      XSetForeground(disp, DefaultGC(disp, screen), _rgb(ir, ig, ib));
      // Actually draw a pixel at (i, j) coords
      // TODO: check if XDrawPoints() will be faster
      // in that case prepare array of points

      // We use "height - j" because otherwise the image would be flipped vertically
      XDrawPoint(disp, win, DefaultGC(disp, screen), i, w_height - j);
    }
  }
}

// end Ray helper functions

int main(void) {
  // X window setup
  XEvent event;

  disp = XOpenDisplay(NULL);
  if (disp == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  screen = DefaultScreen(disp);
  win = XCreateSimpleWindow(disp, RootWindow(disp, screen), 0, 0, w_width, w_height, 0,
                           BlackPixel(disp, screen), WhitePixel(disp, screen));
  XSelectInput(disp, win, ExposureMask | KeyPressMask);
  XMapWindow(disp, win);
  // end X window setup
  
  while (1) {
    XNextEvent(disp, &event);
    if (event.type == Expose) {
      draw();
    }
     if (event.type == KeyPress) {
      // We'll move camera depending on a key pressed
      if (event.xkey.keycode == W_KEY) {
        // Change the view and draw everything again
        // NOTE: just change the origin for now
        // TODO: make the actuall correct camera rotation
        // origin = vec3(-1.0, 1.5, 0.0);
   
        // draw();
        XSetForeground(disp, DefaultGC(disp, screen), _rgb(255, 1, 127));
        XDrawPoint(disp, win, DefaultGC(disp, screen), 5, 5);
      }

      if (event.xkey.keycode == Q_KEY) {
        break;
      }
    }
  }
 
  XCloseDisplay(disp);
  return 0;
}
