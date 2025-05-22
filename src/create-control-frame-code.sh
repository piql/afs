#!/bin/bash
#
# Script for creating control frame 'boot vector'
#

# Supported control frame versions:
declare -a cfs=("reel-4k-controlframe-v7.xml" "reel-4k-controlframe-v6.xml" "reel-4k-controlframe-v5.xml" "reel-4k-controlframe-v4.xml" "reel-4k-controlframe-v3.xml")

CFBF_H_OUT="$PWD/controlframeboxingformat.h"
CFBF_C_OUT="$PWD/controlframeboxingformat.c"

osx_trick=
if [ "$OSTYPE" == "darwin16" ] || [ "$OSTYPE" == "darwin18" ] ; then
   osx_trick="''"
fi

#
# Create .cpp file
#
echo '#include "../inc/controlframeboxingformat.h"' > $CFBF_C_OUT
echo >> $CFBF_C_OUT

cfvars=()
# Convert files
for c in "${cfs[@]}"
do

   cf="../conf/$c"
   variable=`echo "afs_$(basename $cf .xml)" | sed -e "s/-/_/g"`
   cfvars+=($variable)

   cp "$cf" "$variable"
   chmod u+rw "$variable"
   echo "// Generated using: xxd -i $variable source file=$cf" >> $CFBF_C_OUT 
   xxd -i $variable >> $CFBF_C_OUT
   # Replace variable definition to macros definition
   sed -i $osx_trick 's/unsigned int afs_/#define afs_/g' $CFBF_C_OUT || (echo "sed error"; exit 1)
   sed -i $osx_trick 's/ = \(.*[0-9]\);$/ \1/g'  $CFBF_C_OUT  || (echo "sed error"; exit 1)
   rm "$variable"
   echo >> $CFBF_C_OUT
done

# Create vectors
echo -n "unsigned char* afs_control_frame_format[AFS_FRAME_FORMAT_MAX] = { " >> $CFBF_C_OUT 
for cf in "${cfvars[@]}"
do
   echo -n "${cf}, " >> $CFBF_C_OUT 
done
echo "};" >> $CFBF_C_OUT 
echo >> $CFBF_C_OUT

echo -n "const unsigned int afs_control_frame_format_len[AFS_FRAME_FORMAT_MAX] = { " >> $CFBF_C_OUT 
for cf in "${cfvars[@]}"
do
   echo -n "${cf}_len, " >> $CFBF_C_OUT 
done
echo "};" >> $CFBF_C_OUT 
echo >> $CFBF_C_OUT

# Remove last comma in array definition
sed -i $osx_trick 's/, };/ };/g' $CFBF_C_OUT  || (echo "sed error"; exit 1)

#
# Create .h file
#
echo '#ifdef __cplusplus' > $CFBF_H_OUT 
echo 'extern "C" {' >> $CFBF_H_OUT 
echo '#endif' >> $CFBF_H_OUT 
echo >> $CFBF_H_OUT
echo >> $CFBF_H_OUT

cfcount=${#cfvars[@]}
for (( i=$(($cfcount-1)); i>=0; i-- ));
do
    echo "#define AFS_FRAME_FORMAT_V$(($i+1)) $i" >> $CFBF_H_OUT
done
echo "#define AFS_FRAME_FORMAT_MAX $cfcount" >> $CFBF_H_OUT

echo "extern unsigned char* afs_control_frame_format[AFS_FRAME_FORMAT_MAX];" >> $CFBF_H_OUT
echo "extern const unsigned int afs_control_frame_format_len[AFS_FRAME_FORMAT_MAX];" >> $CFBF_H_OUT
echo >> $CFBF_H_OUT
echo '#ifdef __cplusplus' >> $CFBF_H_OUT 
echo '}' >> $CFBF_H_OUT 
echo '#endif' >> $CFBF_H_OUT 
echo >> $CFBF_H_OUT

