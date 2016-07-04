#include "nhMandelbrot.h"
using namespace std;

int main()
{
    //nhMandelbrot* image = new nhMandelbrot(-2.0f, 1.0f, -1.0f, 1.0f, 2400, 1600, 1000);
    nhMandelbrot* image = new nhMandelbrot(-2.0f, 1.0f, -1.0f, 1.0f, 4800, 3200, 1000);
    image->InitColors();
    image->RenderImage("mandelbrot.png");
    delete image;

    return 0;
}
