#include <main.hpp>

int main(int argc, char *argv[])
{
    #ifdef _WIN32
    windows::run();
    #elif defined(__APPLE__)
    macos::run();
    #endif

    return 0;
}
