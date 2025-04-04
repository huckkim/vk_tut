# vk_tut

C++ Code for [Vulkan Tutorial](https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/02_Validation_layers.html)
using [Vulkan Headers](https://github.com/KhronosGroup/Vulkan-Hpp)

## Build Instructions
### Windows Visual Studio
- Only tested for Windows, Visual Studio 2022

1. Setup vcpkg
```
$ git clone https://github.com/microsoft/vcpkg.git
$ cd vcpkg; .\bootstrap-vcpkg.bat
```  
2. Setup env variables for vcpkg
```
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
```

3. Clone this repo
```
$ git clone git@github.com:huckkim/vk_tut.git
$ cd vk_tut
```
4. Create a CMakeUserPresets.json file `VCPKG_ROOT` set to proper location
```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "default",
      "inherits": "vcpkg",
      "environment": {
        "VCPKG_ROOT": "<path to vcpkg>"
      }
    }
  ]
}
```

5. Visual Studio should properly configure CMake Project.

### Linux
- Untested, but should be similar except no Visual Studio and explicitly using `cmake --preset=default`

### MacOS
`>:(` I hate xcodebuild, would be supported once they add MoltenVk to vcpkg ports but alas.

