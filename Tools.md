**Quick Links**

<br />

# Tools Overview #

A good tool chain is of high importance for formats targeted at real-time applications. That's why the Assembly3D code project aims providing a complete pipeline for transferring assets from 3D modeling tools into a 'compiled' form for real-time use. The main categories are:

## Shift ##
> The first step is to convert or transform other 3D file-formats into Assembly3D formats. The idea is mainly convert the structure but leave that data itself unprocessed. Initially, the focus lies on a few common digital exchange formats.

  * **[ObjShift](ObjShift.md)**: Converts Autodesk's (former Alias|Wavefront) _OBJ_ format in to [meshes](Mesh.md).

  * **MtlShift**: Converts Autodesk's (former Alias|Wavefront) _MTL_ format in to materials.

  * **FBX**: Converts Autodesk's FBX files to various Assembly3D formats.

  * **DaeShift**: Converts COLLADA files to various Assembly3D formats.


## Wiz(ard) ##

After converting files to the various Assembly3D formats it might me required to manipulate the data. Ideally the tools in this category result a final 'compiled' version which need no further processing in the targeted real-time application..

  * **[MeshWiz](MeshWiz.md)**: Manipulates Mesh data. For example applies transformation to geometry, stitches texture coordinates or converts data type of indices.

## Prim(imitves) ##

In addition to converting other formats to Assembly3D files, it is possible to generate this from parametrized primitives.

  * **[MeshPrim](MeshPrim.md)**: Generate Mesh files from parametric equations like Quadrics, Superquadrics and other primitive shapes.


## Test ##

Utilities for automated unit tests. These help ensure quality and compatibility of the other tools.

  * **[MeshTest](MeshTest.md)**: Compare Mesh files for equality. Options include strict comparison or the the use of deltas and ignoring the sorting of vertex  attributes.

## Viewer ##

  * **[Viewer](Viewer.md)**: Simple commandline application for displaying mesh files