#include "nhMandelbrot.h"
#include <memory>

using namespace std;

int main(int argc, char *argv[])
{
    auto image = std::unique_ptr<nhMandelbrot>(new nhMandelbrot(-2.0f, 1.0f, -1.0f, 1.0f, 4800, 3200, 1000));
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

    return 0;
}
