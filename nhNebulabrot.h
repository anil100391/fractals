#ifndef _NH_NEBULABROT_H_
#define _NH_NEBULABROT_H_

#include "nhImage.h"
#include <cmath>
#include <ctime>
#include <fstream>
#include <algorithm>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class nhNebulabrot : public nhImage
{
public:

    nhNebulabrot( double xmin, double xmax,
                  double ymin, double ymax,
                  int resX, int resY, int maxIter, int minIter )
        : nhImage(xmin, xmax, ymin, ymax, resX, resY)
        , p_maxIter(maxIter)
        , p_minIter(minIter)
    {
    }

    virtual ~nhNebulabrot() = default;

    // Manipulates the color space so that to represent each pixel
    // belonging or not belonging to the mandelbrot set.
    bool InitColors( void ) override;

private:

    int IterationsToGetKnocked(int row, int col) const;
    static bool PointHasOrbitBetween( double cx,
                                      double cy,
                                      int minIter,
                                      int maxIter );
    void GetAStartingPoint( double &x, double &y ) const;

    int p_maxIter;
    int p_minIter;
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
int nhNebulabrot::IterationsToGetKnocked(int row, int col) const
{
    int count = 0;
    double cx = 0.0, cy = 0.0, x0 = 0.0, y0 = 0.0;
    nhImage::PointAtPixel(row, col, cx, cy, nhImage::CENTER);
    // Iteration of 0 under f(z) = z^2 + c //
    while ( x0 * x0 + y0 * y0 < 4 && count < p_maxIter )
    {
        double fx = x0 * x0 - y0 * y0 + cx;
        double fy = 2.0 * x0 * y0 + cy;
        x0 = fx;
        y0 = fy;
        ++count;
    }
    return count;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void nhNebulabrot::GetAStartingPoint( double &x, double &y ) const
{
    x = 0.0;
    y = 0.0;

    do
    {
        x = rand() / (float) RAND_MAX;
        y = rand() / (float) RAND_MAX;
        x = (p_xMax - p_xMin) * x + p_xMin;
        y = (p_yMax - p_yMin) * y + p_yMin;
    }
    while (!PointHasOrbitBetween(x, y, p_minIter, p_maxIter));
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhNebulabrot::PointHasOrbitBetween( double cx,
                                         double cy,
                                         int minIter,
                                         int maxIter )
{
    bool circle_cond = (cx + 1) * (cx + 1) + cy * cy < 0.0625;

    if ( circle_cond )
    {
        return false;
    }

    double p = std::sqrt( (cx - 0.25) * (cx - 0.25) + cy * cy );
    bool cardioid_cond = cx - (p - 2*p*p + 0.25) < 0;

    if ( cardioid_cond )
    {
        return false;
    }

    double x0 = 0.0, y0 = 0.0;
    int count = 0;
    // Iteration of 0 under f(z) = z^2 + c //
    while ( x0 * x0 + y0 * y0 < 4 && count < maxIter )
    {
        double fx = x0 * x0 - y0 * y0 + cx;
        double fy = 2.0 * x0 * y0 + cy;
        x0 = fx;
        y0 = fy;
        ++count;
    }

    return (count >= minIter) && (count < maxIter);
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhNebulabrot::InitColors( void )
{
    if ( !nhImage::InitColors() )
    {
        return false;
    }

    clock_t seed = clock();
    srand(seed);

    std::ofstream outfile("nebulabrot_points.txt");
    int samples = 1000000;
    for ( int ii = 0; ii < samples; ++ii )
    {
        double cx = 0, cy = 0;
        GetAStartingPoint(cx, cy);
        outfile << cx << "\t" << cy << "\n";

        // Iteration of 0 under f(z) = z^2 + c //
        int count = 0;
        double x0 = 0.0, y0 = 0.0;
        while ( x0 * x0 + y0 * y0 < 4 && count < p_maxIter )
        {
            ++count;

            double fx = x0 * x0 - y0 * y0 + cx;
            double fy = 2.0 * x0 * y0 + cy;
            x0 = fx;
            y0 = fy;

            int px = 0, py = 0;
            if ( nhImage::PixelAtPoint(x0, y0, px, py) )
            {
                // increase pixel brightness
                uint8_t *pixel = &p_colorData[4*(py * p_resX + px)];
                uint32_t &hitCount = *reinterpret_cast<uint32_t *>(pixel);
                ++hitCount;
            }
        }
    }

    uint32_t maxHits = 0;
    for ( size_t ii = 0; ii < p_colorData.size() / 4; ++ii )
    {
        uint8_t *pixel = &p_colorData[4 * ii];
        uint32_t hitCount = *reinterpret_cast<uint32_t *>(pixel);
        if ( hitCount > maxHits )
        {
            maxHits = hitCount;
        }
    }

    if ( maxHits == 0 )
        return false;

    for ( size_t ii = 0; ii < p_colorData.size() / 4; ++ii )
    {
        uint8_t *pixel = &p_colorData[4 * ii];
        uint32_t hitCount = *reinterpret_cast<uint32_t *>(pixel);
        if ( hitCount == 0 )
        {
            pixel[3] = 255;
            continue;
        }
        float density = 1.0f * hitCount / maxHits;
        density = std::pow(density, 0.85f);
        // density *= 2.0f;
        std::clamp(density, 0.0f, 1.0f);
        uint8_t intensity = static_cast<uint8_t>(255 * density);

        if ( intensity != 0 )
        {
            pixel[0] = intensity;
            pixel[1] = intensity;
            pixel[2] = intensity;
        }
        pixel[3] = 255;
    }
    return true;
}

#endif // _NH_NEBULABROT_H_
