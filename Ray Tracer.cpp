#include <iostream>
#include <fstream>

int main()
{
    // Image
    int image_width = 256;
    int image_height = 256;

    // Render
    // define an output file
    std::ofstream imageOut("output/imageOut.ppm");
    // add header for image file
    imageOut << "P3\n" << image_width << ' ' << image_height << "\n255\n";


    // for loop works across each column (i) in a row (j) at a time
    // outer loop for rows
    for (int j = 0; j < image_height; j++)
    {
        // outputs number of rows remaining. Refreshed each loop.
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        // inner loop for columns
        for (int i = 0; i < image_width; i++)
        {
            // Column index divided by total image width to get a colour value between 0 and 1.
            // Image_width must be reduced by one as first (left-most) pixel is index 0
            // and last (right-most) pixel is index 255
            // 
            // Pixels are least red on the left, most red on the right.
            auto r = double(i) / (image_width - 1);
            // Pixels are least green at the top, most green at the bottom.
            auto g = double(j) / (image_height - 1);
            // Pixels always have zero blue input.
            auto b = 0.0;

            // Integer value, converting the above 0 to 1 values to 0 to 256.
            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            // Outputs each pixel's RGB values from 0 to 255 to the file.
            imageOut << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::clog << "\rDone.               \n";
}

