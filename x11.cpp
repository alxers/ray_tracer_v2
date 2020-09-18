// Freebsd
// cc x11.c -I /usr/local/include -L /usr/local/lib/ -l X11

// Linux
// cc x11.cpp -I /usr/local/include -L /usr/lib/x86_64-linux-gnu/X11 -l X11 -lm -lstdc++ -ggdb

#define Q_KEY 0x18
#define W_KEY 0x19
#define A_KEY 0x26
#define S_KEY 0x27
#define D_KEY 0x28
#define Z_KEY 0x34
#define X_KEY 0x35

#define UP_KEY 0x6f
#define DOWN_KEY 0x74
#define LEFT_KEY 0x71
#define RIGHT_KEY 0x72

#define M_PI 3.1415926535

#define SPHERE_OBJ 1
#define BOX_OBJ 2

#define XY 3
#define XZ 4
#define YZ 5

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ray.h"
#include "hittable.h"
#include "box.h"
#include "sphere.h"
#include "camera.h"

// Make mapping so that it's possible to use RGB in Xlib
unsigned long _rgb(int r, int g, int b) {
  return b + (g << 8) + (r << 16);
}

// Random number in 0 <= num < 1
float random_num() {
  // TODO: replace rand()
  // return rand() % ();
  return 0;
}

struct world {
  int spheres_count;
  int boxes_count;
  struct xy_rect *xy;
  struct xz_rect *xz;
  struct yz_rect *yz;
  struct sphere *spheres;
  struct aabb *boxes;
  // struct box *boxes;
};

// Ray helper functions

