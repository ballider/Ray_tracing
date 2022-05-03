#ifndef Material_hpp
#define Material_hpp

#include <iostream>
#include "Vec3d.hpp"

struct Material {
    Material(const Vec3d& color, const double& shininess): _color(color),  _shininess(shininess) {};
    Material(const Vec3d& color): _color(color), _shininess(100) {};
    Material(): _color(), _shininess(100) {};
    
    Vec3d _color;   // diffuse
    double _shininess;

    Material& operator=(const Material& material) {
        if(this == &material) return *this;
        _color = material._color;
        _shininess = material._shininess;
        return *this;
    }
};

#endif /* Material_hpp */
