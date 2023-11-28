#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include "body.h"

template<typename T>
using vector = std::vector<T>;

// the base class of all integrators
class Integrator {
public:
    double dt;
    size_t steps;
    vector<Body> initial_conditions;
    // vvv funciton ptr, returns acceleration vectors vvv
    vector<Vector3> (*rate_func)(vector<Body> state, double time);

    virtual vector<vector<Body>> run() const = 0;
};

// Forward Euler Integrator derived class
class ForwardEuler : Integrator {
public:
    ForwardEuler(double dt, size_t steps, vector<Body> initial, vector<Vector3> (*rate)(vector<Body> state, double time)) {
        this->dt = dt;
        this->steps = steps;
        this->initial_conditions = initial;
        this->rate_func = rate;
    }

    vector<vector<Body>> run() const override {
        vector<vector<Body>> states;
        states.push_back(initial_conditions);
        for (size_t n = 1; n <= steps; n++) {
            double time = dt * n;
            // current state = last state
            vector<Body> state_n = states[n - 1];
            vector<Vector3> rate = rate_func(states[n - 1], time - dt);
            // add rate*dt to the current state (which is currently set to last state)
            for (size_t i = 0; i < state_n.size(); i++) {
                state_n[i].vel.x += dt * rate[i].x;
                state_n[i].vel.y += dt * rate[i].y;
                state_n[i].vel.z += dt * rate[i].z;
                state_n[i].pos.x += dt * states[n - 1][i].vel.x;
                state_n[i].pos.y += dt * states[n - 1][i].vel.y;
                state_n[i].pos.z += dt * states[n - 1][i].vel.z;
            }
            states.push_back(state_n);
        }
        return states;
    }
};
