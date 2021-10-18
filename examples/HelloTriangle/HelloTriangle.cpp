#include <flow/foundation.hpp>
#include <engine/vk/flow_context.hpp>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>



int run()
{
    int result = 0;

    Error err = setup();

    mainLoop();

    cleanup();

    return result;
}

Error setup()
{

    return SUCCESS;
}

void mainLoop()
{

}

void cleanup()
{

}

void cleanupSwapchain()
{

}

Error recreateSwapchain()
{

}

void draw()
{

}

int main()
{
    try
    {
        return run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}