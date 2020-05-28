#define M_PI 3.14159265358979323846
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <memory>

#include <utility>
#include <string>
#include <vector>
#include <map>

#include <chrono>
#include "omp.h"
#include "CImg.h"
#include "geometry.hpp"
#include "data_processing.hpp"
#include "fun.hpp"

void autotest() { // Front view
    ScreenData scr_data = scan_data("screen_data.txt", "test.bmp");
    std::vector<Sphere*> spheres = scan_spheres("spheres_data.txt", scr_data);
    if (spheres.size() == 0) {
        return;
    }
    render(scr_data, spheres);

    for (size_t i = 0; i < spheres.size(); i++) {
        delete spheres[i];
    }

    std::cout << "test: OK " << std::endl;
}
void autotest2() {

    ScreenData scr_data = scan_data("screen_data2.txt", "test2.bmp");
    std::vector<Sphere*> spheres = scan_spheres("spheres_data2.txt", scr_data);
    if (spheres.size() == 0) {
        return;
    }
    render(scr_data, spheres);

    for (size_t i = 0; i < spheres.size(); i++) {
        delete spheres[i];
    }
    std::cout << "test2: OK " << std::endl;
}

void autotest3() {

    ScreenData scr_data = scan_data("screen_data2.txt", "test3.bmp");
    std::vector<Sphere*> spheres = scan_spheres("spheres_data3.txt", scr_data);
    if (spheres.size() == 0) {
        return;
    }
    std::cout<<(scr_data.num_sp-1)<<std::endl;
    Vec3i T; T.x = (127/(scr_data.num_sp-1)); T.y = (127/(scr_data.num_sp-1)); T.z = (127/(scr_data.num_sp-1));
    std::cout<<T<<std::endl;
    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        (*it)->material = Vec3i(64, 64, 64) + T*((*it)->index - 1);
        std::cout<<" ind: "<<(*it)->index<<" is a "<<(*it)->center<< "\t\t"<<(*it)->material<<"\n";
    }
    render(scr_data, spheres);
    for (size_t i = 0; i < spheres.size(); i++) {
        delete spheres[i];
    }
    std::cout << "test3: OK " << std::endl;
}
