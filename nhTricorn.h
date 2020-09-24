#ifndef _NH_TRICORN_H_
#define _NH_TRICORN_H_

#include "nhImage.h"

class nhTricorn : public nhImage
{
public:

    nhTricorn( double xmin, double xmax,
               double ymin, double ymax,
               int resX, int resY, int maxIter )
        : nhImage(xmin, xmax, ymin, ymax, resX, resY)
        , p_maxIter(maxIter)
    {
        Paint();
    }

    virtual ~nhTricorn() = default;

    bool Paint( void ) override;

private:

    int p_maxIter;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool nhTricorn::Paint( void )
{
    if ( !nhImage::Paint() )
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
            // Iteration of 0 under f(z) = conj(z)^2 + c //
            while ( x0 * x0 + y0 * y0 < 4 && count < p_maxIter )
            {
                double fx = x0 * x0 - y0 * y0 + cx;
                double fy = -2.0 * x0 * y0 + cy;
                x0 = fx;
                y0 = fy;
                count++;
            }
            double rc, gc, bc;
            double weight = 1.0f - count / p_maxIter;

            rc = 1.0f * weight + 0.0f * (1.0f - weight) ;
            gc = 1.0f * weight + 0.0f * (1.0f - weight) ;
            bc = 1.0f * weight + 0.0f * (1.0f - weight) ;

            uint8_t *pixel = &p_colorData[4*(jj * p_resX + ii)];
            pixel[0] = (uint8_t)(rc * 255);
            pixel[1] = (uint8_t)(gc * 255);
            pixel[2] = (uint8_t)(bc * 255);
            pixel[3] = 255;
        }
    }
    return true;
}

#endif // _NH_TRICORN_H_
