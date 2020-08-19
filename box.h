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

bool xz_hit(ray *r, float t0, float t1, hit_record *rec) {
  return true;
}

#endif
