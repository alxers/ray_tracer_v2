#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

struct hit_record
{
  float t;
  vec3 p;
  vec3 normal;
};

// bool hit(ray &r, float t_min, float t_max, hit_record &rec) {
//   hit_record temp_rec;
//   bool hit_anything = false;
//   double closest_so_far = t_max;
//   for (int i = 0; i < world.size; i++) {
//     if (world[i].hit(r, t_min, closest_so_far, temp_rec)) {
//       hit_anything = true;
//       closest_so_far = temp_rec.t;
//       rec = temp_rec;
//     }
//   }
//   return hit_anything;
// }

#endif
