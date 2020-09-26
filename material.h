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
    p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
  } while (dot(&p, &p) >= 1);
  return p;
}

// vec3 random_in_unit_box() {
//   vec3 p;
//   do {
//     p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
//     // TODO: this is not correct
//   } while (
//     p.x() == 1 || p.y() == 1 || p.z() == 1 ||
//     p.x() == -1 || p.y() == -1 || p.z() == -1
//   );
//   // printf("%0.6f %0.6f %0.6f\n", p.x(), p.y(), p.z());
//   return p;
// }

vec3 random_in_unit_box() {
  return 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
}

vec3 reflect(vec3 *v, vec3 *n) {
  return *v - 2 * dot(v, n) * (*n);
}

// Lambertian reflectance is the property that defines an ideal "matte" or diffusely reflecting surface
bool lambertian_scatter(ray *r_in, hit_record *rec, vec3 *attenuation, ray *scattered, struct material *mat) {
  vec3 target = rec->p + rec->normal + random_in_unit_sphere();
  *scattered = ray(rec->p, target - rec->p);
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
