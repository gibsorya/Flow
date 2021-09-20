#include "instance.hpp"
#include "../root.hpp"
namespace flow::vulkan {
	void startFlow()
	{
		root->flowInstance->instance = FlowInstance::createInstance();
		initComponents();
	}

	void initComponents()
	{
	}

}