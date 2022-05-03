#ifndef Ray_hpp
#define Ray_hpp

#include <iostream>
#include "Vec3d.hpp"

struct Ray {
    Vec3d orig;     // start point
    Vec3d dir;      // unit direction vector

    Ray(): orig(Vec3d()), dir(Vec3d()) {};
    Ray(const Vec3d& orig, const Vec3d& dir): orig(orig), dir(dir) {};
    Ray(const Ray& ray): orig(ray.orig), dir(ray.dir) {};
};

#endif /* Ray_hpp */
