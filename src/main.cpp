#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include "body.h"
#include "integrator.h"
#include "file_handler.h"

// all possible commands
enum class Command : uint8_t {
    INVALID = 0,
    QUIT = 1,
    HELP = 2,
    LIST_BODIES = 3,
    ADD_BODY = 4,
    DEL_BODY = 5,
    CLEAR_BODIES = 6,
    SET_DT = 7,
    RUN = 8,
    CHANGE_NAME = 9,
    SET_STEPS = 10,
};

// parses the input string into a command
Command input_command() {
    std::cout << "[cmd]: ";
    std::string cmd;
    std::cin >> cmd;

    if (cmd == "quit" || cmd == "q") {
        return Command::QUIT;
    } else if (cmd == "help" || cmd == "?") {
        return Command::HELP;
    } else if (cmd == "list" || cmd == "ls") {
        return Command::LIST_BODIES;
    } else if (cmd == "add" || cmd == "a") {
        return Command::ADD_BODY;
    } else if (cmd == "del" || cmd == "d") {
        return Command::DEL_BODY;
    } else if (cmd == "del_all") {
        return Command::CLEAR_BODIES;
    } else if (cmd == "set_dt") {
        return Command::SET_DT;
    } else if (cmd == "run") {
        return Command::RUN;
    } else if (cmd == "filename") {
        return Command::CHANGE_NAME;
    } else if (cmd == "set_steps") {
        return Command::SET_STEPS;
    } else {
        return Command::INVALID;
    }
}

// helper function for handling int input
int input_int(std::string label) {
    while (true) {
        // keep prompting for int input until a valid int is provided
        std::cout << "[int]" << label << ": ";
        std::string str;
        std::cin >> str;
        try {
            return stoi(str);
        } catch(...) {
            std::cout << "`" << str << "` is not an int." << std::endl;
        }
    }
}

// helper function for handling double input
double input_double(std::string label) {
    while (true) {
        // keep prompting for double input until a valid double is provided
        std::cout << "[double]" << label << ": ";
        std::string str;
        std::cin >> str;
        try {
            return stod(str);
        } catch(...) {
            std::cout << "`" << str << "` is not a double." << std::endl;
        }
    }
}

// helper function for handling condition input
// yes is true, no is the default
bool input_y_N() {
    std::cout << "[y/N]: ";
    std::string str;
    std::cin >> str;
    return (str.size() > 0) && ((str[0] == 'y') || (str[0] == 'Y'));
}

// helper function for handling condition input
// yes is true, yes is the default
bool input_Y_n() {
    std::cout << "[Y/n]: ";
    std::string str;
    std::cin >> str;
    return !((str.size() > 0) && ((str[0] == 'n') || (str[0] == 'N')));
}

// helper function for computing the distance between vectors
double dist(const Vector3 a, const Vector3 b) {
    return std::sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z));
}

// computes the gravitational force of attraction between all bodies
std::vector<Vector3> rate_func(std::vector<Body> state, double t) {
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
        rates.push_back(rate);
    }
    return rates;
}

// the name of the output file
std::string file_name = "output.csv";

