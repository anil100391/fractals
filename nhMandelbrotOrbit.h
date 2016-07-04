#include <cmath>
#include "nhOrbit.h"

// -------------------------------------------------------------------------- //
// Class to draw orbit of z under iteration of mandelbrot rule z = z^2 + c    //
// -------------------------------------------------------------------------- //
class nhMandelbrotOrbit : public nhOrbit
{
public:

    nhMandelbrotOrbit( double xmin, double xmax,
                       double ymin, double ymax,
                       int resX, int resY )
        : nhOrbit(xmin, xmax, ymin, ymax, resX, resY)
    {
    }

    virtual ~nhMandelbrotOrbit() {}

    virtual void DrawNewOrbit( const nhColor &color );


private:
    void GetAStartingPoint( double &x, double &y ) const;
    int  IsInMandelbrotSet( const double &x,
                            const double &y,
                            const int &maxIter ) const;
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhMandelbrotOrbit::DrawNewOrbit( const nhColor &color )
{
    double x = 0.0f,
           y = 0.0f;

    GetAStartingPoint(x,y);

    // recursively process this starting point by the mandelbrot function
    //         z_n+1 = z_n * z_n + starting point
    // change the color information stored at each pixel starting point lands
    // going through the above transformation

    double x0 = 0.0f;
    double y0 = 0.0f;
    int px, py, pixelIndex, count = 0;
    while ( x0 * x0 + y0 * y0 < 4 && count < p_orbitLimit )
    {
        double fx = x0 * x0 - y0 * y0 + x;
        double fy = 2.0 * x0 * y0 + y;
        x0 = fx;
        y0 = fy;
        count++;

        nhImage::PixelAtPoint(x0, y0, px, py);
        // Average the color info at this pixel
        pixelIndex = py * p_resX + px;
        p_redCh[pixelIndex]   += color.p_red;
        p_greenCh[pixelIndex] += color.p_green;
        p_blueCh[pixelIndex]  += color.p_blue;
        p_alphaCh[pixelIndex] += color.p_alpha;
        p_redCh[pixelIndex]   *= 0.5f;
        p_greenCh[pixelIndex] *= 0.5f;
        p_blueCh[pixelIndex]  *= 0.5f;
        p_alphaCh[pixelIndex] *= 0.5f;
    }

}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhMandelbrotOrbit::GetAStartingPoint( double &x, double &y ) const
{
    x = 0.0f;
    y = 0.0f;

    clock_t seed = clock();
    srand(seed);

    do
    {
        x = rand() / (float) RAND_MAX;
        y = rand() / (float) RAND_MAX;
        x = (p_xMax - p_xMin) * x + p_xMin;
        y = (p_yMax - p_yMin) * y + p_yMin;
    }
    while ( !IsInMandelbrotSet(x,y, p_orbitLimit) );
    //while ( IsInMandelbrotSet(x,y, p_orbitLimit) != 2 );
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
int nhMandelbrotOrbit::IsInMandelbrotSet( const double &cx,
                                           const double &cy,
                                           const int &maxIter ) const
{
    // returns 1 if (cx, cy) is inside the cardoid or circle features
    // returns 2 if orbit of (cx, cy) remain bounded for maxIter
    // returns 0 if orbit of (cx, cy) gets unbounded before maxIter
    double x0 = 0.0,
           y0 = 0.0;

    double p = sqrt( (cx - 0.25) * (cx - 0.25) + cy * cy );
    bool cardioid_cond = cx - (p - 2*p*p + 0.25) < 0;

    if ( cardioid_cond )
    {
        return 1;
    }

    bool circle_cond = (cx + 1) * (cx + 1) + cy * cy < 0.0625;

    if ( circle_cond )
    {
        return 1;
    }

    int count = 0;
    // Iteration of 0 under f(z) = z^2 + c //
    while ( x0 * x0 + y0 * y0 < 4 && count < maxIter )
    {
        double fx = x0 * x0 - y0 * y0 + cx;
        double fy = 2.0 * x0 * y0 + cy;
        x0 = fx;
        y0 = fy;
        count++;
    }

    if ( count == maxIter )
    {
        return 2;
    }

    return 0;
}
