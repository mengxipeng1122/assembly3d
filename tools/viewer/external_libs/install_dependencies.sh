#!/bin/sh

####################################################

GLM_VERSION=0.9.2.3
GLM_URL="http://sourceforge.net/projects/ogl-math/files/glm-$GLM_VERSION/glm-$GLM_VERSION.zip"
GLM_ARCHIVE="glm-$GLM_VERSION.zip"

TCLAP_VERSION=1.2.1
TCLAP_URL="http://sourceforge.net/projects/tclap/files/tclap-$TCLAP_VERSION.tar.gz"
TCLAP_ARCHIVE="tclap.tar.gz"

####################################################
system_name=`uname -a`

echo "-----------------------------------------------"
echo "Downloading external libs for Assembly3D viewer."
echo "-----------------------------------------------\n"

if [ ! -d "glm-$GLM_VERSION" ]; then
    echo "downloading GLM $GLM_VERSION library"
    case $system_name in
        Darwin*)
            curl -L $GLM_URL -o $GLM_ARCHIVE
            ;;
        *)
            wget $GLM_URL -O $GLM_ARCHIVE
            ;;
    esac
    unzip $GLM_ARCHIVE
    rm $GLM_ARCHIVE
else
    echo "glm-$GLM_VERSION already installed.\n"
fi


if [ ! -d "tclap-$TCLAP_VERSION" ]; then
    echo "downloading TCLAP $TCLAP_VERSION library"
    case $system_name in
        Darwin*)
            curl -L $TCLAP_URL -o $TCLAP_ARCHIVE
            ;;
        *)
            wget $TCLAP_URL -O $TCLAP_ARCHIVE
            ;;
    esac
    tar -zxvf $TCLAP_ARCHIVE
    rm $TCLAP_ARCHIVE
else
    echo "tclap-$TCLAP_VERSION already installed.\n"
fi

echo "-----------------------------------------------\n"
