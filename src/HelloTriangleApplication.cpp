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
	using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
	switch (messageSeverity)
	{
	case eVerbose:
		break;
	case eInfo:
		spdlog::info("Validation Layer: {}, {}",
			pCallbackData->pMessageIdName,
			pCallbackData->pMessage);
		break;
	case eWarning:
		spdlog::warn("Validation Layer: {}, {}",
			pCallbackData->pMessageIdName,
			pCallbackData->pMessage);
		break;
	case eError:
		spdlog::error("Validation Layer: {}, {}",
			pCallbackData->pMessageIdName,
			pCallbackData->pMessage);
		break;
	}
	return VK_FALSE;
}

VkResult
CreateDebugUtilsMessengerEXT(
	vk::Instance instance,
	const VkDebugUtilsMessengerCreateInfoEXT& createInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, &createInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(vk::Instance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
		func(instance, messenger, pAllocator);
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
	setupDebugMessenger();
}

void
HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(myWindow))
	{
		glfwPollEvents();
	}
}

bool
HelloTriangleApplication::checkValidationLayerSupport()
{
	auto availableLayers = vk::enumerateInstanceLayerProperties();
	auto layers = getValidationLayers();

	std::vector<const char*> l1;

	std::for_each(availableLayers.begin(), availableLayers.end(),
		[&l1](const vk::LayerProperties& prop)
		{
			l1.push_back(prop.layerName.data());
		});

	for (auto layer : layers)
	{
		auto it = std::find_if(l1.begin(), l1.end(),
			[&layer](const char* e)
			{
				return strcmp(layer, e) == 0;
			});
		if (it == l1.end())
		{
			spdlog::error("Failed to find {} extension support!", layer);
			return false;
		}
	}
	return true;
}

bool
HelloTriangleApplication::checkExtensionSupport()
{
	auto availableExtensions = vk::enumerateInstanceExtensionProperties();
	auto extensions = getExtensions();

	std::vector<const char*> l1;

	std::for_each(availableExtensions.begin(), availableExtensions.end(),
		[&l1](const vk::ExtensionProperties& prop)
		{
			l1.push_back(prop.extensionName.data());
		});

	for (auto extension : extensions)
	{
		auto it = std::find_if(l1.begin(), l1.end(),
			[&extension](const char* e)
			{
				return strcmp(extension, e) == 0;
			});
		if (it == l1.end())
		{
			spdlog::error("Failed to find {} extension support!", extension);
			return false;
		}
	}
	return true;
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

	return res;
}

std::vector<const char*>
HelloTriangleApplication::getExtensions()
{
	std::vector<const char*> res;

	// glfw3 required extensions
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	res.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// debug messenger callback extension
	res.push_back(vk::EXTDebugUtilsExtensionName);

	return res;
}

vk::DebugUtilsMessengerCreateInfoEXT
HelloTriangleApplication::createDebugMessengerCreateInfo()
{
	using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
	using enum vk::DebugUtilsMessageTypeFlagBitsEXT;
	vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.messageSeverity = eVerbose | eWarning | eError;
	createInfo.messageType = eGeneral | eValidation | ePerformance;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;
	return createInfo;
}

void
HelloTriangleApplication::setupDebugMessenger()
{
	if (!theEnableValidationLayer)
		return;

	vk::DebugUtilsMessengerCreateInfoEXT createInfo = createDebugMessengerCreateInfo();

	if (CreateDebugUtilsMessengerEXT(myInstance, createInfo, nullptr, &myDebugMessenger) != VK_SUCCESS)
		throw std::runtime_error("Failed to setup debug messenger!");
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

	if (!checkExtensionSupport())
		throw std::runtime_error("Failed to support all required extensions!");
	if (!checkValidationLayerSupport())
		throw std::runtime_error("Failed to support all required validation layers!");

	vk::InstanceCreateInfo createInfo{};
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.size() ? extensions.data() : nullptr;

	auto validationLayers = getValidationLayers();
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.size() ? validationLayers.data() : nullptr;

	vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (theEnableValidationLayer)
	{
		debugCreateInfo = createDebugMessengerCreateInfo();
		createInfo.pNext = &debugCreateInfo;
	}

	myInstance = vk::createInstance(createInfo, nullptr);
}

void
HelloTriangleApplication::cleanUp()
{
	if (theEnableValidationLayer)
		DestroyDebugUtilsMessengerEXT(myInstance, myDebugMessenger, nullptr);
	myInstance.destroy();

	glfwDestroyWindow(myWindow);
	glfwTerminate();
}
