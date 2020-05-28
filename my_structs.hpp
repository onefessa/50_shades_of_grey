#pragma once
#include <utility>
#include <string>
#include <map>
#include "omp.h"
#include "CImg.h"
#include "geometry.hpp"

struct Sphere {
    Vec3f center;
    float radius;
    Vec3i material;
    int index;

    Sphere(){};
    Sphere(const Vec3f &c, const float &r) : center(c), radius(r){}
    ~Sphere(){};

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float& t0) const;
};

struct ScreenData{
    int width;
    int height;
    float alpha;
    Vec3i backgroundColor;
    double screen_dist;
    Vec3f up;
    Vec3f cam;
    Vec3f normal;
    Vec3f X;
    float limit;
    std::string output_file;
    int num_sp;
    std::multimap <float, Sphere*, std::less<float> > distances;
};

bool Sphere::ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const {
    Vec3f L = center - orig;
    float Lcos = L*dir;
    float d2 = L*L - Lcos*Lcos;
    if (d2 > radius*radius) return false;
    float thc = sqrtf(radius*radius - d2);
    t0       = Lcos - thc;      // peresec
    float t1 = Lcos + thc;      // peresec
    if (t0 < 0) t0 = t1;        // kasatel
    if (t0 < 0) return false;
    return true;
}
