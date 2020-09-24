#ifndef _NHORBIT_H_
#define _NHORBIT_H_

#include <ctime>
#include <cstring>
#include "nhImage.h"

// -----------------------------------------------------------------------------
// Structure to represent a RGBA color
// -----------------------------------------------------------------------------
struct nhColor
{
    nhColor() {}
    nhColor( const uint8_t red,
             const uint8_t green,
             const uint8_t blue,
             const uint8_t alpha )
    {
        p_red = red;
        p_green = green;
        p_blue = blue;
        p_alpha = alpha;
    }

    void Randomize()
    {
        time_t seed = clock();
        srand(seed);
        p_red   = (uint8_t)(rand() * 255.0f / RAND_MAX);
        p_green = (uint8_t)(rand() * 255.0f / RAND_MAX);
        p_blue  = (uint8_t)(rand() * 255.0f / RAND_MAX);
        p_alpha = (uint8_t)(rand() * 255.0f / RAND_MAX);
    }

    uint8_t    p_red;
    uint8_t    p_green;
    uint8_t    p_blue;
    uint8_t    p_alpha;
};

// -----------------------------------------------------------------------------
// Class to paint the orbit of a number z = x + iy under some iterative rule
// -----------------------------------------------------------------------------
class nhOrbit : public nhImage
{
public:

    nhOrbit( double xmin, double xmax,
             double ymin, double ymax,
             int resX, int resY )
        : nhImage(xmin, xmax, ymin, ymax, resX, resY)
    {
        Paint();
    }

    virtual ~nhOrbit() = default;

    // Updates the color space to match the image file. Hence can be used
    // to draw orbit of new z's over already drawn old orbits
    bool InitializeFromFile( const char *fileName );

    virtual void DrawNewOrbit( const nhColor &color ) = 0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool nhOrbit::InitializeFromFile( const char *fileName )
{
    if ( !Paint() )
    {
        return false;
    }

    cairo_surface_t *surface = cairo_image_surface_create_from_png(fileName);

    if ( !surface )
    {
        std::cout<<"File could not be read\n";
        return false;
    }

    unsigned char *colorInfo = cairo_image_surface_get_data (surface);
    int width = cairo_image_surface_get_width(surface);
    int height = cairo_image_surface_get_height(surface);
    int stride = cairo_image_surface_get_stride(surface);

    std::cout<<"Width: "<<width<<std::endl;
    std::cout<<"Height: "<<height<<std::endl;
    std::cout<<"Stride: "<<stride<<std::endl;

    memcpy( &p_colorData[0], colorInfo, p_colorData.size() * sizeof(uint8_t) );

    cairo_surface_destroy(surface);
    std::cout<<"File read successfully\n";
    return true;
}

#endif // _NHORBIT_H_