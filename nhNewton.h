#ifndef _NH_NEWTON_H_
#define _NH_NEWTON_H_

#include "nhImage.h"
#include <thread>
#include <functional>
#include <algorithm>
#include <cmath>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class nhNewton : public nhImage
{
public:

    nhNewton( double xmin, double xmax,
              double ymin, double ymax,
              int resX, int resY, int maxIter )
        : nhImage(xmin, xmax, ymin, ymax, resX, resY)
        , p_maxIter(maxIter)
    {
    }

    virtual ~nhNewton() = default;

    bool InitColors( void ) override;

private:

    struct Job
    {
        Job(nhNewton *image, int beginrow, int endrow)
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

        nhNewton *_image    = nullptr;
        int           _beginrow = 0;
        int           _endrow   = 0;
    };

    void ComputeRow(int row);

    int IterationsToRoot(int row, int col, int &root) const;

    int p_maxIter;
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
struct complex
{
    complex() = default;
    complex(float real, float imag) : x(real), y(imag) {}

    float x = 0;
    float y = 0;
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
static float absSq(const complex& z)
{
    return z.x * z.x + z.y * z.y;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static complex conjugate(const complex& z)
{
    complex zbar = z;
    zbar.y *= -1.0;
    return zbar;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static complex mult(const complex& a, const complex& b)
{
    complex res;
    res.x = a.x * b.x - a.y * b.y;
    res.y = a.x * b.y + b.x * a.y;
    return res;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
static complex exponent(const complex& z, int n)
{
    if ( n == 1 )
        return z;

    complex zn = z;
    int ii = 1;
    for ( ii = 1; ii < n; ++ii )
    {
        zn = mult(zn, z);
    }

    return zn;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
int nhNewton::IterationsToRoot(int row, int col, int &root) const
{
    // z_n+1 -> z_n - f(z_n) / f'(z_n)
    // f: z^3 -2z + 2
    // f': 3z^2 - 2

    root = -1;
    complex roots[3];
    roots[0] = complex(-1.76929235f, 0.0f);
    roots[1] = complex(0.88464618f, 0.58974281f);
    roots[2] = complex(0.88464618f, -0.58974281f);

    float tol = 0.00001;

    double cx, cy;
    nhImage::PointAtPixel(row, col, cx, cy, nhImage::CENTER);
    complex z(cx, cy);

    for ( int i = 0; i < p_maxIter; ++i )
    {
        complex zn;
        zn.x = z.x;
        zn.y = z.y;

        complex fz = exponent(zn, 3);
        fz.x += 2.0;
        fz.x -= 2*zn.x;
        fz.y -= 2*zn.y;

        complex fprimez = exponent(zn,2);
        fprimez.x *= 3.0;
        fprimez.x -= 2.0;
        fprimez.y *= 3.0;

        complex fzbyfprimez = mult(fz, conjugate(fprimez));
        float len = absSq(fprimez);
        fzbyfprimez.x /= len;
        fzbyfprimez.y /= len;

        // apply newton's iteration
        z.x = z.x - fzbyfprimez.x;
        z.y = z.y - fzbyfprimez.y;

        int j;
        for ( j = 0; j < 3; ++j)
        {
            complex diff;
            diff.x = z.x - roots[j].x;
            diff.y = z.y - roots[j].y;
            if ( absSq(diff) < tol * tol )
            {
                root = j;
                break;
            }
        }

        if ( root != -1 )
            return i;
    }
    return p_maxIter;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhNewton::InitColors( void )
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
void nhNewton::ComputeRow(int row)
{
    int jj = row;
    const float a0 = 0.233f;
    const float a1 = 1.609f;
    for ( int ii = 0; ii < p_resX; ++ii )
    {
        int root = -1;
        int count = IterationsToRoot(ii, jj, root);

        float normi = float(count)/p_maxIter;
        normi = std::pow(normi, a1);
        normi = 2.0 * normi - 1.0;
        float mixfac = a0 - std::exp(-2.0*normi*normi);

        uint8_t *pixel = &p_colorData[4*(jj * p_resX + ii)];
        pixel[0] = 0;
        pixel[1] = 0;
        pixel[2] = 0;
        pixel[3] = 255;
        if ( root != -1 )
            pixel[root] = (uint8_t)(mixfac * 10 * 255);
    }
}

#endif // _NH_NEWTON_H_
