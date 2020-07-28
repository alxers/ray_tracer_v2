#ifndef MATERIALH
#define MATERIALH

struct material {
  char type[20];
  // Measure of diffuse reflection
  vec3 albedo;
};

vec3 reflect(vec3 *v, vec3 *n) {
	return v - 2 * dot(v, n) * n;
}

#endif
