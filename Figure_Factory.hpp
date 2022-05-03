#ifndef Figure_Factory_hpp
#define Figure_Factory_hpp

#include "Figure.hpp"

class Creator{
public:
    virtual Figure* factoryMethod() = 0;
    virtual Figure* factoryMethod(const Vec3d& c, double R) = 0;
    virtual Figure* factoryMethod(const Vec3d& a, const Vec3d& b) = 0;
    virtual Figure* factoryMethod(const Vec3d& a, const Vec3d& b, const Vec3d& c, const Vec3d& d) = 0;

    virtual ~Creator(){};
};

class CreateSphere : public Creator {
public:
    Figure* factoryMethod() override {
        return new Sphere();
    }
    Figure* factoryMethod(const Vec3d& c, double R) {
        return new Sphere(c, R);
    }
    Figure* factoryMethod(const Vec3d& a, const Vec3d& b) override {
        return new Sphere();
    }
    Figure* factoryMethod(const Vec3d& a, const Vec3d& b, const Vec3d& c, const Vec3d& d) override {
        return new Sphere();
    }
};

class CreateBox : public Creator {
public:
    Figure* factoryMethod() override {
        return new Box();
    }
    Figure* factoryMethod(const Vec3d& a, const Vec3d& b) override {
        return new Box(a, b);
    }
    Figure* factoryMethod(const Vec3d& c, double R) override {
        return new Box();
    }
    Figure* factoryMethod(const Vec3d& a, const Vec3d& b, const Vec3d& c, const Vec3d& d) override {
        return new Box();
    }
};

class CreateTetrahedron : public Creator {
public:
    Figure* factoryMethod() override {
        return new Tetrahedron();
    }
    Figure* factoryMethod(const Vec3d& c, double R) override {
        return new Tetrahedron();
    }
    Figure* factoryMethod(const Vec3d& a, const Vec3d& b) override {
        return new Tetrahedron();
    }
    Figure* factoryMethod(const Vec3d& a, const Vec3d& b, const Vec3d& c, const Vec3d& d) override {
        return new Tetrahedron(a, b, c, d);
    }
};


#endif