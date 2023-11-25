#pragma once

#include "body.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void save_to_file(std::string name, std::vector<std::vector<Body>> data, double dt);
