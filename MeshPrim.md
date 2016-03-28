**Quick Links**

<br />

# Description #

MeshPrim is a tool for generating Assembly3D _mesh_ files from parametric equations.

<br />
# Command line tool #

## Usage ##

```
working-dir$ ./MeshPrim -o=<FileName.mesh.xml> [OPTIONS]
```


## Options ##
```

   MeshPrim  {--plane=<half-extend>|--cube=<half-extend>
             |--sphere=<radius/slices/stacks>
             |--torus=<inner/outer/slices/stacks>
             |--disk=<inner/outer/slices/stacks>
             |--cylinder=<base/top/height/slices/stacks>
             |--trapezoid=<base/top/height/slices/stacks>
             |--rectangle=<extX/extY/slices/stacks>
             |--partial-disk=<inner/outer/slices/stacks/start/sweep>
             |--partial-cylinder=<base/top/height/slices/stacks/start/sweep
             >} -o=<output-name>
             [--generate-attributes=<positions/normals/texcoords>]
             [-d=<path>] [--] [--version] [-h]


Where: 

   --plane=<half-extend>
     (OR required)  Generates plane geometry.
         -- OR --
   --cube=<half-extend>
     (OR required)  Generates cube geometry.
         -- OR --
   --sphere=<radius/slices/stacks>
     (OR required)  Generates sphere geometry.
         -- OR --
   --torus=<inner/outer/slices/stacks>
     (OR required)  Generates torus geometry.
         -- OR --
   --disk=<inner/outer/slices/stacks>
     (OR required)  Generates disk primitive.
         -- OR --
   --cylinder=<base/top/height/slices/stacks>
     (OR required)  Generates cylinder primitive.
         -- OR --
   --trapezoid=<base/top/height/slices/stacks>
     (OR required)  Generates trapezoid primitive.
         -- OR --
   --rectangle=<extX/extY/slices/stacks>
     (OR required)  Generates rectangle primitive.
         -- OR --
   --partial-disk=<inner/outer/slices/stacks/start/sweep>
     (OR required)  Generates partial disk primitive.
         -- OR --
   --partial-cylinder=<base/top/height/slices/stacks/start/sweep>
     (OR required)  Generates partial cylinder primitive.


   -o=<output-name>,  --output-file-name=<output-name>
     (required)  Output file name.

   --generate-attributes=<positions/normals/texcoords>
     Specifies which attribute to generate.

   -d=<path>,  --output-dir=<path>
     Output directory (default=.).

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```