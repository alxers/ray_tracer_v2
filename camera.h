#ifndef CAMERAH
#define CAMERAH

struct camera
{
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 origin;
};

ray get_ray(float u, float v, struct camera cam) {
  return ray(cam.origin, cam.lower_left_corner + u * cam.horizontal + v * cam.vertical - cam.origin);
}

#endif
