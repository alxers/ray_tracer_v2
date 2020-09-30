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

  // TODO: remove material from aabb
  // probably should be in a "box" struct
  struct material mat;
};

bool aabb_hit(ray *r, float tmin, float tmax, struct aabb *box, hit_record *rec) {
  // "slab" method:
  // n-dimensional aabb is an intersection of n-axis aligned intervals
  // for ray to hit one interval we need to figure out whether the ray hits the boundaries.
  // Since the ray is a function p(t) = A + tB the equation applies to all three of x, y, z coords
  // f.e. x(t) = Ax + t*Bx;
  // This ray hits the plane x = x0 at the t:
  // x0 = Ax + t0 * Bx;
  // thus the hitpoint is t0 = (x0 - Ax) / Bx, same for t1.
  // Now for a hit the "t" intervals should overlap.
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

  int normX = (int)(rec->p.x() / fabs(d.x()) * bias);
  int normY = (int)(rec->p.y() / fabs(d.y()) * bias);
  int normZ = (int)(rec->p.z() / fabs(d.z()) * bias);
  // printf("%d, %d, %d\n", one, two, three);
  rec->normal = unit_vector(vec3((float)normX, (float)normY, (float)normZ));

  return true;
}

#endif
