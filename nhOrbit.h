#include <ctime>
#include <functional>
#include "nhImage.h"

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
using fractalIterator = std::function<void(double&, double&, double, double)>;

// -------------------------------------------------------------------------- //
// Class to paint the orbit of a number z = x + iy under some iterative rule  //
// -------------------------------------------------------------------------- //
class nhOrbit : public nhImage
{
public:

    nhOrbit( const fractalIterator &iterator,
             double xmin, double xmax,
             double ymin, double ymax,
             unsigned int resX, unsigned int resY )
        : _numIter(0),
          _iterator(iterator),
          nhImage(xmin, xmax, ymin, ymax, resX, resY)
    {
    }

    // Updates the color space to match the image file. Hence can be used
    // to draw orbit of new z's over already drawn old orbits
    bool InitializeFromFile(const char *fileName);

    void DrawNewOrbit(const nhColor &color);

    void SetIterations(unsigned int limit) noexcept;

    void GetRandomPoint(double &x, double &y) const noexcept;

private:

    unsigned int    _numIter;
    fractalIterator _iterator;
};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhOrbit::InitializeFromFile(const char *fileName)
{
    // initialize the pixels
    InitColors();
    return true;
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhOrbit::DrawNewOrbit(const nhColor &color)
{
    double cx = 0.0, cy = 0.0;
    GetRandomPoint(cx, cy);

    double x = cx, y = cy;

    unsigned int iter = 0;
    while (++iter < _numIter)
    {
        unsigned int px = 0, py = 0;
        if ( PixelAtPoint(x, y, px, py) )
        {
            BlendPixel(px, py, color);
        }
        _iterator(x, y, cx, cy);
    }
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhOrbit::SetIterations(unsigned int limit) noexcept
{
    _numIter = limit;
}

// -------------------------------------------------------------------------- //
void nhOrbit::GetRandomPoint(double &x, double &y) const noexcept
{
}
