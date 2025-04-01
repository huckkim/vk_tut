#include "HelloTriangleApplication.h"
#include <spdlog/spdlog.h>

#include <iostream>

int main()
{
    HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		spdlog::error(e.what());
		return EXIT_FAILURE;
	}
    return 0;
}