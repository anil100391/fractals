#include <omp.h>
#include<stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cairo/cairo.h>

#ifndef _NHIMAGE_H
#define _NHIMAGE_H

// -------------------------------------------------------------------------- //
//  Class to map a portion of Cartesian Plane to an Image of given resolution //
// -------------------------------------------------------------------------- //
class nhImage
{
public:

    nhImage( double xmin, double xmax,
             double ymin, double ymax,
             int resX, int resY )
        : p_xMin(xmin)
        , p_xMax(xmax)
        , p_yMin(ymin)
        , p_yMax(ymax)
        , p_resX(resX)
        , p_resY(resY)
        , p_initialized(false)
    {}

    enum PIXEL_CORNER
    {
        CENTER,
        LOWER_LEFT,
        LOWER_RIGHT,
        UPPER_RIGHT,
        UPPER_LEFT,
        ALL_CORNERS
    };

    // Allocate memory for the rgba color space
    virtual bool InitColors( void );

    // Renders this image to a png file
    void RenderImage( const char* fileName, int numThreads = -1 );

    // Gets the point (x,y) lying on pixel px, py which
    // corresponds to coordinate of the provided pixel corner
    bool PointAtPixel( const int px, const int py,
                       double &x, double &y,
                       const PIXEL_CORNER loc = CENTER );

    // Gets the pixel lying on point
    bool PixelAtPoint( const double x, const double y,
                       int &px, int &py );

    bool IsInitialized() const
    {
        return p_initialized;
    }

protected:

    virtual ~nhImage()
    {
        free(p_redCh);
        free(p_blueCh);
        free(p_greenCh);
        free(p_alphaCh);
    }

    // Image resolution
    int     p_resX,
            p_resY;

    // Color channels
    float   *p_redCh,
            *p_greenCh,
            *p_blueCh,
            *p_alphaCh;

    // Geometry corresponding to image
    double  p_xMin,
            p_xMax,
            p_yMin,
            p_yMax;

    bool    p_initialized;

};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhImage::PointAtPixel( const int px, const int py,
                            double &x, double &y,
                            const nhImage::PIXEL_CORNER loc )
{
    if ( px < 0 || px > p_resX || py < 0 || py > p_resY )
    {
        return false;
    }

    double pixelSpanX = ( p_xMax - p_xMin ) / p_resX;
    x = p_xMin + px * pixelSpanX;

    double pixelSpanY = ( p_yMax - p_yMin ) / p_resY;
    y = p_yMin + py * pixelSpanY;

    switch ( loc )
    {
    case CENTER:
        x += 0.5 * pixelSpanX;
        y += 0.5 * pixelSpanY;
        break;
    case LOWER_LEFT:
        y += pixelSpanY;
        break;
    case LOWER_RIGHT:
        x += pixelSpanX;
        y += pixelSpanY;
        break;
    case UPPER_RIGHT:
        x += pixelSpanX;
        break;
    case UPPER_LEFT:
        break;
    default:
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhImage::PixelAtPoint( const double x, const double y, int &px, int &py )
{
    if ( x > p_xMax || x < p_xMin || y > p_yMax || y < p_yMin )
    {
        return false;
    }

    double pixelSpan = ( p_xMax - p_xMin ) / p_resX;
    px = ( x - p_xMin ) / pixelSpan;

    pixelSpan = ( p_yMax - p_yMin ) / p_resY;
    py = ( y - p_yMin ) / pixelSpan;

    return true; 
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhImage::RenderImage( const char* fileName, int numThreads )
{
    // This function is implemented to make use of multiple machine cores
    // to allow faster rendering of large images. It will split the image in
    // number of threads available and and write them to fileName[threadnum]
    if ( p_resX <= 0 || p_resY <= 0 )
    {
        std::cerr<<"Abort... Image resolution does not make sense\n";
        return;
    }

    std::cout<<"Rendering Image of resolution "<<p_resX<<"x"<<p_resY<<std::endl;

    if ( numThreads > 0 )
    {
        omp_set_num_threads(numThreads);
    }
    #pragma omp parallel
    {
    int threadID = omp_get_thread_num();
    if ( threadID == 0 )
    {
        std::cout << "Working with " << omp_get_num_threads() << " threads\n";
    }
    cairo_surface_t    *surface;
    cairo_t            *cr;

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, p_resX, p_resY);
    cr      = cairo_create(surface);
    cairo_scale(cr, 1, 1);

    char localFileName[512];
    sprintf(localFileName, "%s%d", fileName, threadID);
    int startJJ = threadID * ( p_resY / omp_get_num_threads() );
    for ( int jj = startJJ; jj < startJJ + p_resY / omp_get_num_threads(); ++jj )
    {
        for ( int ii = 0; ii < p_resX; ++ii )
        {
            cairo_set_source_rgba(cr, p_redCh[jj * p_resX + ii],
                                      p_greenCh[jj * p_resX + ii],
                                      p_blueCh[jj * p_resX + ii],
                                      p_alphaCh[jj * p_resX + ii]);
            cairo_rectangle(cr, ii, jj, 1, 1);
            cairo_fill(cr);
        }
    }

    cairo_surface_write_to_png(surface, localFileName);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    }
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhImage::InitColors( void )
{
    if ( IsInitialized() )
    {
        return true;
    }

    p_redCh   = (float *) malloc(p_resX * p_resY * sizeof(float));
    p_greenCh = (float *) malloc(p_resX * p_resY * sizeof(float));
    p_blueCh  = (float *) malloc(p_resX * p_resY * sizeof(float));
    p_alphaCh = (float *) malloc(p_resX * p_resY * sizeof(float));

    if ( !p_redCh || !p_greenCh || !p_blueCh || !p_alphaCh )
    {
        std::cout<<"Not enough memory to store color info. "<<std::endl;
        return false;
    }

    for ( int jj = 0; jj < p_resY; ++jj )
    {
        for ( int ii = 0; ii < p_resX; ++ii )
        {
            p_redCh[jj * p_resX + ii]   = 0.0f;
            p_greenCh[jj * p_resX + ii] = 0.0f;
            p_blueCh[jj * p_resX + ii]  = 0.0f;
            p_alphaCh[jj * p_resX + ii] = 0.0f;
        }
    }
    p_initialized = true;
    return true;
}

#endif // _NHIMAGE_H
