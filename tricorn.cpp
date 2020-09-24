#include "nhTricorn.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    const double xmin = -2.0f;
    const double xmax = 2.0f;
    const double ymin = -2.0f;
    const double ymax = 2.0f;

    int resx = 1200;
    int resy = 1200;
    int iterations = 1000;

    nhTricorn image{ xmin, xmax, ymin, ymax, resx, resy, iterations };
    image.Render( "tricorn.png" );
    return 0;
}
