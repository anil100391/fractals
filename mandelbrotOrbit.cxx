#include "nhMandelbrotOrbit.h"
#include <memory>

int main(int argc, char *argv[])
{
    auto image = std::make_unique<nhMandelbrotOrbit>(-2.0f, 1.0f, -1.0f, 1.0f, 1040, 720, 5000, 6000);
    if ( !image->InitColors() )
    {
        exit(0);
    }

    const char* fileName = "orbit.png";
    image->InitializeFromFile(fileName);

    nhColor color;
    for ( int ii = 0; ii < 10; ++ii )
    {
        std::cout << "Rendering orbit: " << ii << std::endl;
        color.Randomize();
        image->DrawNewOrbit(color);
    }

    image->RenderImage(fileName);
    return 0;
}
