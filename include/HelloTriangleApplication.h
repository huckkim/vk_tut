#pragma once

#include <vulkan/vulkan.hpp>

#include <optional>

struct GLFWwindow;

static const std::vector<const char*> theValidationLayers
{
	"VK_LAYER_KHRONOS_validation"
};

#ifdef _DEBUG
static const bool theEnableValidationLayer = true;
#else
static const bool theEnableValidationLayer = false;
#endif

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

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
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSurface();

	// vulkan debug functions
	bool checkValidationLayerSupport();
	std::vector<const char*> getValidationLayers();
	vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo();

	bool checkExtensionSupport();
	std::vector<const char*> getExtensions();

private:
	GLFWwindow* myWindow;

	// vulkan objects
	vk::Instance			myInstance;
	vk::PhysicalDevice		myPhysicalDevice;
	vk::Device				myDevice;
	vk::Queue				myQueue;
	vk::SurfaceKHR			mySurface;
	vk::Queue				myPresentQueue;

	VkDebugUtilsMessengerEXT myDebugMessenger;
};
