#include <memory>
#include <iostream>
#include "nhOrbit.h"

bool mandelbrotIterator(double &x, double &y, double cx, double cy)
{
    // z = z * z + c
    double lx = x * x - y * y + cx;
    double ly = 2 * x * y + cy;
    x = lx;
    y = ly;
    return true;
}

int main(int argc, char *argv[])
{
    auto image = std::make_unique<nhOrbit>(mandelbrotIterator, -2.0f, 1.0f, -1.0f, 1.0f, 256*3, 256*2);
    image->SetIterations(1000);

    nhColor color;
    for ( int ii = 0; ii < 500; ++ii )
    {
        std::cout << "Rendering orbit: " << ii << std::endl;
        color.Randomize();
        image->DrawNewOrbit(color);
    }

    return 0;
}