vec3 color(ray *r, struct world *scene, int depth) {
  hit_record rec;
  hit_record temp_rec;

  hit_record rec2;
  hit_record temp_rec2;
  float t_min = 0.0;
  float t_max = MAXFLOAT;
  struct material mat;

  // Guard from too deep recursion
  if (depth <= 0) {
    return vec3(0, 0, 0);
  }

  int hit_object = 0;
  double closest_so_far = t_max;
  vec3 box_norm = {};

  for (int i = 0; i < scene->spheres_count; i++) {
    if (hit_sphere(&scene->spheres[i], r, t_min, closest_so_far, &temp_rec)) {
      hit_object = SPHERE_OBJ;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
      mat = scene->spheres[i].mat;
    }
  }

  for (int i = 0; i < scene->boxes_count; i++) {
    // TODO: check why do we enter box_normal when row with box wasn't displayed
    // b box.h:195
    // or is it correct, but we enter here because of the box reflection in the sphere?
    if (aabb_hit(r, t_min, closest_so_far, &scene->boxes[i], &temp_rec2)) {
      hit_object = BOX_OBJ;
      closest_so_far = temp_rec2.t;
      rec2 = temp_rec2;
      // box_norm = box_normal(&scene->boxes[i], r->direction(), &rec2);
      // printf("%.6f %.6f %.6f\n", box_norm.x(), box_norm.y(), box_norm.z());
      mat = { 2, vec3(0.8, 0.3, 0.3) };
    }
  }
  // for (int i = 0; i < scene->boxes_count; i++) {
  //   struct box curr_box = scene->boxes[i];
  //   if (box_hit(r, t_min, closest_so_far, &temp_rec, &curr_box.xy, &curr_box.xz, &curr_box.yz)) {
  //     hit_object = BOX_OBJ;
  //     closest_so_far = temp_rec.t;
  //     rec = temp_rec;
  //     // box_norm = box_normal(&scene->boxes[i], r->direction());
  //     // printf("%.6f %.6f %.6f\n", box_norm.x(), box_norm.y(), box_norm.z());
  //     mat = { 2, vec3(0.8, 0.3, 0.3) };
  //   }
  // }

  if (hit_object == SPHERE_OBJ) {
    ray scattered;
    vec3 attenuation;
    if (mat.type == 1 && lambertian_scatter(r, &rec, &attenuation, &scattered, &mat)) {
      return attenuation * color(&scattered, scene, depth - 1);
    } else if (mat.type == 2 && metal_scatter(r, &rec, &attenuation, &scattered, &mat)) {
      return attenuation * color(&scattered, scene, depth - 1);
    }
  } else if (hit_object == BOX_OBJ) {
    ray scattered_box;
    vec3 attenuation_box;
    // TODO: Move rec to the geometry object itself!!!
    // metal_scatter(r, &rec, &attenuation, &scattered, &mat);
    // return attenuation_box * color(&scattered_box, scene, depth - 1);
    return vec3(rec2.normal.x(), rec2.normal.y(), rec2.normal.z());
  } else {
    vec3 unit_direction = unit_vector(r->direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
  }
}


int w_width = 400;
int w_height = 200;
// Off by default, makes rendering too slow
bool ANTIALIASING = false;
int samples_per_pixel = 50;
int max_depth = 30;

// Xlib variables
Display *disp;
Window win;
int screen;
// End Xlib variables

void draw(struct camera cam) {
  struct material mat1 = { 1, vec3(0.8, 0.3, 0.3) };
  struct material mat2 = { 1, vec3(0.8, 0.8, 0.0) };
  struct material mat3 = { 2, vec3(0.8, 0.6, 0.2) };
  struct material mat4 = { 2, vec3(0.8, 0.6, 0.8) };
  struct sphere sp1 = { vec3(0, 0, -1), 0.5, mat1 };
  struct sphere sp2 = { vec3(0, -100.5, -1), 100, mat2 };
  struct sphere sp3 = { vec3(1, 0, -1), 0.5, mat3 };
  struct sphere sp4 = { vec3(-1, 0, -1), 0.5, mat4 };

  struct sphere spheres[] = { sp1, sp2, sp3, sp4 };

  // struct aabb b1 = { vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5) };
  struct aabb b1 = { vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5) };
  struct aabb boxes[] = { b1 };

  // float x0 = -0.5;
  // float y0 = -0.5;
  // float z0 = -0.5;
  // float x1 = 0.5;
  // float y1 = 0.5;
  // float z1 = 0.5;
  // struct xy_rect xy = { x0, x1, y0, y1, z0 };
  // struct xz_rect xz = { x0, x1, z0, z1, y0 };
  // struct yz_rect yz = { y0, y1, z0, z1, x0 };
  // struct box b2 = {
  //   vec3(x0, y0, z0),
  //   vec3(x1, y1, z1),
  //   xy,
  //   xz,
  //   yz
  // };
  // struct box boxes[] = { b2 };

  // Create scene with geometry objects
  struct world scene;
  scene.spheres = spheres;
  scene.spheres_count = 4;
  scene.boxes = boxes;
  scene.boxes_count = 1;


  for (int j = w_height - 1; j >= 0; --j) {
    for (int i = 0; i < w_width; ++i) {

      vec3 col;

      if (ANTIALIASING) {
        col = vec3(0, 0, 0);
        for (int s = 0; s < samples_per_pixel; s++) {
          float u = float(i + drand48()) / float(w_width - 1);
          float v = float(j + drand48()) / float(w_height - 1);
          ray r = get_ray(u, v, cam);
          col += color(&r, &scene, max_depth);
        }

        col /= float(samples_per_pixel);
      } else {
        float u = float(i) / float(w_width - 1);
        float v = float(j) / float(w_height - 1);

        ray r = get_ray(u, v, cam);
        col = color(&r, &scene, max_depth);
        // Compensate gamma correctness?
        col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      }

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

  // Ray setup
  struct camera cam;
  // TODO: check if zoom in/out is the same as W/D
  int lfrom_x = -2;
  int lfrom_y = 2;
  int lfrom_z = 1;
  vec3 lookfrom = vec3(lfrom_x, lfrom_y, lfrom_z);
  vec3 lookat = vec3(0, 0, -1);
  vec3 vup = vec3(0, 1, 0);
  int vfov = 20;
  float a_ratio = float(w_width)/float(w_height);
  set_cam(lookfrom, lookat, vup, vfov, a_ratio, &cam);
  // end Ray setup
  
  while (1) {
    XNextEvent(disp, &event);
    if (event.type == Expose) {
      draw(cam);
    }
     if (event.type == KeyPress) {
      // printf("Key: %x\n", event.xkey.keycode);
      // We'll move camera depending on a key pressed
      if (event.xkey.keycode == W_KEY) {
        lfrom_y += 1;
        lookfrom = vec3(lfrom_x, lfrom_y, lfrom_z);
        set_cam(lookfrom, lookat, vup, vfov, a_ratio, &cam);
        draw(cam);
      }
      if (event.xkey.keycode == A_KEY) {
        lfrom_x -= 1;
        lookfrom = vec3(lfrom_x, lfrom_y, lfrom_z);
        set_cam(lookfrom, lookat, vup, vfov, a_ratio, &cam);
        draw(cam);
      }
      if (event.xkey.keycode == S_KEY) {
        lfrom_y -= 1;
        lookfrom = vec3(lfrom_x, lfrom_y, lfrom_z);
        set_cam(lookfrom, lookat, vup, vfov, a_ratio, &cam);
        draw(cam);
      }
      if (event.xkey.keycode == D_KEY) {
        lfrom_x += 1;
        lookfrom = vec3(lfrom_x, lfrom_y, lfrom_z);
        set_cam(lookfrom, lookat, vup, vfov, a_ratio, &cam);
        draw(cam);
      }
      if (event.xkey.keycode == Z_KEY) {
        lfrom_y -= 1;
        lookfrom = vec3(lfrom_x, lfrom_y, lfrom_z);
        set_cam(lookfrom, lookat, vup, vfov, a_ratio, &cam);
        draw(cam);
      }
      if (event.xkey.keycode == X_KEY) {
        lfrom_y += 1;
        lookfrom = vec3(lfrom_x, lfrom_y, lfrom_z);
        set_cam(lookfrom, lookat, vup, vfov, a_ratio, &cam);
        draw(cam);
      }
      // printf("%d %d %d\n", lfrom_x, lfrom_y, lfrom_z);

      if (event.xkey.keycode == Q_KEY) {
        break;
      }
    }
  }
 
  XCloseDisplay(disp);
  return 0;
}
