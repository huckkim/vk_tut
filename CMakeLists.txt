cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project(VkTutorial)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

add_executable(VkTutorial 
	"src/main.cpp"
	"src/HelloTriangleApplication.cpp"
)

target_include_directories(VkTutorial PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(VkTutorial 
	Vulkan::Vulkan
	Vulkan::Headers
	glfw
	glm::glm
	spdlog::spdlog)
