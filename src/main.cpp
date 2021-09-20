#include "engine/engine.cpp"

#include <iostream>

void run() {
    flow::initVulkan();
    flow::mainLoop();
    flow::cleanup();
}

int main() {
    try {
        run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}