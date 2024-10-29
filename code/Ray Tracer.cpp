#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    // WORLD

    // Variable 'world' refers to a list of all hittable objects in the world
    hittable_list world;
    
    // Create materials for the ground, and left, centre and right spheres
    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // Add spheres, assigning them one of the above materials
    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // CAMERA
    camera cam;

    // This is the IDEAL aspect ratio, but due to int rounding it is not
    // necessarily the *actual* aspect ratio.
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    // Define how many samples to use for each pixel
    cam.samples_per_pixel = 100;
    // Defines the max number of times a ray can bounce within the scene.
    cam.max_depth = 50;
    // initialises the camera and renders the scene for each pixel through the nested for loop
    cam.render(world);
}
