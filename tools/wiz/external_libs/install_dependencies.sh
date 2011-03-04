#!/bin/sh

####################################################

TCLAP_VERSION=1.2.0
TCLAP_URL="http://sourceforge.net/projects/tclap/files/tclap-$TCLAP_VERSION.tar.gz"
TCLAP_ARCHIVE="tclap.tar.gz"

TIXML_VERSION=2.6.1
TIXML_VERSION2=2_6_1
TIXML_URL="http://sourceforge.net/projects/tinyxml/files/tinyxml/$TIXML_VERSION/tinyxml_$TIXML_VERSION2.zip"
TIXML_ARCHIVE="tinyxml.zip"

####################################################
system_name=`uname -a`

echo "-----------------------------------------------"
echo "Downloading external libs for Assembly3D tools."
echo "-----------------------------------------------\n"

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

if [ ! -d "tinyxml" ]; then
    echo '\ndownloading tinyxml version: $TIXML_VERSION library'
    case $system_name in
        Darwin*)
            curl -L $TIXML_URL -o $TIXML_ARCHIVE
            ;;
        *)
            wget $TIXML_URL -O $TIXML_ARCHIVE
            ;;
    esac
    unzip $TIXML_ARCHIVE
    rm $TIXML_ARCHIVE
else
    echo "\ntinyxml already installed.\n"
fi

echo "-----------------------------------------------\n"
