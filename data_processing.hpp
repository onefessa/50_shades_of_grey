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
#include "my_structs.hpp"

ScreenData scan_data(const char* myfileName, const char* outputFile){
    ScreenData screendata;
  try{
    std::ifstream myfile(myfileName, std::ios_base::in);
    if (!myfile.is_open()) {
        throw std::invalid_argument("Error! Can't open your " + std::string(myfileName) + " file\n");
    }//Vec3f cam, normal, up;    float screen, limit, alpha; unsigned int width, height;
    std::string key; myfile>>key;
    if (!(key == "cam")) { throw std::invalid_argument("Error! Wrong data_cam " + key + " key\n"); }
    myfile>> screendata.cam.x >> screendata.cam.y >> screendata.cam.z>>key;

    if (!(key == "normal")) { throw std::invalid_argument("Error! Wrong data_normal " + key + " key\n"); }
    myfile>> screendata.normal.x >> screendata.normal.y >> screendata.normal.z>>key;
    screendata.normal.normalize();
    if (!(key == "up")) { throw std::invalid_argument("Error! Wrong data_up " + key + " key\n"); }
    myfile>> screendata.up.x >> screendata.up.y >> screendata.up.z;
    screendata.up.normalize();
    //std::cout<<"camera: "<<screendata.cam<<"normal: "<<screendata.normal<<"up: "<<screendata.up<<"X: "<<screendata.X<<std::endl;
    myfile>>key>>screendata.screen_dist;
    if (!(key == "screen")) { throw std::invalid_argument("Error! Wrong data_screen " + key + " key\n");}
    myfile>>key>> screendata.limit;
    if (!(key == "limit")) { throw std::invalid_argument("Error! Wrong data_limit " + key + " key\n");}
    myfile>>key>> screendata.alpha;
    if (!(key == "alpha")) { throw std::invalid_argument("Error! Wrong data_alpha " + key + " key\n");}
    std::cout<<"screen: "<<screendata.screen_dist<<"\nlimit: "<<screendata.limit<<"\nalpha: "<<screendata.alpha<<std::endl;

    myfile>>key>> screendata.width;
    if (!(key == "width")) { throw std::invalid_argument("Error! Wrong data_width " + key + " key\n"); }
    myfile>>key>> screendata.height;
    if (!(key == "height")) { throw std::invalid_argument("Error! Wrong data_height " + key + " key\n"); }

    std::cout<<"width: "<<screendata.width<<"\nheight: "<<screendata.height<<std::endl;
    screendata.output_file = outputFile;
    screendata.backgroundColor = Vec3i(252, 255, 255);
    screendata.X = cross(screendata.up, screendata.normal).normalize();
    screendata.up = cross(screendata.normal,  screendata.X).normalize();
    std::cout<<"camera: "<<screendata.cam<<"X: "<<screendata.X<<"up: "<<screendata.up<<"normal: "<<screendata.normal<<std::endl;
    myfile.close();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
  return screendata;
}

std::vector<Sphere*>  scan_spheres(const char* myfileName, ScreenData& scr_data){
    std::vector<Sphere*> spheres;
    scr_data.num_sp = 0;
try{
    std::ifstream myfile(myfileName, std::ios_base::in);
    if (!myfile.is_open()) {
        throw std::invalid_argument("Error! Can't open your " + std::string(myfileName) + " file\n");
    }
    std::string key;
    while (myfile >> key) {
        Vec3f curr; float r; Vec3i mat;
        myfile>>key;
        if (!(key == "center:")) {
            throw std::invalid_argument("Error! Wrong data_center " + key + " key\n"); return spheres; }
        myfile>> curr.x>> curr.y >> curr.z; //std::cout<<key<<" "<<curr;
        myfile>>key;
        if (!(key == "radius:")) {
            throw std::invalid_argument("Error! Wrong data_radius " + key + " key\n"); return spheres; }
        myfile>> r; //std::cout<<key<<" "<<r<<"\n";

        Sphere* sp = new Sphere(curr, r);
        std::cout<<"\n\ncenter: "<< sp->center<<"radius: "<<sp->radius<<std::endl;
        spheres.push_back(sp);
        scr_data.num_sp ++;

        float dist = ((spheres.back()->center-scr_data.cam).z)*((spheres.back()->center - scr_data.cam).z);
           // std::cout<< "dist^2: "  << dist << std::endl;
            scr_data.distances.insert(std::make_pair(dist, spheres.back()));
        }

    std::map<int, Sphere*> tmp1;
    std::map<Sphere*, int> poss;
    int i = 1;
    for (std::map<float, Sphere*>::const_iterator it = scr_data.distances.begin(); it != scr_data.distances.end(); ++it) {
        tmp1.insert(std::make_pair(i, it->second));
        //std::cout<<i<<" is a "<<it->second<<"\n";
        i++;
    }
    for (std::map<int, Sphere*>::const_iterator it = tmp1.begin(); it != tmp1.end(); ++it) {
        poss.insert(std::make_pair(it->second, it->first));
      //  std::cout<<it->second<<" is a "<<it->first<<"\n";
    }

    for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it) {
        (*it)->index = poss.find(*it)->second;
    }
    myfile.close();
        /*scr_data.num_sp++;

        float d2 = curr*curr;
        scr_data.distances.insert(std::make_pair(d2, spheres.back()));
    }
    myfile.close();

    /std::multimap<Sphere*, float> swaped;
    for(std::multimap<float, Sphere*>::const_iterator it = scr_data.distances.begin(); it != scr_data.distances.end(); ++it){
        swaped.insert(std::make_pair(it->second, it->first));
    }
    int k = 0;
    for (//std::multimap<float, Sphere*>::const_iterator
         auto it = scr_data.distances.begin(); it != scr_data.distances.end(); ++it){
        k++;
        (it->second).index = k;
        std::cout << it.center << " has index " <<it.index << '\n';
        */
    }

    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

return spheres;
}



