#! /bin/sh

OUTPUT_DIR=./input

if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir $OUTPUT_DIR
fi

FILE_EXT=".mesh.xml"

MeshPrim -d=$OUTPUT_DIR -o=Plane$FILE_EXT --plane=1
MeshWiz $OUTPUT_DIR/Plane$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

MeshPrim -d=$OUTPUT_DIR -o=Cube$FILE_EXT --cube=1
MeshWiz $OUTPUT_DIR/Cube$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

MeshPrim -d=$OUTPUT_DIR -o=Sphere$FILE_EXT --sphere=1/32/32
MeshWiz $OUTPUT_DIR/Sphere$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

MeshPrim -d=$OUTPUT_DIR -o=Torus$FILE_EXT --torus=0.5/1.0/32/32
MeshWiz $OUTPUT_DIR/Torus$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

MeshPrim -d=$OUTPUT_DIR -o=Rectangle$FILE_EXT --rectangle=1/1/1/1
MeshWiz $OUTPUT_DIR/Rectangle$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

MeshPrim -d=$OUTPUT_DIR -o=Trapezoid$FILE_EXT --trapezoid=0.5/1.0/1.0/32/32
MeshWiz $OUTPUT_DIR/Trapezoid$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

MeshPrim -d=$OUTPUT_DIR -o=Cylinder$FILE_EXT --cylinder=1/0.5/2/32/32
MeshWiz $OUTPUT_DIR/Cylinder$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

MeshPrim -d=$OUTPUT_DIR -o=Disk$FILE_EXT --disk=0.5/1.0/32/32
MeshWiz $OUTPUT_DIR/Disk$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

# MeshPrim -d=$OUTPUT_DIR -o=PartialCylinder$FILE_EXT --partial-cylinder=
# MeshWiz $OUTPUT_DIR/PartialCylinder$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q

# MeshPrim -d=$OUTPUT_DIR -o=PartialDisk$FILE_EXT --partial-disk=
# MeshWiz $OUTPUT_DIR/PartialDisk$FILE_EXT -o=$OUTPUT_DIR --dump-txt -q
