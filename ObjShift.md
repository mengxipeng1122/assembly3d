**Quick Links**

<br />

# Description #

ObjShift is a tool for converting geometric data from [OBJ format](http://local.wasp.uwa.edu.au/~pbourke/dataformats/obj/) to Assembly3D _mesh_ files.
<br />


# Command line tool #

## Usage ##

```
working-dir$ java -jar ObjShift.jar [-m] <path/to/model.obj> [output/directory]
```

## Options ##

```
    ObjShift.jar [-m] <source-file> [output-dir]


    Where:

        -m, --multiple
         creates separate(multiple) mesh files for each object
         note that this applies to o(object name) but not g (group names)

        <source-file>
        (required) File to convert.

        [output-dir]
        the output directory where the mesh files will be written to. The current directory if not specified.
```
<br />


# Web #

> | &lt;wiki:gadget url="http://assembly.interaction3d.org/tools/shift/obj/gadget.xml" height="300" border="0" /&gt; |
|:-----------------------------------------------------------------------------------------------------------------|