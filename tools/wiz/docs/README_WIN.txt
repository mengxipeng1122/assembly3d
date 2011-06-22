Instructions for building the Assembly3D tools on Windows
---------------------------------------

System Path
-----------
You need the following available on your system path:
- CMake 2.8+
- Visual Studio
- wget
- 7zip


Environment
-----------


Building the tools
----------------

Open a Command window and navigate to the
Assembly3DTools/external_libs directory.  Then run
./install_dependencies.bat .

Open the cmake gui  and enter Assembly3D directory as source path and
Assembly/builds as build directory, hit the configure button and the
generate. You can now open the Visual Studio solution file and build.
