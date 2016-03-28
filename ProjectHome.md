<font color='red'>Note: This project is very much under construction and all information in the wiki are simply placeholders for the content to come soon.</font>

<font color='green'>Note: Please do not hesitate with critics, recommendations or feature requests. Since the project is at an early phase you can actively shape it!</font>


# About #

Assembly3D is a collection of file-formats describing animated virtual 3D scenes with a focus on real-time use. This code project contains schemata, samples and tools for the individual formats.

## Format Design ##

The design is mainly influenced by the following concepts (for details see the [Formats](Formats.md) wiki page):

  * _Simplicity_: Parsing and using (e.g. rendering) the data should be easy and require only little code.

  * _Modularity_: Rather than having a single _über_ format, there are dedicated formats for sematic units such as meshes, materials, animations and so forth.

  * _Separation of Layout & Data_: Most data intensive formats are bipartite. There is a human readable part (XML, JSON) which describes the binary part (raw or compressed data).

  * _Hardware-friendly_: The file formats use simple array based structures and are designed for use with technologies such as OpenGL and OpenCL.


## Tools ##

A major focus of the project is to provide a set of tools to transfer assets from 3D modeling tools into a 'compiled' form (for details see the [Tools](Tools.md) wiki page). The main categories are:

  * _Shift_: Conversion tools which transform other 3D file-formats such as OBJ or COLLADA.

  * _Wiz(ard)_: Tools to manipulate and validate data. For example applying transformation to geometry.

  * _Prim(imitves)_: Generate files from parametric equations. For example meshed from Quadrics and Superquadrics.

  * _Test_: Utilities for automated unit tests. Mainly tools to compare files/data in various ways. E.g. using deltas or ignoring the sorting of attributes.

  * _Viewer_: Simple commandline application for displaying mesh files.

## Samples ##

Finally, example files for each format and source code are intended to further ease the use.

  * _Content_: A small set of animated 3D characters and scenes.

  * _Demo_: Various small applications like the **[WebViewer](http://assembly.interaction3d.org/samples/webgl/)** (requires a browser with [WebGL support](http://www.doesmybrowsersupportwebgl.com)).


<br />

---

<br />
# Sponsors #

| <a href='http://www.qu.tu-berlin.de'><img src='http://assembly.interaction3d.org/img/tu-berlin.gif' alt='TU Berlin' border='0' /></a> | <a href='http://www.laboratories.telekom.com/'><img src='http://assembly.interaction3d.org/img/t-labs.png' alt='Deutsche Telekom Laboratories' border='0' /></a> |
|:--------------------------------------------------------------------------------------------------------------------------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------|

<br />
# Statistics #

<a href='Hidden comment: 
<wiki:gadget url="http://www.ohloh.net/p/487938/widgets/project_factoids.xml" border="0" height="230" width="360"/>
'></a>

&lt;wiki:gadget url="http://www.ohloh.net/p/487938/widgets/project\_languages.xml" border="1" height="200" width="330"/&gt;