Instructions for building the Assembly3D tools on Linux
---------------------------------------

System Path
-----------
You need the following available on your system path:
- CMake 2.8+
- Xcode
- curl


Environment
-----------


Building the tools
----------------

Open a Terminal window and navigate to the
Assembly3DTools/external_libs directory.  Then run
./install_dependencies.sh . Navigate to the Assembly3DTools/builds
directory and run following commands:

If you have cmake in your PATH then:
cmake ..

Else, open the cmake gui from /Applications directory and enter
Assembly3D directory as source path and Assembly/builds as build
directory, hit the configure button and the generate. You can now open
the Xcode project file and build
