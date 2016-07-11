#include "nhMandelbrotOrbit.h"

int main(int argc, char *argv[])
{
    int numThreads = -1;
    if ( argc > 1 )
    {
        numThreads = atoi(argv[1]);
    }

    if ( numThreads > 0 )
    {
        omp_set_num_threads(numThreads);
        std::cout << "Requested " << numThreads << " threads for rendering\n";
    }

#pragma omp parallel
{
    nhMandelbrotOrbit* image = new nhMandelbrotOrbit(-2.0f, 1.0f, -1.0f, 1.0f, 10400, 7200);
    if ( !image->InitColors() )
    {
        exit(0);
    }

    image->SetOrbitLimit(100000);
    nhColor color;
    for ( int ii = 0; ii < 10000; ++ii )
    {
        color.Randomize();
        image->DrawNewOrbit(color);
    }

    char fileName[256];
    sprintf(fileName, "new%d.png", omp_get_thread_num());
    image->RenderImage(fileName);
    delete image;
}
    return 0;
}
