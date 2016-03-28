# Project Ideas #

Although any kind of contribution is highly appreciated, there are some topics/areas of particular interest. For each, recommended tasks and expertise are listed below. Please let us know if you are interested or have any questions (assembly@interaction3d.org)!

<br />
## Physics ##

Currently, the Assembly3D file formats focus on (graphics) rendering and animation. Extensions for physics simulations would be a great addition.

### Tasks ###

  * Specification of a _format_ for rigid body properties such as mass, moments of inertia, friction and more.
  * Development of a _tool_ for creating bound geometries from the triangle [meshes](Mesh.md). Examples are oriented boxes, capsules, convex hulls.

  * Development of a _tool_ for creating bounding volumes of a mesh given an underlying skeleton. For example, a capsule for each 'bone' connecting two joints. Ideally, this tool will be based on the previous one.

  * Write a sample program using an open source physics engine. This should demonstrate the creation of a simulated scene from the Assembly3D files.


### Expertise ###

  * In physics simulation using an open source engine (e.g. [bulletphysics.org](http://bulletphysics.org) )

  * In C++ or Java for tool development (C++ should be cross platform)

  * In XML including [schemata](http://www.w3schools.com/schema) definitions and [transformations](http://www.w3schools.com/xsl/) to JSON


<br />
## Compression ##

Some Assembly3D formats outsource data into raw binary files for efficient loading into the target memory. This is perfect if the files are stored on a medium without space limitations and if the transfer is _not_ bandwidth limited. We expect data compression to improve loading times if the files are stored on the web or compact discs.

### Tasks ###

  * Development or utilization of existing`*` (but free) compression algorithms for floating point and integer lists as used in the Assembly3D formats (see [Mesh](Mesh.md) or [Anim](Anim.md)). A focus should be on fast decompression and a high compression rate.

  * Development of a _tool_ for measuring load times of raw and compressed data. This should be used for comparison with different media and internet connection speeds.

`*` For example [OpenCTM](http://openctm.sourceforge.net/)

### Expertise ###

  * In entropy and data compression

  * In C++ or Java for algorithm and tool development






&lt;wiki-comment&gt;



## Asset browser ##

## Sample Assets ##

## Voxel based representations ##




&lt;/wiki-comment&gt;

