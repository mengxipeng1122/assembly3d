**Quick Links**

<br />


## XML ##

XML-Schema: [scene.xsd](http://assembly3d.googlecode.com/hg/xml/schema/scene.xsd)

```
<Scene xmlns="http://assembly.interaction3d.org/scene">
  <World objects="3">
    <Object name="Cube" scale="2.0">
      <Position x="0" y="2" z="0"/>
      <Orientation x="0" y="0" z="0" w="1"/>
    </Object>
    <Object name="Cube.dark[0]">
      <Position x="-3" y="0" z="0"/>
      <Orientation x="0" y="0" z="0"/>
    </Object>
    <Object name="Cube.dark[1]">
      <Position x="+3" y="0" z="0"/>
      <Orientation x="0" y="0" z="0"/>
    </Object>
  </World>	
</Scene>
```


## JSON ##

XML-Transformation: [scene.xsl](http://assembly3d.googlecode.com/hg/xml/transform/scene.xsl)

```
{
  "objects": 
  [
    {
      "name": "Cube", "scale": 2.0,
      "position": [ 0, 2, 0 ], 
      "orientation": [ 0.0, 0.0, 0.0, 1.0 ]       
    },
    {
      "name": "Cube.dark[0]",
      "position": [ -3, 0, 0 ], 
      "orientation": [ 0.0, 0.0, 0.0 ]       
    },
    {
      "name": "Cube.dark[1]",
      "position": [ +3, 0, 0 ], 
      "orientation": [ 0.0, 0.0, 0.0 ]       
    },
  ]
}
```