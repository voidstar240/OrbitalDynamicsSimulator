#pragma once
#include <string>

// Basic Vector in 3d space
struct Vector3 {
    double x, y, z;

    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3() : x(0.0), y(0.0), z(0.0) {}

    Vector3 operator+(const Vector3& other) {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
};

// Stores all the information needed to simulate a celestial body
struct Body {
    std::string name; // this is optional
    Vector3 pos;
    Vector3 vel;
    double mu; // the gravitational parameter of the body

    Body(Vector3 pos, Vector3 vel, double mu) : pos(pos), vel(vel), mu(mu) {}
    Body() : pos(), vel(), mu(0.0) {}
};
