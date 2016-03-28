**Quick Links**

<br />

# Restrictions #

## Mesh ##

Supported Primitive Types:
  * triangles
  * polylist

Restrictions on Primitive Groups:
  * all must have exactly the same 'inputs'


Restrictions on Input Sources:
  * must be local to mesh

Restrictions on Sources:
  * accessors must only consist of type="float" params
  * there must be between 1-4 params
  * offset must be 0 and stride must equal the number of params
  * source must be a local float array
  * param names/semantic are ignored


For transformation:

Supported Inputs Semantics:
  * POSITION
  * NORMAL
  * TEXCOORD
  * TEXTANGENT
  * TEXBINORMAL

Restrictions on Inputs:
  * only set="0"