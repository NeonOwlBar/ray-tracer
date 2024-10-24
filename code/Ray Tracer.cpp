#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
    // WORLD

    // Variable 'world' refers to a list of all hittable objects in the world
    hittable_list world;
    // Original sphere
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    // Acts as the ground, but it still clearly a sphere
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // CAMERA
    camera cam;

    // This is the IDEAL aspect ratio, but due to int rounding it is not
    // necessarily the *actual* aspect ratio.
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    // Define how many samples to use for each pixel
    cam.samples_per_pixel = 100;
    // initialises the camera and renders the scene for each pixel through the nested for loop
    cam.render(world);
}
