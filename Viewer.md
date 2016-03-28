**Quick Links**

<br />

# Description #

Viewer is a tool for displaying Assembly3D _mesh_ files.


<br />
# Command line tool #

## Usage ##

```
working-dir$ ./A3DViewer -o=<FileName.mesh.xml> [OPTIONS]
```


## Options ##
```
   ./A3DViewer  [-s=<path/to/scene>] [-d=<path/to/meshes>]
                [-m=<path/to/mesh>] [-b=<path/to/binary>] [--scale=<scale>]
                [--scene-scale=<scale>] [--] [--version] [-h]


Where: 

   -s=<path/to/scene>,  --scene-file=<path/to/scene>
     Scene file.

   -d=<path/to/meshes>,  --mesh-dir=<path/to/meshes>
     Mesh directory.

   -m=<path/to/mesh>,  --mesh-file=<path/to/mesh>
     Mesh file.

   -b=<path/to/binary>,  --binary-file=<path/to/binary>
     Binary file.

   --scale=<scale>
     Scale.

   --scene-scale=<scale>
     Scene scale.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```