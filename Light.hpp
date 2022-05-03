#ifndef Light_hpp
#define Light_hpp

#include <iostream>
#include "Vec3d.hpp"

struct Light {
    Vec3d _position;
    double _diffuse;    // коэффициент в модели Фонга
                        // ambident и specular считаем равные 1
    
    Light(): _position(), _diffuse(1.0) {};
    Light(const Vec3d& position, double diffuse): _position(position), _diffuse(diffuse) {};
    Light(const Light& light): _position(light._position), _diffuse(light._diffuse) {};
    Light(const Vec3d& position): _position(position), _diffuse(1.0) {};

    Light& operator=(const Light& light){
        if(this == &light) return *this;
        _position = light._position;
        _diffuse = light._diffuse;
        return *this;
    }
};

#endif /* Light_hpp */
