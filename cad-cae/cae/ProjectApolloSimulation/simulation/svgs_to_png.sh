#!/bin/bash
if [ $# -eq 0 ];then
    echo "usage:  $0 <files to convert from svg to png>"
    echo "  file will be renamed from .svg to .png"
    exit 1
fi
for file in $@; do
    echo "file $file"
    if [ ${file: -4} == ".svg" ]; then
        inkscape -z -w 640 -h 480 $file -e ${file%.svg}.png
    else
        echo "file $file does not have svg extension, skipping"
    fi
done   
