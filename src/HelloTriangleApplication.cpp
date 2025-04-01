#include "HelloTriangleApplication.h"

#define GLFW_INCLUDE
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

const uint32_t HEIGHT = 600;
const uint32_t WIDTH = 600;

static VKAPI_ATTR vk::Bool32 VKAPI_CALL
debugCallback(
	vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	vk::DebugUtilsMessageTypeFlagsEXT messageType,
	const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
)
{
	spdlog::error("Validation Layer: {}, {}",
		pCallbackData->pMessageIdName,
		pCallbackData->pMessage);
	return VK_FALSE;
}

VkResult
CreateDebugUtilsMessengerEXT(
	vk::Instance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void
HelloTriangleApplication::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanUp();
}

void
HelloTriangleApplication::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	myWindow = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
}

void
HelloTriangleApplication::initVulkan()
{
	createInstance();
}

void
HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(myWindow))
	{
		glfwPollEvents();
	}
}

void
HelloTriangleApplication::cleanUp()
{
	myInstance.destroy();

	glfwDestroyWindow(myWindow);
	glfwTerminate();
}

bool
HelloTriangleApplication::checkValidationLayerSupport()
{
	auto availableLayers = vk::enumerateInstanceLayerProperties();
	auto layers = getValidationLayers();
	for (const auto v : layers)
	{
	}
}

bool
HelloTriangleApplication::checkExtensionSupport()
{
	auto availableExtensions = vk::enumerateInstanceExtensionProperties();
	auto extensions = getExtensions();
	for (const auto& extension : extensions)
	{
	}
}

std::vector<const char*>
HelloTriangleApplication::getValidationLayers()
{
	std::vector<const char*> res;

	// if not enabled return empty layers;
	if (!theEnableValidationLayer)
		return res;
		
	for (auto l : theValidationLayers)
		res.push_back(l);

	res.push_back(vk::EXTDebugUtilsExtensionName);

	return res;
}

std::vector<const char*>
HelloTriangleApplication::getExtensions()
{
	std::vector<const char*> res;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	res.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return res;
}

void
HelloTriangleApplication::setupDebugMessenger()
{
	if (!theEnableValidationLayer)
		return;

	using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
	using enum vk::DebugUtilsMessageTypeFlagBitsEXT;

	vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.messageSeverity = eVerbose | eWarning | eError;
	createInfo.messageType = eGeneral | eValidation | ePerformance;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;
}

void
HelloTriangleApplication::createInstance()
{
	vk::ApplicationInfo appInfo{};
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	auto extensions = getExtensions();
	auto validationLayers = getValidationLayers();
	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.size() ? extensions.data() : nullptr;
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.size() ? validationLayers.data() : nullptr;

	myInstance = vk::createInstance(createInfo, nullptr);
}
