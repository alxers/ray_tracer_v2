#ifndef BOXH
#define BOXH

float min(float a, float b) {
  if (a < b) {
    return a;
  }
  return b;
}

float max(float a, float b) {
  if (a > b) {
    return a;
  }
  return b;
}

// Keep aabb and box separate
// aabb is going to be used for BVH
struct aabb
{
  // min and max bounds
  vec3 vmin;
  vec3 vmax;
};

bool aabb_hit(ray *r, float tmin, float tmax, struct aabb *box, hit_record *rec) {
  for (int i = 0; i < 3; i++) {
    float t0 = min((box->vmin[i] - r->origin()[i]) / r->direction()[i],
                   (box->vmax[i] - r->origin()[i]) / r->direction()[i]);
    float t1 = max((box->vmin[i] - r->origin()[i]) / r->direction()[i],
                   (box->vmax[i] - r->origin()[i]) / r->direction()[i]);

    tmin = max(t0, tmin);
    tmax = min(t1, tmax);
    if (tmax <= tmin) {
      return false;
    }
  }
  rec->t = tmin;
  rec->p = r->point_at_parameter(rec->t);

  // For this method to work with arbitrary aabb (not only a unit cube)
  // we need to compute divisor
  vec3 d = (box->vmin - box->vmax) * 0.5;
  float bias = 1.000001;
  vec3 n;
  int normX = (int)(rec->p.x() / fabs(d.x()) * bias);
  int normY = (int)(rec->p.y() / fabs(d.y()) * bias);
  int normZ = (int)(rec->p.z() / fabs(d.z()) * bias);
  // printf("%d, %d, %d\n", one, two, three);
  rec->normal = unit_vector(vec3((float)normX, (float)normY, (float)normZ));

  return true;
}

// struct xz_rect
// {
//   float x0;
//   float x1;
//   float z0;
//   float z1;
//   float k;
// };

// struct xy_rect
// {
//   float x0;
//   float x1;
//   float y0;
//   float y1;
//   float k;
// };

// struct yz_rect
// {
//   float y0;
//   float y1;
//   float z0;
//   float z1;
//   float k;
// };

// struct box
// {
//   vec3 vmin;
//   vec3 vmax;
//   struct xy_rect xy;
//   struct xz_rect xz;
//   struct yz_rect yz;
// };

// bool xy_hit(ray *r, float t0, float t1, hit_record *rec, struct xy_rect *xy, bool flip_norm) {
//   vec3 ray_origin = r->origin();
//   vec3 ray_direction = r->direction();
//   float t = (xy->k - ray_origin.z()) / ray_direction.z();

//   if (t < t0 || t > t1) {
//     return false;
//   }

//   float x = ray_origin.x() + t * ray_direction.x();
//   float y = ray_origin.y() + t * ray_direction.y();

//   if (x < xy->x0 || x > xy->x1 || y < xy->y0 || y > xy->y1) {
//     return false;
//   }

//   // rec->u = (x - x0) / (x1 - x0);
//   // rec->v = (y - y0) / (y1 - y0);
//   rec->t = t;
//   rec->p = r->point_at_parameter(t);
//   if (flip_norm) {
//     rec->normal = vec3(0, 0, -1);
//   } else {
//     rec->normal = vec3(0, 0, 1);
//   }
//   return true;
// }

// bool xz_hit(ray *r, float t0, float t1, hit_record *rec, struct xz_rect *xz, bool flip_norm) {
//   vec3 ray_origin = r->origin();
//   vec3 ray_direction = r->direction();
//   float t = (xz->k - ray_origin.y()) / ray_direction.y();

//   if (t < t0 || t > t1) {
//     return false;
//   }

//   float x = ray_origin.x() + t * ray_direction.x();
//   float z = ray_origin.z() + t * ray_direction.z();
//   // printf("x %.06f z %.06f\n", x, z);
//   if (x < xz->x0 || x > xz->x1 || z < xz->z0 || z > xz->z1) {
//     return false;
//   }

//   // rec->u = (x - x0) / (x1 - x0);
//   // rec->v = (y - y0) / (y1 - y0);
//   rec->t = t;
//   rec->p = r->point_at_parameter(t);
//   if (flip_norm) {
//     rec->normal = vec3(0, -1, 0);
//   } else {
//     rec->normal = vec3(0, 1, 0);
//   }
//   return true;
// }

// bool yz_hit(ray *r, float t0, float t1, hit_record *rec, struct yz_rect *yz, bool flip_norm) {
//   vec3 ray_origin = r->origin();
//   vec3 ray_direction = r->direction();
//   float t = (yz->k - ray_origin.x()) / ray_direction.x();

//   if (t < t0 || t > t1) {
//     return false;
//   }

//   float y = ray_origin.y() + t * ray_direction.y();
//   float z = ray_origin.z() + t * ray_direction.z();

//   if (y < yz->y0 || y > yz->y1 || z < yz->z0 || z > yz->z1) {
//     return false;
//   }

//   rec->t = t;
//   rec->p = r->point_at_parameter(t);
//   if (flip_norm) {
//     rec->normal = vec3(-1, 0, 0);
//   } else {
//     rec->normal = vec3(1, 0, 0);
//   }
//   return true;
// }

// bool box_hit(
//     ray *r,
//     float t0,
//     float t1,
//     hit_record *rec,
//     struct xy_rect *xy,
//     struct xz_rect *xz,
//     struct yz_rect *yz
//   ) {
//   // printf("%.6f %.6f\n", t0, t1);
//   bool is_xy_hit = xy_hit(r, t0, t1, rec, xy, false);
//   bool is_xz_hit = xz_hit(r, t0, t1, rec, xz, false);
//   bool is_yz_hit = yz_hit(r, t0, t1, rec, yz, false);
//   return (
//     is_xy_hit ||
//     is_xz_hit ||
//     is_yz_hit
//   );
// }




// vec3 box_normal(aabb *box, vec3 hit, hit_record *rec) {
//   vec3 c = (box->vmin + box->vmax) * 0.5;
//   vec3 p = rec->p - c;
//   // For this method to work with arbitrary aabb (not only a unit cube)
//   // we need to compute divisor
//   vec3 d = (box->vmin - box->vmax) * 0.5;
//   float bias = 1.000001;
//   vec3 n;
//   int one = (int)(p.x() / fabs(d.x()) * bias);
//   int two = (int)(p.y() / fabs(d.y()) * bias);
//   int three = (int)(p.z() / fabs(d.z()) * bias);
//   printf("%d, %d, %d\n", one, two, three);
//   n = vec3((float)one, (float)two, (float)three);
//   // printf("%0.6f, %0.6f, %0.6f\n", n.x(), n.y(), n.z());

//   vec3 u = unit_vector(n);
//   return u;
// }

// Only for the unit cube
// vec3 box_normal(aabb *box, vec3 hit) {
//   vec3 c = (box->vmin + box->vmax) * 0.5;
//   vec3 p = hit - c;
//   // float bias = 1.000001;

  
//   float first, second, third;
//   first = p.x();
//   second = p.y();
//   third = p.z();
//   // printf("%0.6f, %0.6f, %0.6f\n", first, second, third);
//   // printf("%0.6f, %0.6f, %0.6f\n", c.x(), c.y(), c.z());
//   //

//   int one = (p.x());
//   int two = (p.y());
//   int three = (p.z());
//   // printf("%d, %d, %d\n", one, two, three);

//   vec3 n = vec3((float)one, (float)two, (float)three);

//   return unit_vector(n);
// }

#endif
