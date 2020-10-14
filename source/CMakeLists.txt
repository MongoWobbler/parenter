# Make sure to download FindMaya.cmake from:
# https://github.com/chadmv/cgcmake
# and place it in CMake's modules: CMAKE_INSTALL_PATH\share\cmake-VERSION\Modules
# Example: C:\Program Files\CMake\share\cmake-3.18\Modules\FindMaya.cmake
# 
# To build:
# 1. Make sure all the source files are defined in the variable "SOURCE_FILES" below!
#	 Name the .mll below. In this case "Parenter"
#
# 2. Create a folder called "build" above the source files path.
# 		Example: C:/Parenter/build
# 
# 3. Run the Command Shell inside the build folder.
#
# 4. Run the CMake generate command with the appropriate visual studio version.
#		This will generate the Visual Studio Solution ready to be built
# 		Use the command "cmake -h" to see all the visual studio versions available.
# 		Note at the end of the is where the source files are at. (../ means go up a folder, and then into the /source folder)
#		Maya 2018: 
#			cmake -G "Visual Studio 14 2015 Win64" -DMAYA_VERSION=2018 ../source
#		Maya 2019: 
#			cmake -G "Visual Studio 14 2015 Win64" -DMAYA_VERSION=2019 ../source
#		Maya 2020: 
#			cmake -G "Visual Studio 15 2017 Win64" -DMAYA_VERSION=2020 ../source
# 
# 5. Run the build command from CMake and the Visual Studio Solution from the last command is made.
#		cmake --build . --config Release

cmake_minimum_required(VERSION 3.1)
project(Parenter)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

set(SOURCE_FILES
	"pluginMain.cpp"
	"parenter.cpp"
	"parenter.h"
	"util.h"
	)

find_package(Maya REQUIRED)

add_library(Parenter SHARED ${SOURCE_FILES})
target_link_libraries(Parenter PRIVATE Maya::Maya)
target_include_directories(Parenter PRIVATE Maya::Maya)

MAYA_PLUGIN(Parenter)