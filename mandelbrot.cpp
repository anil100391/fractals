#include "nhMandelbrot.h"
#include <memory>

using namespace std;

int main(int argc, char *argv[])
{
    auto image = std::make_unique<nhMandelbrot>(-2.0f, 1.0f, -1.0f, 1.0f, 4800, 3200, 1000);
    image->InitColors();
    image->RenderImage("mandelbrot.png");
    return 0;
}
