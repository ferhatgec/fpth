#include "include/fpaper.hpp"
#include "include/fpaper_extract.hpp"

int main(int argc, char** argv) noexcept {
    FPaper init; init.Init("test.fpaper");
    FPaper_Extract extract(init);

    extract.Extract();
}
