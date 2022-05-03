#include "Vec3d.hpp"

Vec3d operator-(const Vec3d& a, const Vec3d& b) {
    return Vec3d(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

Vec3d operator+(const Vec3d& a, const Vec3d& b) {
    return Vec3d(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

Vec3d operator*(const Vec3d& a, double k) {
    return Vec3d(a[0]*k, a[1]*k, a[2]*k);
}

Vec3d operator*(double k, const Vec3d& a) {
    return Vec3d(a[0]*k, a[1]*k, a[2]*k);
}

Vec3d cross(const Vec3d& a, const Vec3d& b) {
    return Vec3d(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

double dot(const Vec3d& a, const Vec3d& b) {
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]; 
}