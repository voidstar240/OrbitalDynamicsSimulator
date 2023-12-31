#include "file_handler.h"

void save_to_file(std::string name, std::vector<std::vector<Body>> data, double dt) {
    std::ofstream file;
    file.open(name);

    // Write header of the csv file
    file << "time,";
    for (size_t i = 0; i < data[0].size(); i++) {
        file << data[0][i].name << " x,";
        file << data[0][i].name << " y,";
        file << data[0][i].name << " z,";
        file << data[0][i].name << " x',";
        file << data[0][i].name << " y',";
        file << data[0][i].name << " z',";
    }
    file << "\n";

    // write all the lines of data
    for (size_t n = 0; n < data.size(); n++) {
        file << static_cast<double>(n) * dt << ",";
        for (size_t i = 0; i < data[n].size(); i++) {
            file << data[n][i].pos.x << ",";
            file << data[n][i].pos.y << ",";
            file << data[n][i].pos.z << ",";
            file << data[n][i].vel.x << ",";
            file << data[n][i].vel.y << ",";
            file << data[n][i].vel.z << ",";
        }
        file << "\n";
    }
    file.close(); // close the file
}
