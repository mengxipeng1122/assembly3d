**Quick Links**

<br />

# Schema #

This format describes the animations with key-frames grouped into channels. Each channel groups attributes, such as key-times, translations and rotations.

Similar to most Assembly3D formats, the data intensive components are outsourced to a binary file while the layout is specified via XML or JSON.

## XML ##

XML-Schema: [anim.xsd](http://assembly.interaction3d.org/xml/schema/anim.xsd)

```
<Animation xmlns="http://assembly.interaction3d.org/anim">
  <Sampler duration="5.0" channels="1">
    <Channel name="Root" keyframes="2" from="0.0" to="1.0" attributes="3">
      <Attribute name="TIME" size="1" type="FLOAT"/>
      <Attribute name="TRANSLATION" size="3" type="FLOAT"/>
      <Attribute name="ROTATION" size="4" type="FLOAT"/>
    </Channel>
    <Channel name="Spline" keyframes="5" from="0.1" to="0.75" attributes="1">
      <Attribute name="POSITION" size="3" type="FLOAT"/>
    </Channel>
  </Sampler>
</Animation>
```

## JSON ##

XML-Transformation: [anim.xsl](http://assembly.interaction3d.org/xml/transform/anim.xsl)

```
{
  "duration": 1.0,	
  "channels": 
  [
    {
      "name": "Root", "keyframes": 2, "from": 0.0, "to": 1.0,
      "attributes":
      [
        { "name": "TIME", "size": 1, "type": "FLOAT" },
        { "name": "TRANSLATE", "size": 3, "type": "FLOAT" },
        { "name": "ROTATE", "size": 4, "type": "FLOAT" }
      ]
    },
    {
      "name": "Spline", "keyframes": 5, "from": 0.1, "to": 0.75,
      "attributes":
      [
        { "name": "POSITION", "size": 3, "type": "FLOAT" }
      ]
    }
  ]
}
```

## Data ##

```
ChannelAttribute<keyframes>
{
  // type: FLOAT(32bit)
  // size: positive integer
  type name[keyframes][size]; 
};
```


<br />
# Common Profile #

The following channel attributes are part of the core profile.

| Name | Size | Relative |
|:-----|:-----|:---------|
| `TIME` | `1`  | `false`  |
| `TRANSLATION` | `3`  | `true`   |
| `ROTATION` | `4`  | `true`   |
| `POSITION` | `2`  | `false`  |
| `ORIENTATION` | `4`  | `false`  |

Note: Rotations and Orientations are expected to be unit quaternions in the common profile, but this is not required otherwise.

<br />
# F.A.Q. #

Q: Is it possible to _group_ animations (have sub-animations) and describe transitions and blends?
> A: NO, not in this format at least. However, it is likely that there will be a dedicated format referring to the key-frame animation files.


Q: How are the key-frames _interpolated_? Is there a way to specify a method like _linear_, or _cubic_?
> A: YES, somehow. The format structure itself is agnostic to the interpolation. The default is linear but any can be used as long as the required attributes, like tangents, are present. However, we recommend to encode the interpolation type into the channel names.