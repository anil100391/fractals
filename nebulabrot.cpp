#include "nhNebulabrot.h"

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
    int min_iter = 100;
    int max_iter = 1000;

    nhNebulabrot image{ xmin, xmax, ymin, ymax, resx, resy, max_iter, min_iter };
    image.Render( "nebulabrot.png" );
    return 0;
}
