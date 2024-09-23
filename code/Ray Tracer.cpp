#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <fstream>


// NOTE: SPHERE-RELATED MATHS. Explained in Ray Tracing in One Weekend here:
//      https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
// TLDR:
// * The equation of a sphere at the origin (x^2 + y^2 + z^2 = r^2) can be used 
//      to determine if a point is inside (< r^2), on (= r^2), or outside (> r^2) a sphere.
// * Applying this to an arbitrary point (Cx, Cy, Cz) allows that equation to be
//      rewritten as: (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2
// * The dot product definition allows this to be written in vector form:
//      (C - P) . (C - P) = r^2
// * Representing a ray as a function P(t) = Q + td (see ray class) allows this adjustment,
//   which is satisfied at some t value, as long as the ray hits the sphere:
//      (C - P(t)) . (C - P(t)) = r^2
// * Replace P(t) with Q + td to begin solving for t:
//      (C - (Q+td)) . (C - (Q+td)) = r^2
// * Expand this equation out to a quadratic equation (ax^2 + bx + c = 0):
//      (d.d)t^2 -2td.(C-Q) + (C-Q).(C-Q) - r^2 = 0
// * Take the discriminant (b^2 - 4ac) from the quadratic formula (x = -b +/- sqrt(b^2 - 4ac) / 2a)
//   to find how many real solutions the equation has. 
// NOTE: a vector dotted with itself is equal to the squared length of that vector
//      a = d.d = d.length_squared()
//      b = -2d.(C-Q)
//      c = (C-Q).(C-Q) - r^2 = (C-Q).length_squared() - r^2
//      (reminder: d = direction, C is the sphere centre, Q is the origin)
//    * If the discriminant (b^2 - 4ac) is:
//        - positive  -> two real solutions (goes through the sphere)
//        - zero      -> one real solution (is a tangent to the sphere)
//        - negative  -> no real solutions (misses the sphere entirely)
// * HOWEVER, b having a factor of -2 allows for a simplification. 
//   (Explanation: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/simplifyingtheray-sphereintersectioncode)
//      Replacing b with -2h in the quadratic formula results in:
//      x = (h +/- sqrt(h^2 - ac)) / a
// * Using the previous b coefficient, solving for h results in:
//      h = d.(C-Q)
// 
// Visualisation (as seen in the Ray Tracing in One Weekend, Chapter 5.1): 
//      https://raytracing.github.io/images/fig-1.05-ray-sphere.jpg

/// <summary>
/// Determines if a ray hits a sphere using the quadratic formula
/// </summary>
/// <param name="center">Centre of the given sphere</param>
/// <param name="radius">Radius of the given sphere</param>
/// <param name="r">Ray</param>
/// <returns>lowest t value if hit, -1 if not</returns>
double hit_sphere(const point3& center, double radius, const ray& r)
{
    // Vector from origin to centre of sphere
    vec3 oc = center - r.origin();

    // See NOTE above this function to explain why quadratic formula is being used
    // Any vector dotted against itself = vector's length squared
    auto a = r.direction().length_squared();
    // b with factor of -2 removed (replaced b with -2h in quadratic formula)
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    // Part of quadratic formula (b^2 - 4ac), adjusted due to h replacing b
    auto discriminant = h*h - a*c;

    // If discriminant < 0, ray missed.
    // Else, return lowest t value
    if (discriminant < 0)
    {
        // Didn't hit sphere
        return -1.0;
    }
    else
    {
        // Quadratic formula solving for x
        // but ADJUSTED FOR H: x = ( h +/- sqrt(h^2 - ac) )  /  a
        // Only return the lowest value as this will be closest to the camera
        return (h - std::sqrt(discriminant)) / a;
    }
}

// Returns a ray colour given a passed ray
color ray_color(const ray& r)
{
    // t value for where ray intersects sphere at Z = -1 with radius 0.5 units
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    // t = -1 if it missed, so if it hits return a specific colour
    if (t > 0.0)
    {
        // Unit vector from sphere centre to hit point
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        // Returns a colour representing direction (values between 0 and 1)
        // x = red, y = blue, z = green
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
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

int main()
{
    // IMAGE
    /*int image_width = 256;
    int image_height = 256;*/

    // This is the IDEAL aspect ratio, but due to int rounding it is not
    // necessarily the *actual* aspect ratio.
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image heigth, and enure that it's at least 1.
    // width / height = ratio, this is just solving for the height using the 
    // ideal aspect ratio
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // CAMERA
    auto focal_length = 1.0;
    // Viewport widths less than one are okay since they are real valued (not 
    // just an integer).
    auto viewport_height = 2.0;
    // Width = [aspect ratio] * height. Therefore multiply the height by the 
    // *real* aspect ratio to find the width.
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);
    
    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    // Both vectors start in the top left.
    auto viewport_u = vec3(viewport_width, 0, 0); // from (top) LEFT TO (top) RIGHT
    auto viewport_v = vec3(0, -viewport_height, 0); // from TOP (left) TO BOTTOM (left)

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    // Splits viewport into image_width number of pixels across.
    auto pixel_delta_u = viewport_u / image_width;
    // Splits viewport into image_height number of pixels down.
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    // Make relative to camera_center
    // Use the viewport_u and viewport_v vectors rather than inputting viewport_width and 
    // viewport_height directly to minimise errors if these values ever change.
    auto viewport_upper_left = camera_center
                             - vec3(0 , 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    // REMEMBER pixel_delta_v is already in the negative y direction due to viewport_v vector 
    // being negative y from the start
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    // RENDER
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

        // inner loop for columns
        for (int i = 0; i < image_width; i++)
        {
            // Centre of current pixel. Calculated relative to pixel00. Add i number of pixel_delta_u
            // and j number of pixel_delta_v to find the current pixel.
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(image_out, pixel_color);
        }
    }
    // Overwrites "Scanlines remaining: x".
    // The tabs are there so that there are enough characters to overwrite 
    // all the characters in the previous statement.
    std::clog << "\rDone.                   \n";
}
