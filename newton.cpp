#include "nhNewton.h"
#include <memory>

using namespace std;

int main(int argc, char *argv[])
{
    auto image = std::make_unique<nhNewton>(-2.0f, 1.0f, -1.0f, 1.0f, 4800, 3200, 100);
    image->InitColors();
    image->RenderImage("newton.png");
    return 0;
}
