#include <iostream>
#include "ray.h"

// struct box
// {
//     float x;
//     float y;
//     float z;
// };

// box box1;

struct box3
{
    // min and max bounds
    vec3 vmin;
    vec3 vmax;
};

// test box
vec3 min(-0.5, -0.5, -2.0);
vec3 max(0.5, 0.5, -3.0);
box3 b;
b.vmin = vec3 min(-0.5, -0.5, -2.0);
// box.vmax = max;


bool intersect(const ray &r) 
{
    // box min;
    // box max;

    // min.x = -0.5;
    // min.y = -0.5;
    // min.z = -2.0;

    // max.x = 0.5;
    // max.y = 0.5;
    // max.z = -3.0;


    float tmin = (min.x() - r.origin().x()) / r.direction().x(); 
    float tmax = (max.x() - r.origin().x()) / r.direction().x(); 

    // std::cout << "orig" << r.origin().x() << " " << r.origin().y() << " " << r.origin().z() << "\n";
    // std::cout << "direc"<< r.direction().x() << " " << r.direction().y() << " " << r.direction().z() << "\n";
 
    if (tmin > tmax) std::swap(tmin, tmax); 
 
    float tymin = (min.y() - r.origin().y()) / r.direction().y(); 
    float tymax = (max.y() - r.origin().y()) / r.direction().y(); 
 
    if (tymin > tymax) std::swap(tymin, tymax); 
 
    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
 
    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 
 
    float tzmin = (min.z() - r.origin().z()) / r.direction().z(); 
    float tzmax = (max.z() - r.origin().z()) / r.direction().z(); 
 
    if (tzmin > tzmax) std::swap(tzmin, tzmax); 
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
 
    if (tzmin > tmin) 
        tmin = tzmin; 
 
    if (tzmax < tmax) 
        tmax = tzmax; 
 
    return true; 
} 

bool hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

// vec3 color(const ray& r) {
//  if (hit_sphere(vec3(0,0,-1), 0.5, r)) {
//         return vec3(1, 0, 0);
//  }

//  vec3 unit_direction = unit_vector(r.direction());
//  float t = 0.5*(unit_direction.y() + 1.0);
//  return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
// }

// Calculate normal for the cube
// vec3 normal(box b, vec3 hit) {
//     vec3 centerPoint = ((box.min + box.max) * 0.5,
//         );
// }

vec3 color(const ray& r) {
    // Box b(vec3(-1.0, 0.0, 0.0), vec3());
    if (intersect(r)) {
        if(r.direction().z() == -2.0) {
            return vec3(1, 0.5, 0);
        } else {
            return vec3(1, 0, 0);
        }
    }

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 800;
    int ny = 400;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(-2.0, 2.0, 0.0);
    // Draw pixels from left to right
    // top to bottom
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
