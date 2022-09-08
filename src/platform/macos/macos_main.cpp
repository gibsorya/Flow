//
// Created by Ryan Gibson on 3/23/22.
//

#include <engine/app.hpp>

int run(){
    int result = 0;

    Error err = flow::setup();

    if(err != SUCCESS){
        return 255;
    }

    flow::mainLoop();
    flow::cleanup();

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