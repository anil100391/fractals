#include "nhMandelbrot.h"
using namespace std;

int main(int argc, char *argv[])
{
    //nhMandelbrot* image = new nhMandelbrot(-2.0f, 1.0f, -1.0f, 1.0f, 2400, 1600, 1000);
    nhMandelbrot* image = new nhMandelbrot(-2.0f, 1.0f, -1.0f, 1.0f, 4800, 3200, 1000);
    image->InitColors();

    int numThreads = -1;
    if ( argc > 1 )
    {
        numThreads = atoi(argv[1]);
    }

    if ( numThreads > 0 )
    {
        cout << "Requested " << numThreads << " threads for rendering\n";
    }

    image->RenderImage("mandelbrot.png", numThreads);
    delete image;

    return 0;
}
