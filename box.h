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

// bool aabb_hit(ray *r, float tmin, float tmax, struct aabb *box) {
//   for (int i = 0; i < 3; i++) {
//     float t0 = min()
//   }
// }
