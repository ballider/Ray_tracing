#ifndef Figure_hpp
#define Figure_hpp

#include <iostream>
#include "Vec3d.hpp"
#include "Material.hpp"
#include "Ray.hpp"

class Figure {
protected:
    Material _material;
public:
    Figure(): _material() {};
    Figure(const Material& material): _material(material) {};

    virtual double ray_intersect(const Ray& ray) const = 0;
    
    virtual Vec3d center() const = 0;

    Material material() const {
        return _material;
    };

    void UpdateMaterial(const Material& material) {
        _material = material;
    }

    virtual ~Figure(){};
};

class Sphere : public Figure {
private:
    Vec3d _center = {0.0, 0.0, 0.0};
    double _R = 1.0;
private:
    const double PI = 3.14159265358979323846;
public:
    Sphere() = default;
    Sphere(const Vec3d& c, double R, const Material& material): Figure(material), _center(c), _R(R) {};
    Sphere(const Vec3d& c, double R): Figure(), _center(c), _R(R) {};

    double ray_intersect(const Ray& ray) const override {
        Vec3d L = ray.orig - _center;

        double a = 1.0; // (dir; dir) = 1
        double b = 2*dot(ray.dir,L);
        double c = dot(L, L) - _R*_R;
        
        double det = b*b-4*a*c;

        if(det > 0) {
            double t1 = (-b + sqrt(det)) / 2;
            double t2 = (-b - sqrt(det)) / 2;
            if(t1 > 0 && t2 > 0) {              // если меньше нуля значит пересекает вне поля видимости камеры
                return std::min(t1, t2);
            }
        }
        
        return -1;  // не пересекла или за камерой
    }

    Vec3d center() const override{
        return _center;
    }
    double R() const {
        return _R;
    }
};

class Box : public Figure {
private:
    Vec3d _A = {0.0, 0.0, 0.0};
    Vec3d _B = {1.0, 1.0, 1.0};
public:
    Box() = default;
    Box(const Vec3d& A, const Vec3d& B, const Material& material): Figure(material), _A(A), _B(B) {};
    Box(const Vec3d& A, const Vec3d& B): Figure(), _A(A), _B(B) {};

    double ray_intersect(const Ray& ray) const override {
        // Так как все стороны бокса параллельны осям коориднат, то они емеют вид xi = a, где xi - это координта(x, y, z)
        // луч пересекает грань, если dir не перпендикулярен нормали номарли, то есть когда скалярное произведение не равно 0
        double t_near = std::numeric_limits<double>::min(),
               t_far  = std::numeric_limits<double>::max();
        double t1, t2;

        for(int i = 0; i < 3; ++i) {
            double d = ray.dir[i];
            if(d < 0.0) d *= -1;
            if(d >= std::numeric_limits<double>::epsilon()) {
                t1 = (_A[i] - ray.orig[i]) / ray.dir[i];    // первая грань
                t2 = (_B[i] - ray.orig[i]) / ray.dir[i];    // вторая грань
                if(t1 > t2) std::swap(t1, t2);

                //std::cout << t1 << " " << t1 << "\n";

                if(t1 > t_near) t_near = t1;
                if(t2 < t_far) t_far = t2;

                if(t_near > t_far) return -1;
                if(t_far < 0.0) return -1;
            }
            else {
                //std::cout << "ok\n";
                // если dir параллелен нормали(dir[i] == 0)
                if(ray.orig[i] < _A[i] || ray.orig[i] > _B[i] ) return -1;
            }
        }
        //std::cout << t_near << "\n";
        if(t_near <= t_far && t_far >=0) return t_near;
        else return -1;
    }

    Vec3d A() const {
        return _A;
    }
    Vec3d B() const {
        return _B;
    }
    Vec3d center() const override {
        return Vec3d( (_A[0]+_B[0])/2, (_A[1]+_B[1])/2, (_A[2]+_B[2])/2);
    }
};

class Tetrahedron : public Figure {
private:
    Vec3d _A = {0.0, 0.0, 0.0};
    Vec3d _B = {1.0, 0.0, 0.0};
    Vec3d _C = {0.0, 1.0, 0.0};
    Vec3d _D = {0.0, 0.0, 1.0};
private:
    // Функция возвращает расстояние от начала луча до точки пересечения или 0.
    // Алгоритм Моллера — Трумбора
    double triangle_intersection(const Vec3d& orig, const Vec3d& dir, const Vec3d& v0, const Vec3d& v1, const Vec3d& v2) const {
        Vec3d e1 = v1 - v0;
        Vec3d e2 = v2 - v0;
        // Вычисление вектора нормали к плоскости
        Vec3d pvec = cross(dir, e2);
        double det = dot(e1, pvec);

        // Луч параллелен плоскости
        if (det < 1e-8 && det > -1e-8) {
            return 0;
        }

        double inv_det = 1 / det;
        Vec3d tvec = orig - v0;
        double u = dot(tvec, pvec) * inv_det;
        if (u < 0 || u > 1) {
            return 0;
        }

        Vec3d qvec = cross(tvec, e1);
        double v = dot(dir, qvec) * inv_det;
        if (v < 0 || u + v > 1) {
            return 0;
        }
        return dot(e2, qvec) * inv_det;
    }
public:
    Tetrahedron() = default;
    Tetrahedron(const Vec3d& A, const Vec3d& B, const Vec3d& C, const Vec3d& D, const Material& material): Figure(material), _A(A), _B(B), _C(C), _D(D) {};
    Tetrahedron(const Vec3d& A, const Vec3d& B, const Vec3d& C, const Vec3d& D): Figure(), _A(A), _B(B), _C(C), _D(D) {};
    
    double ray_intersect(const Ray& ray) const override {
        double t1 = triangle_intersection(ray.orig, ray.dir, _A, _B, _C);
        double t2 = triangle_intersection(ray.orig, ray.dir, _A, _B, _D);
        double t3 = triangle_intersection(ray.orig, ray.dir, _D, _B, _C);
        double t4 = triangle_intersection(ray.orig, ray.dir, _A, _D, _C);

        double t_leave = std::max(t1, std::max(t2, std::max(t3, t4)));

        if(t_leave <= 0.0) return -1;

        double t_enter = -1.0;

        if(t1 != 0.0) t_enter = t1;
        if(t2 != 0.0) t_enter = std::min(t_enter, t2);
        if(t3 != 0.0) t_enter = std::min(t_enter, t3);
        if(t4 != 0.0) t_enter = std::min(t_enter, t4);

        return t_enter;
    }

    Vec3d center() const override {
        double sumi[3] = {0.0, 0.0, 0.0};
        for(size_t i = 0; i < 3; ++i) {
            sumi[i] += (_A[i]+_B[i]+_C[i]+_D[i]);
        }
        return Vec3d(sumi[0]/4, sumi[1]/4, sumi[2]/4);
    }
};

#endif /* Figure_hpp */
