**Quick Links**

<br />

# Motivation and Design #

There various 3D formats available for different purposes like, _Digital Content Creation (DDC)_,_Digital Asset Exchange_ and _Real-time Applications_. The Assembly3D formats are clearly designed for the last category upon the following principles.

## Simplicity ##
Parsing and processing the data should be as easy as possible. In general the indirect consequences are less code and higher performance.

## Modularity ##
Rather than having a single über format, there are dedicated formats for semantic units. Currently, the following sub-formats exist or are in development:

  * **[Mesh](Mesh.md)**: Triangles Meshes with custom vertex attributes and named groups (e.g. for materials).
<a href='Hidden comment: 
* *Modifier*: Data for manipulating mesh vertices. Two common types for character animation are _morph-targets_ and _vertex-skinning_.

* *Material*: Defines uniform variables and references textures used for shading.

* *Light*: Defines light type and parameters. Potentially referring to texture masks and meshes as emitting geometry.
'></a>

  * **[Scene](Scene.md)**: Object positions and locations.

  * **[Animation](Anim.md)**: Key-frame animation grouped in channels with custom attributes such as key-times, translations and rotations.


## Separation of Layout & Data ##
Most data intensive formats are bipartite:

  * **Layout**: The human readable part which describes the accompanying binary data. Currently, there are schema definitions for XML and transformations to JSON.

  * **Data**: Flat, array like structures with the purpose of fast loading into the target memory (CPU or GPU RAM). In addition, compression schemes are planned for low bandwidth scenarios.


## Hardware-friendly ##
The formats, especially the binary parts, are designed for efficient use with technologies such as OpenGL and OpenCL.