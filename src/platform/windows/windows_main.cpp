#include "os_windows.hpp"

int run(){
    int result = 0;

    Error err = app::setup();

    if(err != SUCCESS){
        return 255;
    }

    return result;
}

int main() {
    try {
        return run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}