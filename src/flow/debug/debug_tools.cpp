#include <debug_tools.hpp>

namespace flow::debug
{
    bool checkValidationLayerSupport()
    {
        u32 layerCount;
        vk::Result result;
        result = vk::enumerateInstanceLayerProperties(&layerCount, nullptr);

        std::cout << "LAYER COUNT: " << layerCount << std::endl;

        std::vector<vk::LayerProperties> availableLayers(layerCount);

        std::cout << availableLayers.data() << std::endl;

        result = vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        std::cout << "LAYERS: " << layers.at(0) << std::endl;

        for (const char *layerName : layers)
        {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                std::cout << "LAYER FOUND: " << layerFound << std::endl;
                return false;
            }
        }

        return true;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;

	}
}