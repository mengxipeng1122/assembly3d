#! /bin/sh

INPUT_DIR=$PWD/input
ACTUAL_DIR=$PWD/output/actual
EXPECTED_DIR=$PWD/output/expected
EPSILON=0.0

MESH=Cube.mesh.xml

if [ -f "$INPUT_DIR/$1" ]; then
    MESH=$1
fi

echo $MESH

echo "Running mesh test..."

#-----------------------------------------------

echo "---------------"
echo "Translation..."
MeshWiz $INPUT_DIR/$MESH -q -o=$ACTUAL_DIR/translation -t=1/2/3
MeshTest -a=$ACTUAL_DIR/translation/$MESH -e=$EXPECTED_DIR/translation/$MESH

#-----------------------------------------------

echo "---------------"
echo "Rotation..."
MeshWiz $INPUT_DIR/$MESH -q -o=$ACTUAL_DIR/rotation -r=-45/0/1/0
MeshTest -a=$ACTUAL_DIR/rotation/$MESH -e=$EXPECTED_DIR/rotation/$MESH

#-----------------------------------------------

echo "---------------"
echo "Scaling..."
MeshWiz $INPUT_DIR/$MESH -q -o=$ACTUAL_DIR/scale -s=2/2/2
MeshTest -a=$ACTUAL_DIR/scale/$MESH -e=$EXPECTED_DIR/scale/$MESH

#-----------------------------------------------

# echo "---------------"
# echo "Resizing..."

#-----------------------------------------------
