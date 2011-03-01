#!/bin/sh

if [ -d "classes" ]; then
	rm -R classes
fi

mkdir -p classes
javac -d classes/ ../obj/*.java
jar cfm ObjShift.jar OBJ-MANIFEST.MF -C  classes/ .
chmod +x ObjShift.jar
