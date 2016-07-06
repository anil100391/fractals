#include "nhMandelbrotOrbit.h"

int main(int argc, char *argv[])
{
    nhMandelbrotOrbit* image = new nhMandelbrotOrbit(-2.0f, 1.0f, -1.0f, 1.0f, 1200,800);
    if ( !image->InitializeFromFile("temp.png") )
    {
        return 0;
    }

    image->SetOrbitLimit(100000);
    nhColor color;
    for ( int ii = 0; ii < 10000; ++ii )
    {
        color.Randomize();
        image->DrawNewOrbit(color);
    }

    int numThreads = -1;
    if ( argc > 1 )
    {
        numThreads = atoi(argv[1]);
    }

    if ( numThreads > 0 )
    {
        std::cout << "Requested " << numThreads << " threads for rendering\n";
    }

    image->RenderImage("new.png", numThreads);
    delete image;
    return 0;
}
