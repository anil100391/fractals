#include "nhMandelbrotOrbit.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    const double xmin = -2.0f;
    const double xmax = 1.0f;
    const double ymin = -1.0f;
    const double ymax = 1.0f;

    int resx = 1040;
    int resy = 720;
    int min_iter = 5000;
    int max_iter = 6000;

    nhMandelbrotOrbit image{ xmin, xmax, ymin, ymax, resx, resy, max_iter, min_iter };

    const char *fileName = "orbit.png";
    image.InitializeFromFile( fileName );

    nhColor color;
    for ( int ii = 0; ii < 10; ++ii )
    {
        std::cout << "Rendering orbit: " << ii << std::endl;
        color.Randomize();
        image.DrawNewOrbit( color );
    }

    image.Render( fileName );
    return 0;
}
