// Freebsd
// cc x11.c -I /usr/local/include -L /usr/local/lib/ -l X11

// Linux
// cc x11.cpp -I /usr/local/include -L /usr/lib/x86_64-linux-gnu/X11 -l X11 -lm -lstdc++// In freebsd

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

// Ray helper functions
struct box3
{
    // min and max bounds
    vec3 vmin;
    vec3 vmax;
};

// test box
vec3 min(-0.5, -0.5, -2.0);
vec3 max(0.5, 0.5, -3.0);
// box3 b;
box3 b = { min, max };

bool intersect(const ray &r) 
{
    float tmin = (min.x() - r.origin().x()) / r.direction().x(); 
    float tmax = (max.x() - r.origin().x()) / r.direction().x(); 
 
    if (tmin > tmax) std::swap(tmin, tmax); 
 
    float tymin = (min.y() - r.origin().y()) / r.direction().y(); 
    float tymax = (max.y() - r.origin().y()) / r.direction().y(); 
 
    if (tymin > tymax) std::swap(tymin, tymax); 
 
    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
 
    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 
 
    float tzmin = (min.z() - r.origin().z()) / r.direction().z(); 
    float tzmax = (max.z() - r.origin().z()) / r.direction().z(); 
 
    if (tzmin > tzmax) std::swap(tzmin, tzmax); 
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
 
    if (tzmin > tmin) 
        tmin = tzmin; 
 
    if (tzmax < tmax) 
        tmax = tzmax; 
 
    return true; 
} 

bool hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

// vec3 color(const ray& r) {
//  if (hit_sphere(vec3(0,0,-1), 0.5, r)) {
//         return vec3(1, 0, 0);
//  }

//  vec3 unit_direction = unit_vector(r.direction());
//  float t = 0.5*(unit_direction.y() + 1.0);
//  return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
// }

// Calculate normal for the cube
vec3 normal(box3 box, vec3 hit) {
    vec3 centerPoint(
        (box.vmin.x() + box.vmax.x()) * 0.5,
        (box.vmin.y() + box.vmax.y()) * 0.5,
        (box.vmin.z() + box.vmax.z()) * 0.5
    );

    vec3 p = hit - centerPoint;

    float dx = abs(box.vmin.x() - box.vmax.x())/2;
    float dy = abs(box.vmin.y() - box.vmax.y())/2;
    float dz = abs(box.vmin.z() - box.vmax.z())/2;

    vec3 n(
        (int) p.x()/dx,
        (int) p.y()/dy,
        (int) p.z()/dx
    );

    return unit_vector(n);
}

vec3 color(const ray& r) {

    vec3 n = normal(b, r.direction());

    if (intersect(r)) {
      if(n.y() > 0) {
            return vec3(1, 0.5, 0);
        } else {
            // The actual color in RGB
            return vec3(0, 1, 0);
        }
    }

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int w_height = 200;
int w_width = 300;

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
       for (int j = w_height - 1; j >= 0; j--) {
	 for (int i = 0; i < w_width; i++) {
	   float u = float(i) / float(w_width - 1);
	   float v = float(j) / float(w_height - 1);
	   ray r(origin, lower_left_corner + u * horizontal + v * vertical);
	   
	   vec3 col = color(r);
	   
	   int ir = int(255.999 * col[0]);
	   int ig = int(255.999 * col[1]);
	   int ib = int(255.999 * col[2]);

	   // Set colors
	   XSetForeground(disp, DefaultGC(disp, screen), _rgb(ir, ig, ib));
	   // Actually draw a pixel at (i, j) coords
	   XDrawPoint(disp, win, DefaultGC(disp, screen), i, j);
	  }
       }
}

// end Ray helper functions

int main(void) {
  // X window setup
  XEvent event;
  const char *msg = "Hello, World!";

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
	 // Change the view and draw everythin again
	 // NOTE: just change the origin for now
	 // TODO: make the actuall correct camera rotation
	 origin = vec3(-2.0, 2.0, 0.0);
	 
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
