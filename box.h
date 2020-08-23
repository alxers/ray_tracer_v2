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

bool aabb_hit(ray *r, float tmin, float tmax, struct aabb *box) {
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
  return true;
}

struct xz_rect
{
  float x0;
  float x1;
  float z0;
  float z1;
  float k;
};

struct xy_rect
{
  float x0;
  float x1;
  float y0;
  float y1;
  float k;
};

struct yz_rect
{
  float y0;
  float y1;
  float z0;
  float z1;
  float k;
};

bool xy_hit(ray *r, float t0, float t1, hit_record *rec, struct xy_rect *xy) {
  vec3 ray_origin = r->origin();
  vec3 ray_direction = r->direction();
  float t = (xy->k - ray_origin.z()) / ray_direction.z();

  if (t < t0 || t > t1) {
    return false;
  }

  float x = ray_origin.x() + t * ray_direction.x();
  float y = ray_origin.y() + t * ray_direction.y();

  if (x < xy->x0 || x > xy->x1 || y < xy->y0 || y > xy->y1) {
    return false;
  }

  // rec->u = (x - x0) / (x1 - x0);
  // rec->v = (y - y0) / (y1 - y0);
  rec->t = t;
  rec->p = r->point_at_parameter(t);
  rec->normal = vec3(0, 0, 1);
  return true;
}



// Gives wrong result? Or just coloring function should be different
// for the box?
// vec3 box_normal(aabb *box, vec3 hit) {
//   vec3 c = (box->vmin + box->vmax) * 0.5;
//   vec3 p = hit - c;
//   vec3 d = (box->vmin - box->vmax) * 0.5;
//   float bias = 1.000001;
//   vec3 n;
//   int one = p.x() / fabs(d.x());
//   int two = p.y() / fabs(d.y());
//   int three = p.z() / fabs(d.z());

//   n = vec3((float)one, (float)two, (float)three);

//   vec3 u = unit_vector(n);
//   return u;
// }

#endif
