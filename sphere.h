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
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - s->radius * s->radius;
  float discriminant = b*b - a*c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(b * b - a * c)) / a;
    if (temp < t_max && temp > t_min) {
      rec->t = temp;
      rec->p = r.point_at_parameter(rec->t);
      rec->normal = (rec->p - s->center) / s->radius;
      return true;
    }

    temp = (-b + sqrt(b * b - a * c)) / a;

    if (temp < t_max && temp > t_min) {
      rec->t = temp;
      rec->p = r.point_at_parameter(rec->t);
      rec->normal = (rec->p - s->center) / s->radius;
      return true;
    }
  }

  return false;
}

#endif
