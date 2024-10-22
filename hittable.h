#ifndef HITTABLE_H
#define HITTABLE_H

#include "rt.h"

class material;

class hit_record
{
public:
    point3 p;
    double t;
    shared_ptr<material> mat;
    vec3 normal;
    bool front_face;

    void set_front_face(const ray &r, vec3 outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};

#endif