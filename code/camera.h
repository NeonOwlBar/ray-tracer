#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

#include <fstream>

// camera class has two main jobs:
//		1) Constructs and dispatches rays into the world
//		2) Uses the results of these rays to construct the rendered image.
class camera
{
public:
    double aspect_ratio = 1.0;	// Ratio of image width over height
    int image_width = 100;		// Rendered image width in pixel count

    void render(const hittable& world)
    {
        initialize();

        // Define an output file.
        std::ofstream image_out("output/imageOut.ppm");
        // Add header for image file.
        image_out << "P3\n" << image_width << ' ' << image_height << "\n255\n";


        // For loop works across each column (i) in a row (j) before moving to the next row.
        // Outer loop for rows.
        for (int j = 0; j < image_height; j++)
        {
            // "\r" moves the cursor to the start of this line, causing it to overwrite any 
            // characters that were there before.
            // Outputs number of rows remaining.
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

            // Inner loop for columns
            for (int i = 0; i < image_width; i++)
            {
                // Centre of current pixel. Calculated relative to pixel00. Add i number of pixel_delta_u
                // and j number of pixel_delta_v to find the current pixel.
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = ray_color(r, world);
                //color pixel_color = color(1.0, 1.0, 1.0);
                write_color(image_out, pixel_color);
            }
        }
        // Overwrites "Scanlines remaining: x".
        // The tabs are there so that there are enough characters to overwrite 
        // all the characters in the previous statement.
        std::clog << "\rDone.                   \n";
    }

private:
    int image_height;   // Rendered imeage height
    point3 center;      // Camera centre
    point3 pixel00_loc; // Location of pixel 0, 0
    vec3 pixel_delta_u; // Offset to pixel to the right
    vec3 pixel_delta_v; // Offset to pixel below

    void initialize()
    {
        // Calculate the image height, and ensure that it's at least 1.
        // width / height = ratio, this is just solving for the height using the 
        // ideal aspect ratio
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        // Determine viewport dimensions
        auto focal_length = 1.0;
        // Viewport widths less than one are okay since they are real valued (not 
        // just an integer).
        auto viewport_height = 2.0;
        // Width = [aspect ratio] * height. Therefore multiply the height by the 
        // *real* aspect ratio to find the width.
        auto viewport_width = viewport_height * (double(image_width) / image_height);
      

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        // Both vectors start in the top left.
        auto viewport_u = vec3(viewport_width, 0, 0); // from (top) LEFT TO (top)
        auto viewport_v = vec3(0, -viewport_height, 0); // from TOP (left) TO BOTTOM (left)

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        // Splits viewport into image_width number of pixels across.
        pixel_delta_u = viewport_u / image_width;
        // Splits viewport into image_height number of pixels down.
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        // Make relative to camera_center
        // Use the viewport_u and viewport_v vectors rather than inputting viewport_width and 
        // viewport_height directly to minimise errors if these values ever change.
        auto viewport_upper_left = 
            center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        // REMEMBER pixel_delta_v is already in the negative y direction due to viewport_v vector 
        // being negative y from the start
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, const hittable& world) const
    {
        hit_record rec;
        // If any object in the world gets hit
        if (world.hit(r, interval(0, infinity), rec))
        {
            // Returns the normal where positive x, y, z represents red, green, 
            // blue, respectively
            return 0.5 * (rec.normal + color(1, 1, 1));
        }

        // Background colour
        // Calculates unit vector by passing the ray's direction vector
        vec3 unit_direction = unit_vector(r.direction());
        // Keeps a between 0 and 1 for multiplication below
        // If y = -1, a = 0.    If y = 1, a = 1. 
        auto a = 0.5 * (unit_direction.y() + 1.0);
        // Common linear interpolation calculation in graphics:
            // blendedValue = (1 - a) * startValue  +  a * endValue
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif
