#include "nhTricorn.h"
#include <memory>
using namespace std;

int main(int argc, char *argv[])
{
    auto image = std::make_unique<nhTricorn>(-2.0f, 2.0f, -2.0f, 2.0f, 4800, 4800, 1000);
    image->InitColors();
    image->RenderImage("tricorn.png");
    return 0;
}
