struct box3
{
    // min and max bounds
    vec3 vmin;
    vec3 vmax;
};

// test box
vec3 min(-0.5, -0.5, -1.0);
vec3 max(0.5, 0.5, -3.0);
// box3 b;
box3 b = { min, max };

bool intersect(const ray &r) 
{
    float tmin = (min.x() - r.origin().x()) / r.direction().x(); 
    float tmax = (max.x() - r.origin().x()) / r.direction().x(); 
 
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

// Calculate normal for the cube
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

    // float bias = 1.000001;

    vec3 n = vec3(
      (int)(p.x()/fabs(dx)),
      (int)(p.y()/fabs(dy)),
      (int)(p.z()/fabs(dz))
    );

  // vec3 c = (box->vmin + box->vmax) * 0.5;
  // vec3 p = hit - c;
  // vec3 d = (box->vmin - box->vmax) * 0.5;
  // float bias = 1.000001;
  // vec3 n;
  // int one = p.x() / fabs(d.x());
  // int two = p.y() / fabs(d.y());
  // int three = p.z() / fabs(d.z());

  // n = vec3((float)one, (float)two, (float)three);

  // vec3 u = unit_vector(n);

  // if (n.x() < 1.0 && n.x() > 0.0) {
  //   1+1;
  //   return n;
  // }

  printf("%.6f %.6f %.6f\n", n.x(), n.y(), n.z());


  return unit_vector(n);
  // return u;
}

// vec3 color(const ray& r) {


//     if (intersect(r) > 0.0) {
//         vec3 n = normal(b, r.direction());
//     printf("n is: %f, %f, %f \n", n.x(), n.y(), n.z());
//     return 0.5 * vec3(n.x()+1, n.y()+1, n.z()+1);
//     }

//     vec3 unit_direction = unit_vector(r.direction());
//     float t = 0.5*(unit_direction.y() + 1.0);
//     return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
// }
