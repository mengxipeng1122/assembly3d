**Quick Links**

<br />

# Description #

MeshWiz is a tool for modifying Assembly3D _mesh_ files. It is similar to and actually was inspired by Ogre3D's [MeshMagick](http://www.ogre3d.org/tikiwiki/MeshMagick).

<br />
# Command line tool #

## Usage ##

```
working-dir$ ./MeshWiz [OPTIONS] <path/to/mesh.xml>
```


## Options ##
```
   ./MeshWiz  [-t=<x/y/z>] [-r=<angle/x/y/z>] [-s=<x/y/z|s>]
              [--axes=<x/y/z>] [--resize=<x/y/z|axis/r>] [--center=<1/1/1>]
              [--center-all] [--convert-index-type-to=<int|short|byte>]
              [--stitch] [--stitch-eps=<attribute/epsilon>]
              [--make-normals-consistent] [--flip-front-face]
              [--flip-winding] [--texture-transform] [-o=<output-folder>]
              [-b=<binary-file>] [-q] [--dump-txt] [-i] [--] [--version]
              [-h] <source-file>


Where: 

   -t=<x/y/z>,  --translate=<x/y/z>
     Translates the mesh by this vector.

   -r=<angle/x/y/z>,  --rotate=<angle/x/y/z>
     Rotates the mesh <angle> degrees on the axis x/y/z.

   -s=<x/y/z|s>,  --scale=<x/y/z|s>
     Scales the mesh by this scale vector or scales by one scale factor s.

   --axes=<x/y/z>
     Remaps main axes.

   --resize=<x/y/z|axis/r>
     Scales the mesh so, that its size is x/y/z or give axis and new value
     r.

   --center=<1/1/1>
     Centers mesh to given axis (i.e. 1/1/1 to put center to 0/0/0).

   --center-all
     Centers mesh to all axes.

   --convert-index-type-to=<int|short|byte>
     Converts index type between UNSIGNED_INT , UNSIGNED_SHORT and
     UNSIGNED_BYTE.

   --stitch
     Removes duplicate vertices.

   --stitch-eps=<attribute/epsilon>
     Removes duplicate vertices. Compares all attributes but one given
     attribute with a possible deviation epsilon.

   --make-normals-consistent
     Makes normals consistent if neccesary, by comparing file and
     generating normals: angle < PI/2 means facing outwards otherwise
     inwards.

   --flip-front-face
     Flips front-faces and normals.

   --flip-winding
     Flips winding from cw to ccw or vice versa.

   --texture-transform
     Applies transformation only to texture coordinates.

   -o=<output-folder>,  --output-folder=<output-folder>
     Output folder.

   -b=<binary-file>,  --binary-file=<binary-file>
     Name of binary file.

   -q,  --quite
     No verbose output.

   --dump-txt
     Dumps the mesh to a text file.

   -i,  --info
     Prints the mesh info.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <source-file>
     (required)  File to manipulate.

```