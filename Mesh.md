**Quick Links**

<br />

# Schema #

This format describes the geometry and topology of triangle meshes. First, there is a list of vertices grouped by their attributes, such as positions and normals. The second part is a single list of vertex indices describing the triangles. Groups are used to partition the surface which allows to apply different materials to the mesh.

Similar most Assembly3D formats, the data intensive components are outsourced to a binary file while the layout is specified via XML or JSON.


## XML ##

XML-Schema: [mesh.xsd](http://assembly.interaction3d.org/xml/schema/mesh.xsd)

```
<Mesh xmlns="http://assembly.interaction3d.org/mesh">
  <Vertices count="24" attributes="3">
    <Attribute name="POSITION" size="3" type="FLOAT" />
    <Attribute name="NORMAL" size="3" type="FLOAT" />
    <Attribute name="TEXCOORD" size="2" type="FLOAT" />
  </Vertices>
  <Triangles type="UNSIGNED_SHORT" groups="3">
    <Group name="X" count="4" />
    <Group name="Y" count="4" />
    <Group name="Z" count="4" />
  </Triangles>	
</Mesh>
```


## JSON ##

XML-Transformation: [mesh.xsl](http://assembly.interaction3d.org/xml/transform/mesh.xsl)

```
{
  "vertices": 
  {
    "count": 24,
    "attributes": 
    [
      { "name": "POSITION", "size": 3, "type": "FLOAT" },
      { "name": "NORMAL", "size": 3, "type": "FLOAT"  },
      { "name": "TEXCOORD", "size": 2, "type": "FLOAT"  }
    ]
  },
  "triangles": 
  {
    "groups": 
    [
      "type": "UNSIGNED_SHORT", 
      { "name": "X", "count": 4, },
      { "name": "Y", "count": 4, },
      { "name": "Z", "count": 4, }
    ]
  }
}
```


## Data ##

```
VertexAttribute<vertices>
{
  // type: FLOAT(32bit)
  // size: 1, 2, 3 or 4
  type name[vertices][size]; 
};
```

```
TriangleGroup<type>
{
  // type: UNSIGNED_BYTE(8bit), UNSIGNED_SHORT(16bit), UNSIGNED_INT(32bit)
  type name[triangles][3]; 
};
```

Note: All types are stored in the [little endian](http://en.wikipedia.org/wiki/Endianness) byte order.


<br />
# Common Profile #

The following vertex attributes part of the core profile.

| Name | Size | Optional |
|:-----|:-----|:---------|
| `POSITION` | `3`  | `false`  |
| `NORMAL` | `3`  | `true`   |
| `TEXCOORD` | `2`  | `false`  |
| `TANGENT` | `3`  | `true`   |
| `BITANGENT` | `3`  | `true`   |


<br />
# F.A.Q. #

Q: In the XML description `attributes` and `groups` are _redundant_. Are these required?
> A: YES. These helpful for sequential XML parsers (like SAX) and exact memory allocation.


Q: Is there way to _interleave_ the vertex attributes? After all, such layouts are more efficient with current graphics cards.
> A: NO, not currently at least. In favor of simplicity we decided against it. However, this feature can introduced easily by adding `stream` index to the vertex attributes.


Q: The index type for triangles is the same for all groups. Is it possible to specify a different type for each group?
> A: NO. Again simplicity is the main reason. Also, with a single type it is possible to render the mesh with one _draw call_ only. Although the type is indirectly  determined by the highest vertex index in a group  and these are expected to be similar across.


Q: Is it possible that some triangles are part of more than a single group?
> A: NO, not currently at least. Triangle groups are non-overlapping _partitions_. This was chosen for simplicity, but can be changed if there is enough interest. A solution would be to add a `count` to the triangles and a `start` index to each group.

Q: Are there any other primitives types than triangles? For examples points & lines and variatons like loops, strips and fans?
> A: NO, not currently at least. Once again these were omitted for simplicity reasons. However, additions would be possible if there is enough interest.