#ifndef CAMERAH
#define CAMERAH

struct camera
{
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 origin;
};

void set_cam(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, struct camera *cam) {
  vec3 u, v, w;
  float theta = vfov * M_PI/180;
  float half_height = tan(theta/2);
  float half_width = aspect * half_height;
  cam->origin = lookfrom;
  w = unit_vector(lookfrom - lookat);
  u = unit_vector(cross(vup, w));
  v = cross(w, u);
  // cam->lower_left_corner = vec3(-half_width, -half_height, -1.0);
  cam->lower_left_corner = cam->origin - half_width * u - half_height * v - w;
  cam->horizontal = 2 * half_width * u;
  cam->vertical = 2 * half_height * v;
}

ray get_ray(float u, float v, struct camera cam) {
  return ray(cam.origin, cam.lower_left_corner + u * cam.horizontal + v * cam.vertical - cam.origin);
}

#endif
