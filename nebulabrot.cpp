#include "nhNebulabrot.h"
#include <memory>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    auto image = std::make_unique<nhNebulabrot>(-2.0f, 1.0f, -1.0f, 1.0f, 1200, 800, 1000, 100);
    image->InitColors();
    image->RenderImage("nebulabrot.png");
    return 0;
}
