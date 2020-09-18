#ifndef SPHEREH
#define SPHEREH

#include "material.h"

struct sphere
{
  vec3 center;
  float radius;
  struct material mat;
};

// sphere bounding box
// struct sphere sp;
// aabb(sp.center - vec3(sp.radius, sp.radius, sp.radius), sp.center + vec3(sp.radius, sp.radius, sp.radius));

float hit_sphere(struct sphere *s, ray *r, float t_min, float t_max, hit_record *rec) {
  vec3 oc = r->origin() - s->center;
  vec3 r_dir = r->direction();
  float a = dot(&r_dir, &r_dir);
  float b = dot(&oc, &r_dir);
  float c = dot(&oc, &oc) - s->radius * s->radius;
  float discriminant = b*b - a*c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(b * b - a * c)) / a;
    if (temp < t_max && temp > t_min) {
      rec->t = temp;
      rec->p = r->point_at_parameter(rec->t);
      rec->normal = (rec->p - s->center) / s->radius;
      return true;
    }

    temp = (-b + sqrt(b * b - a * c)) / a;

    if (temp < t_max && temp > t_min) {
      rec->t = temp;
      rec->p = r->point_at_parameter(rec->t);
      rec->normal = (rec->p - s->center) / s->radius;
      return true;
    }
  }

  return false;
}

// bool sphere_bounding_box(struct sphere *s, struct aabb *output_box) {
//   vec3 vmin = vec3(s->center - vec3(s->radius, s->radius, s->radius));
//   vec3 vmax = vec3(s->center + vec3(s->radius, s->radius, s->radius));
//   output_box->vmin = vmin;
//   output_box->vmax = vmax;
//   return true;
// }

#endif
