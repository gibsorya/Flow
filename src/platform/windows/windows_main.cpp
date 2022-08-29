#include <app.hpp>

int main() {
    Error err = flow::setup();
    ERROR_FAIL_COND(err != SUCCESS, ERR_CANT_CREATE, "Failed to create Flow Vulkan instance");

    flow::mainLoop();
    flow::cleanup();

    return 0;
}