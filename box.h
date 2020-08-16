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

vec3 box_normal(aabb *box, vec3 hit) {
    vec3 centerPoint(
        (box->vmin.x() + box->vmax.x()) * 0.5,
        (box->vmin.y() + box->vmax.y()) * 0.5,
        (box->vmin.z() + box->vmax.z()) * 0.5
    );

    vec3 p = hit - centerPoint;

    float dx = abs(box->vmin.x() - box->vmax.x())/2;
    float dy = abs(box->vmin.y() - box->vmax.y())/2;
    float dz = abs(box->vmin.z() - box->vmax.z())/2;

    vec3 n = vec3(
        (int) p.x()/dx,
        (int) p.y()/dy,
        (int) p.z()/dx
    );

    return unit_vector(n);
}

#endif
