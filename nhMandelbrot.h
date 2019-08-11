#ifndef _NH_MANDELBROT_H_
#define _NH_MANDELBROT_H_

#include "nhImage.h"

class nhMandelbrot : public nhImage
{
public:

    nhMandelbrot( double xmin, double xmax,
                  double ymin, double ymax,
                  int resX, int resY, int maxIter )
        : nhImage(xmin, xmax, ymin, ymax, resX, resY)
        , p_maxIter(maxIter)
    {
    }

    virtual ~nhMandelbrot() = default;

    // Manipulates the color space so that to represent each pixel
    // belonging or not belonging to the mandelbrot set.
    bool InitColors( void ) override;

private:

    int p_maxIter;
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhMandelbrot::InitColors( void )
{
    if ( !nhImage::InitColors() )
    {
        return false;
    }

    for ( int jj = 0; jj < p_resY; ++jj )
    {
        for ( int ii = 0; ii < p_resX; ++ii )
        {
            int count = 0;
            double cx = 0.0,
                   cy = 0.0,
                   x0 = 0.0,
                   y0 = 0.0;
            nhImage::PointAtPixel(ii, jj, cx, cy, nhImage::CENTER);
            // Iteration of 0 under f(z) = z^2 + c //
            while ( x0 * x0 + y0 * y0 < 4 && count < p_maxIter )
            {
                double fx = x0 * x0 - y0 * y0 + cx;
                double fy = 2.0 * x0 * y0 + cy;
                x0 = fx;
                y0 = fy;
                count++;
            }
            // count 0 is bluish, 20 is whitish, 30 is orange, maxIter is black
            double rc, gc, bc;
            double weight = 1.0f - count / p_maxIter;
            if ( count < 20 )
            {
                rc = 0.0f * weight + 1.0f * (1.0f - weight) ;
                gc = 0.0f * weight + 1.0f * (1.0f - weight) ;
                bc = 0.2f * weight + 1.0f * (1.0f - weight) ;
            }
            else if ( count < 30 )
            {
                rc = 1.0f * weight + 0.97f * (1.0f - weight) ;
                gc = 1.0f * weight + 0.60f * (1.0f - weight) ;
                bc = 1.0f * weight + 0.00f * (1.0f - weight) ;
            }
            else
            {
                rc = 0.97f * weight + 0.0f * (1.0f - weight) ;
                gc = 0.60f * weight + 0.0f * (1.0f - weight) ;
                bc = 0.00f * weight + 0.0f * (1.0f - weight) ;
            }

            uint8_t *pixel = &p_colorData[4*(jj * p_resX + ii)];
            pixel[0] = (uint8_t)(rc * 255);
            pixel[1] = (uint8_t)(gc * 255);
            pixel[2] = (uint8_t)(bc * 255);
            pixel[3] = 255;
        }
    }
    return true;
}

#endif // _NH_MANDELBROT_H_
