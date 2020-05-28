#pragma once
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

bool trace (const Vec3f &orig, const Vec3f &dir, const std::vector<Sphere*> &spheres,
            float& tNear, Sphere** curr_sph, ScreenData scr_data){
    *curr_sph = nullptr;
//#pragma omp parallel for
    for (size_t k = 0; k < spheres.size(); ++k) {
        float tNear_k = scr_data.limit;
        if (spheres[k]->ray_intersect(orig, dir, tNear_k) && tNear_k < tNear) {
            *curr_sph = spheres[k];
            tNear = tNear_k;
        }
    }
    return (*curr_sph != nullptr); //is_inside
}

Vec3i colourise( const Vec3f &orig, const Vec3f &dir, const std::vector<Sphere*> &spheres,const ScreenData &scr_data){
    Vec3i curr_colour = scr_data.backgroundColor;
    float tnear = scr_data.limit;
    Sphere* curr_sph = nullptr;
    if (trace(orig, dir, spheres, tnear, &curr_sph, scr_data)){
    	if (scr_data.num_sp == 1){
            curr_colour = Vec3i(64, 64, 64);
        }
        else{
            Vec3i T; T.x = (127/(scr_data.num_sp-1)); T.y = (127/(scr_data.num_sp-1)); T.z = (127/(scr_data.num_sp-1));
            curr_colour = Vec3i(64, 64, 64) + T*(curr_sph->index - 1);
        }
    }
    return curr_colour;
}

void render( const ScreenData &scr_data,const std::vector<Sphere*> &spheres){
    std::cout<<"--------------"<<std::endl;
    for (auto it = spheres.begin(); it != spheres.end(); ++it) {
        std::cout<<" ind: "<<(*it)->index<<" is a "<<(*it)->center<< "\n";
    }

    float scale = tan(scr_data.alpha*0.5 * M_PI / 180 )*scr_data.screen_dist;
    float imageAspectRatio = scr_data.width / (float)scr_data.height;

    Vec3f orig; Vec3i tmp;
    int color[3];
    cimg_library::CImg<float> img(scr_data.width, scr_data.height, 1, 3);
//#pragma omp parallel for private(color,tmp)
    for (int j = 0; j < scr_data.height; ++j) {
        for (int i = 0; i < scr_data.width; ++i) {
            float x = (2 * (i + 0.5) / (float)scr_data.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)scr_data.height) * scale;
            Vec3f dir = Vec3f(x, y, -1).normalize();
            dir.normalize();

            tmp = colourise(orig, dir, spheres, scr_data);
            color[0] = tmp[0];
			color[1] = tmp[1];
			color[2] = tmp[2];
            img.draw_point(i, j, color);
        }
    }
    img.save(scr_data.output_file.c_str());
}
