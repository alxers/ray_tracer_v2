#ifndef MATERIALH
#define MATERIALH

struct material {
  // 1 - lambertian
  // 2 - metal
  int type;
  // Measure of diffuse reflection
  vec3 albedo;
};

vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    // Pick a point in the unit cube
    p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    // If it's not on a unit sphere reject it and try again
  } while (dot(&p, &p) >= 1);
  return p;
}

vec3 random_in_unit_box() {
  // vec3 p;
  // p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
  // printf("%0.6f %0.6f %0.6f\n", p.x(), p.y(), p.z());
  // return p;

  // drand48 always return a float between 0 and 1
  // we need a random float between -1 and 1
  return 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
}

vec3 reflect(vec3 *v, vec3 *n) {
  return *v - 2 * dot(v, n) * (*n);
}

// Lambertian reflectance is the property that defines an ideal "matte" or diffusely reflecting surface
// Light that reflects from diffuse surface has random directons.
// so almost any algo that randomazes direction will produce the matte surface.
bool lambertian_scatter(ray *r_in, hit_record *rec, vec3 *attenuation, ray *scattered, struct material *mat) {
  vec3 target = rec->p + rec->normal + random_in_unit_sphere();
  *scattered = ray(rec->p, target - rec->p);
  // "fading"
  *attenuation = mat->albedo;
  return true;
}

bool lambertian_scatter_box(ray *r_in, hit_record *rec, vec3 *attenuation, ray *scattered, struct material *mat) {
  vec3 target = rec->p + random_in_unit_box();
  *scattered = ray(rec->p, target - rec->p);
  *attenuation = mat->albedo;
  return true;
}

bool metal_scatter(ray *r_in, hit_record *rec, vec3 *attenuation, ray *scattered, struct material *mat) {
  vec3 unit_vec_dir = unit_vector(r_in->direction());
  vec3 reflected = reflect(&unit_vec_dir, &rec->normal);
  *scattered = ray(rec->p, reflected);
  *attenuation = mat->albedo;
  vec3 scattered_dir = scattered->direction();
  return (dot(&scattered_dir, &(rec->normal)) > 0);
}

#endif
