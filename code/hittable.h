#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

class hit_record
{
public:
    // Point of intersection
    point3 p;
    // Normal vector
    vec3 normal;
    // t value at intersection closest to camera
    double t;
    // whether the front of a face is outside (true) or inside (false) the hittable
    bool front_face;

    // Sets the hit record normal vector.
    // NOTE: the parameter 'outward_normal' is assumed to have UNIT length
    void set_face_normal(const ray& r, const vec3& outward_normal)
    {

        // ray and face normal pointing OPPOSITE directions? Then dot < 0 (ray is OUTSIDE)
        front_face = dot(r.direction(), outward_normal) < 0;
        // normal is outward normal if front_face is facing outward, otherwise invert it.
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    // Default destructor is generated automatically (doesn't use a user-provided
    // destructor), so hittable is still a trivial type
    virtual ~hittable() = default;

    // Pure virtual function (= 0) - a definition MUST be provided in a derived class.
    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif