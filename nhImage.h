#ifndef _NHIMAGE_H_
#define _NHIMAGE_H_

#include <vector>
#include <cstdint>
#include <iostream>
#include <cairo/cairo.h>

// -----------------------------------------------------------------------------
//  Class to map a portion of Cartesian Plane to an Image of given resolution
// -----------------------------------------------------------------------------
class nhImage
{
public:

    nhImage() = delete;

    nhImage( double xmin, double xmax,
             double ymin, double ymax,
             int resX, int resY )
        : p_xMin(xmin)
        , p_xMax(xmax)
        , p_yMin(ymin)
        , p_yMax(ymax)
        , p_resX(resX)
        , p_resY(resY)
    {}

    virtual ~nhImage() = default;

    enum PIXEL_CORNER
    {
        CENTER,
        LOWER_LEFT,
        LOWER_RIGHT,
        UPPER_RIGHT,
        UPPER_LEFT,
    };

    // initializes pixels of the image
    virtual bool Paint( void );

    // Renders this image to a png file
    void Render( const char* fileName );

    // Gets the point (x,y) lying on pixel px, py which
    // corresponds to coordinate of the provided pixel corner
    bool PointAtPixel( const int px, const int py,
                       double &x, double &y,
                       const PIXEL_CORNER loc = CENTER ) const;

    // Gets the pixel lying on point
    bool PixelAtPoint( const double x, const double y,
                       int &px, int &py ) const;

protected:

    // pixel color info
    std::vector<uint8_t> p_colorData;

    // Geometry corresponding to image
    double  p_xMin;
    double  p_xMax;
    double  p_yMin;
    double  p_yMax;

    // Image resolution
    int     p_resX;
    int     p_resY;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool nhImage::PointAtPixel( const int px, const int py,
                            double &x, double &y,
                            const nhImage::PIXEL_CORNER loc ) const
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

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool nhImage::PixelAtPoint( const double x, const double y, int &px, int &py )
                                                                           const
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

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void nhImage::Render( const char* fileName )
{
    if ( p_colorData.empty() )
        // not yet initialied
        return;

    std::cout << "Rendering "<< p_resX << "x" << p_resY << " image\n";

    cairo_format_t imgFormat = CAIRO_FORMAT_ARGB32;
    int rowStride = cairo_format_stride_for_width ( imgFormat, p_resX );

    cairo_surface_t *surface = cairo_image_surface_create_for_data( p_colorData.data(),
                                                                    CAIRO_FORMAT_ARGB32,
                                                                    p_resX,
                                                                    p_resY,
                                                                    rowStride );
    cairo_surface_write_to_png(surface, fileName);
    cairo_surface_destroy(surface);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool nhImage::Paint( void )
{
    if ( p_colorData.empty() )
    {
        p_colorData.resize(4 * p_resX * p_resY, 0);
        return true;
    }

    return false;
}

#endif // _NHIMAGE_H_
