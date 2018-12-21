#include <vector>
#include <cstdint>
#include "nhRandom.h"

#ifndef _NHIMAGE_H
#define _NHIMAGE_H

// -------------------------------------------------------------------------- //
// Structure to represent a RGBA color                                        //
// -------------------------------------------------------------------------- //
struct nhColor
{
    nhColor() = default;
    nhColor( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha )
        : _red(red), _green(green), _blue(blue), _alpha(alpha)
    {
    }

    void Randomize()
    {
        static nhRandomUniformGenerator rg;
        static std::uniform_int_distribution<> dis(0, 255);
        _red   = dis(rg.Generator());
        _green = dis(rg.Generator());
        _blue  = dis(rg.Generator());
        _alpha = dis(rg.Generator());
    }

    uint8_t    _red   = 0;
    uint8_t    _green = 0;
    uint8_t    _blue  = 0;
    uint8_t    _alpha = std::numeric_limits<uint8_t>::max();
};

// -------------------------------------------------------------------------- //
//  Class to map a portion of Cartesian Plane to an Image of given resolution //
// -------------------------------------------------------------------------- //
class nhImage
{
public:

    nhImage() = delete;
    nhImage(nhImage &&) = delete;
    nhImage(const nhImage &) = delete;
    nhImage& operator = (nhImage &&) = delete;
    nhImage& operator = (const nhImage &) = delete;

    nhImage( double xmin, double xmax,
             double ymin, double ymax,
             unsigned int resX, unsigned int resY )
        : _xMin(xmin)
        , _xMax(xmax)
        , _yMin(ymin)
        , _yMax(ymax)
        , _resX(resX)
        , _resY(resY)
    {
        InitColors();
    }

    // Allocate memory for the rgba color space
    void InitColors( void );

    // Renders this image to a png file
    void RenderImage( const char* fileName, unsigned int numThreads = 1 );

    // Gets the point (x,y) lying on pixel px, py which
    // corresponds to coordinate of the provided pixel corner
    bool PointAtPixel( unsigned int px, unsigned int py,
                       double &x, double &y ) const noexcept;

    bool PixelAtPoint( double x, double y,
                       unsigned int &px, unsigned int &py ) const noexcept;

    bool IsInitialized() const noexcept
    {
        return !_pixels.empty();
    }

    void BlendPixel(unsigned int px, unsigned int py, const nhColor &c) noexcept;

protected: 

    // Color channel
    std::vector<uint8_t>    _pixels;

    // Geometry corresponding to image
    double                  _xMin,
                            _xMax,
                            _yMin,
                            _yMax;

    // Image resolution
    unsigned int            _resX,
                            _resY;

};

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhImage::PointAtPixel( unsigned int px, unsigned int py,
                            double &x, double &y ) const noexcept
{
    double pixelSpanX = ( _xMax - _xMin ) / _resX;
    x = _xMin + px * pixelSpanX;

    double pixelSpanY = ( _yMax - _yMin ) / _resY;
    y = _yMin + py * pixelSpanY;

    // position (x, y) in center of the pixel
    x += 0.5 * pixelSpanX;
    y += 0.5 * pixelSpanY;

    return (x < _xMax && y < _yMax);
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
bool nhImage::PixelAtPoint( double x, double y,
                            unsigned int &px, unsigned int &py ) const noexcept
{
    if (x < _xMin || x > _xMax || y < _yMin || y > _yMax)
    {
        return false;
    }

    double pixelSpanInv = _resX / (_xMax - _xMin);
    px = static_cast<unsigned int>((x - _xMin) * pixelSpanInv);

    pixelSpanInv = _resY / (_yMax - _yMin);
    py = static_cast<unsigned int>(( y - _yMin ) * pixelSpanInv);

    return true; 
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhImage::RenderImage( const char* fileName, unsigned int numThreads )
{
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhImage::InitColors( void )
{
    if ( !IsInitialized() )
    {
        _pixels.resize(4 * _resX * _resY, 0);
    }
}

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void nhImage::BlendPixel( unsigned int px, unsigned int py,
                          const nhColor &color ) noexcept
{
    // simple averaging
    uint8_t *pixel = &_pixels[4 * (py*_resX + px)];
    pixel[0] = (pixel[0] >> 1) + (color._red   >> 1);
    pixel[1] = (pixel[1] >> 1) + (color._green >> 1);
    pixel[2] = (pixel[2] >> 1) + (color._blue  >> 1);
    pixel[3] = (pixel[3] >> 1) + (color._alpha >> 1);
}

#endif // _NHIMAGE_H
