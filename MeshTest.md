**Quick Links**

<br />

# Description #

MeshTest is a tool for comparing Assembly3D _mesh_ files.

<br />
# Command line tool #

## Usage ##

```
working-dir$ ./MeshTest -a=<path/to/actual.xml> -e=<path/to/expected.xml> [OPTIONS]
```


## Options ##
```

   MeshTest  -a=<file-path> [--actual-binary=<file-path>] -e=<file-path>
             [--expected-binary=<file-path>] [--epsilon=<val>]
             [--ignore-order-groups] [--ignore-order-attributes]
             [--exclude-groups=<name-list>]
             [--exclude-attributes=<name-list>] [-q] [--] [--version] [-h]


Where: 

   -a=<file-path>,  --actual=<file-path>
     (required)  Actual file.

   --actual-binary=<file-path>
     Actual input binary file (default: same as actual file but with
     extension '.dat').

   -e=<file-path>,  --expected=<file-path>
     (required)  Expected file.

   --expected-binary=<file-path>
     Expected binary file (default: same as expected file but with
     extension '.dat').

   --epsilon=<val>
     Compares attribute values with a deviation epsilon (default: 0.0).

   --ignore-order-groups
     Ignores group order.

   --ignore-order-attributes
     Ignores attribute order.

   --exclude-groups=<name-list>
     List with groups to exclude

   --exclude-attributes=<name-list>
     List with attributes to exclude.

   -q,  --quite
     No output verbosity.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```