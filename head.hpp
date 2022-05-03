#ifndef head_hpp
#define head_hpp

#include <iostream>
#include <limits>
#include <cmath>
#include <fstream>
#include <vector>
#include <omp.h>
#include <string>
#include <map>

#include "Vec3d.hpp"
#include "Figure.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Ray.hpp"

struct Parametrs {
    Vec3d cam;
    Vec3d normal;
    Vec3d up;
    double screen;
    double limit;
    double alpha;
    size_t width;
    size_t height;
    Light light;
};

//
bool nearest_intersect(const Ray&, const std::vector<Figure*>&, Vec3d&, Vec3d&, Material&);
Vec3d cast_ray(const Ray&, const std::vector<Figure*>&, const std::vector<Light>&);
void render(const std::vector<Figure*>&, Parametrs&);
//

//
bool ParametrsFromFile(const std::string&, Parametrs&);
bool FiguresFromFile(const std::string&, std::vector<Figure*>&);
//

#endif /* head_hpp */
