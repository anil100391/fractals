#include "nhMandelbrotOrbit.h"

int main()
{
    nhMandelbrotOrbit* image = new nhMandelbrotOrbit(-2.0f, 1.0f, -1.0f, 1.0f, 1200,800);
    if ( !image->InitializeFromFile("temp.png") )
    {
        return 0;
    }

    image->SetOrbitLimit(100000);
    nhColor color;
    for ( int ii = 0; ii < 10000; ++ii )
    {
        color.Randomize();
        image->DrawNewOrbit(color);
    }
    image->RenderImage("new.png");
    delete image;
    return 0;
}
