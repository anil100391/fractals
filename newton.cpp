#include "nhNewton.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    const double xmin = -2.0f;
    const double xmax = 1.0f;
    const double ymin = -1.0f;
    const double ymax = 1.0f;

    int resx = 4800;
    int resy = 3200;
    int iterations = 100;

    nhNewton image{ xmin, xmax, ymin, ymax, resx, resy, iterations };
    image.Render( "newton.png" );
    return 0;
}
