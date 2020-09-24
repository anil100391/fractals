#include "nhMandelbrot.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    const double xmin = -2.0f;
    const double xmax = 1.0f;
    const double ymin = -1.0f;
    const double ymax = 1.0f;

    int resx = 1200;
    int resy = 800;
    int iterations = 100;
    nhMandelbrot image( xmin, xmax, ymin, ymax, resx, resy, iterations );
    image.Render( "mandelbrot.png" );
    return 0;
}
