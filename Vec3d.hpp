#ifndef Vec3d_hpp
#define Vec3d_hpp

#include <iostream>
#include <cassert>
#include <cmath>

class Vec3d {
private:
    double _vec[3] = {0.0, 0.0, 0.0};
public:
    Vec3d() = default;
    Vec3d(double x, double y, double z) {
        _vec[0] = x;
        _vec[1] = y;
        _vec[2] = z;
    }
    Vec3d(const Vec3d& a) {
        _vec[0] = a._vec[0];
        _vec[1] = a._vec[1];
        _vec[2] = a._vec[2];
    }

    double norm() const{ 
        return sqrt(_vec[0]*_vec[0]+_vec[1]*_vec[1]+_vec[2]*_vec[2]);
    }
    Vec3d& normalize() {
        double k = 1/norm();
        _vec[0] *= k;
        _vec[1] *= k;
        _vec[2] *= k;
        return *this;
    }

    Vec3d& operator=(const Vec3d& n){
        if(this == &n) return *this;
        _vec[0] = n._vec[0];
        _vec[1] = n._vec[1];
        _vec[2] = n._vec[2];
        return *this;
    }

    double& operator[](const size_t i) {
        assert(i < 3);
        return _vec[i];
    }
    const double& operator[](const size_t i) const {
        assert(i < 3);
        return _vec[i];
    }

    void print() const{
        std::cout << "(" << _vec[0] << "; " << _vec[1] << "; " << _vec[2] << ")";
    }
};

Vec3d operator -(const Vec3d&, const Vec3d&);
Vec3d operator +(const Vec3d&, const Vec3d&);

Vec3d operator*(const Vec3d&, double);
Vec3d operator*(double, const Vec3d&);

Vec3d cross(const Vec3d&, const Vec3d&);
double  dot(const Vec3d&, const Vec3d&);

#endif /* Vec3d_hpp */
