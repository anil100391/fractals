#include <ctime>
#include "nhImage.h"

// -------------------------------------------------------------------------- //
// Structure to represent a RGBA color                                        //
// -------------------------------------------------------------------------- //
struct nhColor
{
    nhColor() {}
    nhColor( const float red,
             const float green,
             const float blue,
             const float alpha )
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
        p_red   = rand() /(float) RAND_MAX;
        p_green = rand() /(float) RAND_MAX;
        p_blue  = rand() /(float) RAND_MAX;
        p_alpha = rand() /(float) RAND_MAX;
    }

    float p_red,
          p_green,
          p_blue,
          p_alpha;
};

// -------------------------------------------------------------------------- //
// Class to paint the orbit of a number z = x + iy under some iterative rule  //
// -------------------------------------------------------------------------- //
class nhOrbit : public nhImage
{
public:

    nhOrbit( double xmin, double xmax,
             double ymin, double ymax,
             int resX, int resY )
        : nhImage(xmin, xmax, ymin, ymax, resX, resY)
    {
    }

    virtual ~nhOrbit() {}

    // Maximum number of iterations to do while drawing orbit of z
    void SetOrbitLimit(int limit)
    {
        p_orbitLimit = limit;
    }

    // Updates the color space to match the image file. Hence can be used
    // to draw orbit of new z's over already drawn old orbits
    bool InitializeFromFile( const char *fileName );

    virtual void DrawNewOrbit( const nhColor &color ) = 0;

protected:
    int p_orbitLimit;

};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhOrbit::InitializeFromFile( const char *fileName )
{
    if ( !IsInitialized() && !InitColors() )
    {
        return false;
    }

    cairo_surface_t    *surface = NULL;

    surface = cairo_image_surface_create_from_png(fileName);

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

    unsigned char *row;
    for ( int ii = 0; ii < height; ++ii )
    {
        row = colorInfo + ii * stride;
        for ( int jj = 0; jj < width; ++jj )
        {
            p_blueCh[ii*width+jj]  = int(*row++)/255.0;
            p_greenCh[ii*width+jj] = int(*row++)/255.0;
            p_redCh[ii*width+jj]   = int(*row++)/255.0;
            p_alphaCh[ii*width+jj] = int(*row++)/255.0;
        }
    }
    cairo_surface_destroy(surface);
    std::cout<<"File read successfully\n";
    return true;
}