int main() {
    std::vector<Body> bodies;
    double dt = 100;
    int steps = 100;
    std::cout << "Orbital Dynamics Simulator\nType `help` for a list of commands." << std::endl;
    bool running = true;
    while (running) {
        switch (input_command()) {
            case Command::INVALID: {
                std::cout << "Command unrecognized. Type `help` for a list of commands." << std::endl;
                break;
            }
            case Command::QUIT: {
                running = false;
                break;
            }
            case Command::HELP: {
                std::cout << "quit       quits the program" << std::endl;
                std::cout << "help       displays this page" << std::endl;
                std::cout << "list       displays all bodies for simulation" << std::endl;
                std::cout << "add        adds a new body to be simulated" << std::endl;
                std::cout << "del        deletes a body at an index" << std::endl;
                std::cout << "del_all   deletes all the bodies" << std::endl;
                std::cout << "set_dt     sets the timestep time for the simulate" << std::endl;
                std::cout << "set_steps  sets the number of timesteps to simulate" << std::endl;
                std::cout << "run        runs the simulation and outputs the data to the file" << std::endl;
                std::cout << "filename   changes the output file name" << std::endl;
                break;
            }
            case Command::LIST_BODIES: {
                if (bodies.size() == 0) {
                    std::cout << "There are currently no bodies to be simulated." << std::endl;
                    break;
                }
                for (size_t i = 0; i < bodies.size(); i++) {
                    std::cout << i << ":" << std::endl;
                    std::cout << "  name: " << bodies[i].name << std::endl;
                    std::cout << "  pos: <" << bodies[i].pos.x << ", " << bodies[i].pos.y << ", " << bodies[i].pos.z << ">" << std::endl;
                    std::cout << "  vel: <" << bodies[i].vel.x << ", " << bodies[i].vel.y << ", " << bodies[i].vel.z << ">" << std::endl;
                    std::cout << "  mu: " << bodies[i].mu << std::endl;
                }
                break;
            }
            case Command::ADD_BODY: {
                std::cout << "Adding body " << bodies.size() << "." << std::endl;
                Body body;
                std::cout << "[string] name: ";
                std::cin >> body.name;
                body.pos.x = input_double(" x");
                body.pos.y = input_double(" y");
                body.pos.z = input_double(" z");
                body.vel.x = input_double(" x'");
                body.vel.y = input_double(" y'");
                body.vel.z = input_double(" z'");
                body.mu = input_double(" mu");
                bodies.push_back(body);
                std::cout << "Added body " << bodies.size() - 1 << "." << std::endl;
                break;
            }
            case Command::DEL_BODY: {
                if (bodies.size() == 0) {
                    std::cout << "There are no bodies to delete." << std::endl;
                    break;
                }
                std::cout << "What index to delete?" << std::endl;
                int i;
                i = input_int(" index");
                if (i < 0 || i >= bodies.size()) {
                    std::cout << i << " out of range [0.." << bodies.size() - 1 << "]." << std::endl;
                    std::cout << "No bodies deleted." << std::endl;
                    break;
                }
                bodies.erase(bodies.begin() + i);
                break;
            }
            case Command::CLEAR_BODIES: {
                std::cout << "Are you sure you want to erase all body data?" << std::endl;
                if (input_y_N()) {
                    bodies.clear();
                    std::cout << "All bodies deleted." << std::endl;
                } else {
                    std::cout << "No bodies deleted." << std::endl;
                }
                break;
            }
            case Command::SET_DT: {
                dt = input_double(" dt");
                if (dt < 0) {
                    dt = 0.01;
                }
                break;
            }
            case Command::SET_STEPS: {
                steps = input_int(" steps");
                if (steps < 1) {
                    steps = 1;
                }
                break;
            }
            case Command::RUN: {
                std::cout << "Simulation Settings:" << std::endl;
                std::cout << "  dt: " << dt << std::endl;
                std::cout << "  steps: " << steps << std::endl;
                std::cout << "  filename: " << file_name << std::endl;
                std::cout << "Do you want to start the simulation?" << std::endl;
                if (!input_y_N()) {
                    std::cout << "Simulation Aborted." << std::endl;
                    break;
                }
                ForwardEuler euler = ForwardEuler(dt, steps, bodies, &rate_func);
                std::vector<std::vector<Body>> data = euler.run();
                std::cout << "Simulation complete!" << std::endl;
                save_to_file(file_name, data, dt);
                std::cout << "Data saved to " << file_name << std::endl;
                break;
            }
            case Command::CHANGE_NAME: {
                std::cout << "[string] name: ";
                std::cin >> file_name;
                break;
            }
        }
    }
}

