#pragma once

#include <vulkan/vulkan.hpp>

struct GLFWwindow;

static const std::vector<const char*> theValidationLayers
{
	"VK_LAYER_KHRONOS_validation"
};

#ifdef DEBUG
static const bool theEnableValidationLayer = true;
#else
static const bool theEnableValidationLayer = false;
#endif


class HelloTriangleApplication
{
public:
	void run();

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanUp();

	// vulkan functions
	void createInstance();
	void setupDebugMessenger();

	// vulkan debug functions
	bool checkValidationLayerSupport();
	std::vector<const char*> getValidationLayers();

	bool checkExtensionSupport();
	std::vector<const char*> getExtensions();

private:
	GLFWwindow* myWindow;

	// vulkan objects
	vk::Instance myInstance;
	vk::DebugUtilsMessengerEXT myDebugMessenger;
};
