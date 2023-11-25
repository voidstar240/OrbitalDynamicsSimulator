#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include "body.h"

// the base class of all integrators
class Integrator {
public:
    double dt;
    size_t steps;
    std::vector<Body> initial_conditions;
    std::vector<Vector3> (*rate_func)(std::vector<Body> state, double time);

    virtual std::vector<std::vector<Body>> run() const = 0;
};

// helper function to calculate the distance between 2 positions
double dist(Vector3 a, Vector3 b) {
    return std::sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z));
}

// the rate function
std::vector<Vector3> grav_rate_func(std::vector<Body> state, double t) {
    std::vector<Vector3> rates;
    for (size_t i = 0; i < state.size(); i++) {
        Vector3 rate;
        for (size_t j = 0; j < state.size(); j++) {
            if (j == i) {
                continue;
            }
            double rate_scal = state[j].mu / std::pow(dist(state[j].pos, state[i].pos), 3.0);
            Vector3 diff = state[j].pos - state[i].pos;
            rate.x += rate_scal * diff.x;
            rate.y += rate_scal * diff.y;
            rate.z += rate_scal * diff.z;
        }
    }
    return rates;
}

// Forward Euler Integrator derived class
class ForwardEuler : Integrator {
public:
    ForwardEuler(double dt, size_t steps, std::vector<Body> initial, std::vector<Vector3> (*rate)(std::vector<Body> state, double time)) {
        this->dt = dt;
        this->steps = steps;
        this->initial_conditions = initial;
        this->rate_func = rate;
    }

    std::vector<std::vector<Body>> run() const override {
        std::vector<std::vector<Body>> states;
        states.push_back(initial_conditions);
        for (size_t n = 1; n <= steps; n++) {
            double time = dt * n;
            // current state = last state
            std::vector<Body> state_n = states[n - 1];
            std::vector<Vector3> rate = grav_rate_func(states[n - 1], time - dt);
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
