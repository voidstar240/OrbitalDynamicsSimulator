#pragma once
#include <string>

struct Vector3 {
    double x, y, z;

    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3() : x(0.0), y(0.0), z(0.0) {}
};

struct Body {
    std::string name;
    Vector3 pos;
    Vector3 vel;
    double mu;

    Body(Vector3 pos, Vector3 vel, double mu) : pos(pos), vel(vel), mu(mu) {}
    Body() : pos(), vel(), mu(0.0) {}
};
