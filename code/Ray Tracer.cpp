#include "vec3.h"
#include "color.h"

#include <iostream>
#include <fstream>

int main()
{
    // Image
    int image_width = 256;
    int image_height = 256;

    // Render
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
            auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.0);
            write_color(image_out, pixel_color);
        }
    }
    // Overwrites "Scanlines remaining: x".
    // The tabs are there so that there are enough characters to overwrite 
    // all the characters in the previous statement.
    std::clog << "\rDone.                   \n";
}
