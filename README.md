# parenter

Maya C++ plug-in. 
Quick attempt at space matching using matrices. Ended up with a play count transform offset node. 

Use:
Once plug-in is loaded, create a "parenter" node, and connect transforms World Matrices into the "parenter"'s Parent Matrices attributes. Pick a transform to be driven by the other transform, and plug in it's World Matrix attribute into the "parenter"'s Child Matrix attribute. Finally, plug in the output translate and rotate from the parent node into the child transform.
Now notice the transform that is plugged into the Child Matrix attribute is driven by one of the parent transforms. Switch the driver by changing the Chosen Parent attribute. Switching the driver updates the transform on the child so that it stays in the same spot at the time it is switched.

Known bug:
Setting a key and switching the chosen parent attribute will make the child transform offset transformations everytime the timeline loops.

CMAKE Instructions:
Make sure to download FindMaya.cmake from:
https://github.com/chadmv/cgcmake
and place it in CMake's modules: CMAKE_INSTALL_PATH\share\cmake-VERSION\Modules
Example: C:\Program Files\CMake\share\cmake-3.18\Modules\FindMaya.cmake
 
To build:
 1. Make sure all the source files are defined in the variable "SOURCE_FILES" in CMakeLists.txt!
	 Name the .mll in CMakeLists.txt. In this case "Parenter"

 2. Create a folder called "build" above the source files path.
 		Example: C:/Parenter/build
 
 3. Run the Command Shell inside the build folder.

 4. Run the CMake generate command with the appropriate visual studio version.
		This will generate the Visual Studio Solution ready to be built
 		Use the command "cmake -h" to see all the visual studio versions available.
 		Note at the end of the is where the source files are at. (../ means go up a folder, and then into the /source folder)
		Maya 2018: 
			cmake -G "Visual Studio 14 2015 Win64" -DMAYA_VERSION=2018 ../source
		Maya 2019: 
			cmake -G "Visual Studio 14 2015 Win64" -DMAYA_VERSION=2019 ../source
		Maya 2020: 
			cmake -G "Visual Studio 15 2017 Win64" -DMAYA_VERSION=2020 ../source
 
 5. Run the build command from CMake and the Visual Studio Solution from the last command is made.
		cmake --build . --config Release
