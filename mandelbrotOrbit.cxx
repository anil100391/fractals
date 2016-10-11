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
    nhMandelbrotOrbit* image = new nhMandelbrotOrbit(-2.0f, 1.0f, -1.0f, 1.0f, 10400, 7200, 50000, 100000);
    if ( !image->InitColors() )
    {
        exit(0);
    }

    char fileName[256];
    sprintf(fileName, "new%d.png0", omp_get_thread_num());
    image->InitializeFromFile(fileName);

    nhColor color;
    for ( int ii = 0; ii < 500; ++ii )
    {
        std::cout << "Rendering orbit: " << ii << std::endl;
        color.Randomize();
        image->DrawNewOrbit(color);
    }

    image->RenderImage(fileName);
    delete image;
}
    return 0;
}
