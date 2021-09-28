// #include "../engine.hpp"
#include <engine/engine.hpp>
// #include "test.hpp"

#include <iostream>

void run() {
    flow::test();
    flow::initVulkan();
    flow::mainLoop();
    flow::cleanup();
    std::cout << "Vulkan finished!" << std::endl;
    system("pause");
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