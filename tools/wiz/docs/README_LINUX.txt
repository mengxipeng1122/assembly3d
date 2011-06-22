Instructions for building the Assembly3D tools on Linux
---------------------------------------

System Path
-----------
You need the following available on your system path:
- CMake 2.8+
- wget


Environment
-----------


Building the tools
----------------

Open a Terminal window and navigate to the
Assembly3DTools/external_libs directory.  Then run
./install_dependencies.sh . Navigate to the Assembly3DTools/builds
directory and run following commands:

cmake ..
make
sudo make install
