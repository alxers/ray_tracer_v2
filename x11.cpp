// Freebsd
// cc x11.c -I /usr/local/include -L /usr/local/lib/ -l X11

// Linux
// cc x11.cpp -I /usr/local/include -L /usr/lib/x86_64-linux-gnu/X11 -l X11 -lm -lstdc++

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ray.h"
#include "box.c"

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

bool hit_sphere(const vec3& center, float radius, const ray& r) {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;
  float discriminant = b*b - 4*a*c;
  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-b - sqrt(discriminant) / (2.0 * a));
  }
}

vec3 color(const ray& r) {
  // float t = hit_sphere(vec3(0,0,-1), 0.5, r);
  // if (t > 0.0) {
  //   vec3 n = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
  //   return 0.5 * vec3(n.x() + 1, n.y() + 1, n.z() + 1);
  // }

  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
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
  for (int j = w_height - 1; j >= 0; --j) {
    for (int i = 0; i < w_width; ++i) {
      float u = float(i) / float(w_width - 1);
      float v = float(j) / float(w_height - 1);
      // ray r(origin, lower_left_corner + u * horizontal + v * vertical);

      // vec3 col = color(r);

      // int ir = int(255.999 * col[0]);
      // int ig = int(255.999 * col[1]);
      // int ib = int(255.999 * col[2]);

      float r = float(i) / float(w_width -1);
      float g = float(j) / float(w_height -1);
      float b = 0.2;
      int ir = int(255.999 * r);
      int ig = int(255.999 * g);
      int ib = int(255.999 * b);

      // Set colors
      XSetForeground(disp, DefaultGC(disp, screen), _rgb(ir, ig, ib));
      // Actually draw a pixel at (i, j) coords
      // TODO: check if XDrawPoints() will be faster
      // in that case prepare array of points
      XDrawPoint(disp, win, DefaultGC(disp, screen), i, j);
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
        origin = vec3(-1.0, 1.5, 0.0);
   
        draw();
      }

      if (event.xkey.keycode == Q_KEY) {
        break;
      }
    }
  }
 
  XCloseDisplay(disp);
  return 0;
}
