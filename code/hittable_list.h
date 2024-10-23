#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

// for vector class template (arrays that can change in size)
#include <vector>

/* A shared pointer is a pointer to a type, with a reference counter.
Any shared pointer that points to an object already owned by another shared 
pointer causes the counter to be incremented. When one of these shared pointers
is destroyed, the counter is decremented. Once the counter reaches zero, the 
memory which the object occupies is deleted.
Shared pointers will be used in this project to allow multiple geometries to 
share a common instance (e.g. multiple spheres using the same colour material).
*/

class hittable_list : public hittable
{
public:
    // Declares a vector of shared pointer hittable objects
    // The objects in this vector would be geometries (types that derive from hittable)
    std::vector<std::shared_ptr<hittable>> objects;

    // Default constructor
    hittable_list() {}
    // Constructor with passed object pointer adds it to objects vector
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    // Erases data in objects vector
    void clear() { objects.clear(); }

    // adds a given pointer to the objects pointer vector
    void add(std::shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    // Override of pure virtual function in hittable class
    // Returns whether any of the objects in the list have been hit
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        // Temporary hit data storage
        hit_record temp_rec;
        // Whether the passed ray hit anything
        bool hit_anything = false;
        // stores t value for closest hit point
        auto closest_so_far = ray_t.max;

        // Cycle through all objects in vector
        for (const auto& object : objects)
        {
            // As the object is a geometry, this would call the geometry's hit function
            // e.g. sphere.hit()
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                // If got this far, something has been hit
                hit_anything = true;
                // Retrieve t value of hit
                closest_so_far = temp_rec.t;
                // Add temporary data to the passed hit_record instance
                rec = temp_rec;
            }
        }
        // If any object in the list was hit, this returns true
        return hit_anything;
    }
};

#endif
