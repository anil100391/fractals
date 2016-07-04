#include "nhTricorn.h"
using namespace std;

int main()
{
    nhTricorn* image = new nhTricorn(-2.0f, 2.0f, -2.0f, 2.0f, 4800, 4800, 1000);
    image->InitColors();
    image->RenderImage("tricorn.png");
    delete image;

    return 0;
}
