#ifndef _NH_MANDELBROT_H_
#define _NH_MANDELBROT_H_

#include "nhImage.h"
#include <thread>
#include <functional>
#include <algorithm>
#include <cmath>

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

    struct Job
    {
        Job(nhMandelbrot *image, int beginrow, int endrow)
            : _image(image), _beginrow(beginrow), _endrow(endrow)
        {
        }

        void operator()()
        {
            for ( int ii = _beginrow; ii < _endrow; ++ii )
            {
                _image->ComputeRow(ii);
            }
        }

        nhMandelbrot *_image    = nullptr;
        int           _beginrow = 0;
        int           _endrow   = 0;
    };

    void ComputeRow(int row);

    int IterationsToGetKnocked(int row, int col) const;

    int p_maxIter;
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
int nhMandelbrot::IterationsToGetKnocked(int row, int col) const
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
        count++;
    }
    return count;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhMandelbrot::InitColors( void )
{
    if ( !nhImage::InitColors() )
    {
        return false;
    }

    size_t numThreads = std::thread::hardware_concurrency();
    int jobSize = p_resY / numThreads;
    std::vector<std::thread> threads;

    int startRow = 0;
    for ( int jj = 0; jj < numThreads - 1; ++jj )
    {
        threads.emplace_back(Job(this, startRow, startRow + jobSize));
        startRow += jobSize;
    }

    threads.emplace_back(Job(this, startRow, p_resY));

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return true;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhMandelbrot::ComputeRow(int row)
{
    int jj = row;
    for ( int ii = 0; ii < p_resX; ++ii )
    {
        int count = IterationsToGetKnocked(ii, jj);

        float normi = float(count)/p_maxIter;
        normi = 2.0 * normi - 1.0;
        normi = normi * normi;
        float mixfac = std::exp(-4.0*normi*normi);

        uint8_t *pixel = &p_colorData[4*(jj * p_resX + ii)];
        pixel[0] = 0;
        pixel[1] = (uint8_t)(mixfac * 255);
        pixel[2] = 0;
        pixel[3] = 255;
    }
}

#endif // _NH_MANDELBROT_H_
