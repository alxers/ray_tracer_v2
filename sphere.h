#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"

struct sphere
{
  vec3 center;
  float radius;
};

float hit_sphere(struct sphere *s, const ray& r, float t_min, float t_max, hit_record *rec) {
  vec3 oc = r.origin() - s->center;
  float a = dot(r.direction(), r.direction());
  float b = 2.0 * dot(oc, r.direction());
  float c = dot(oc, oc) - s->radius * s->radius;
  float discriminant = b*b - 4*a*c;
  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-b - sqrt(discriminant)) / (2.0 * a);
  }
}

#endif
