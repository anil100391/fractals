#include "nhTricorn.h"
#include <memory>
using namespace std;

int main(int argc, char *argv[])
{
    auto image = std::unique_ptr<nhTricorn>(new nhTricorn(-2.0f, 2.0f, -2.0f, 2.0f, 4800, 4800, 1000));
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


    image->RenderImage("tricorn.png", numThreads);

    return 0;
}
