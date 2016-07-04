#include "nhImage.h"

#ifndef _NH_TRICORN_H
#define _NH_TRICORN_H

class nhTricorn : public nhImage
{
public:

    nhTricorn( double xmin, double xmax,
               double ymin, double ymax,
               int resX, int resY, int maxIter )
        : nhImage(xmin, xmax, ymin, ymax, resX, resY)
        , p_maxIter(maxIter)
    {
    }

    virtual ~nhTricorn() {}

    bool InitColors( void );

private:

    int p_maxIter;

};

bool nhTricorn::InitColors( void )
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

            p_redCh[jj * p_resX + ii]    = rc;
            p_greenCh[jj * p_resX + ii]  = gc;
            p_blueCh[jj * p_resX + ii]   = bc;
            p_alphaCh[jj * p_resX + ii]  = 1.0f;
        }
    }
    return true;
}

#endif // _NH_TRICORN_H
