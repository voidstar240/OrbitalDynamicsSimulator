#pragma once
#include <vector>
#include "body.h"

class Integrator {
public:
    double dt;
    size_t steps;
    std::vector<Body> initial_conditions;
    Vector3 (*rate_func)(std::vector<Body> state, double time);

    virtual std::vector<std::vector<Body>> run() const = 0;
};

class ForwardEuler : Integrator {
public:
    ForwardEuler(double dt, size_t steps, std::vector<Body> initial, Vector3 (*rate)(std::vector<Body> state, double time)) {
        this->dt = dt;
        this->steps = steps;
        this->initial_conditions = initial;
        this->rate_func = rate;
    }
};
