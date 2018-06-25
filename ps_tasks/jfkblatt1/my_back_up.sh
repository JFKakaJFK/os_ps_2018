#!/bin/bash
# This bash script creates a backup of the current directory
# - if the target path already exists, only newer files will be copied
# - if the target path doesn't exist, it will be created

# check the number of arguments
if (( $# != 1 )) ; then
  # trow an error if the argument is invalid
  echo "Error. Invalid number of arguments" >&2
  echo "usage: $0 <path to target directory>" >&2
  exit 1
else
  # check if the directory exists
  WD=`pwd`
  DIRNAME=`basename "$WD"`
  DIR=$1
  if [ -e "$DIR" ] ; then
    # check for each file/dir individually if it is newer
    # for FILE in `ls *` ; do
    for FILE in * ; do
      # check if the file exists
      if [ -e "$DIR/$DIRNAME/$FILE" ] ; then
        # if the file exists, check its age
        if [ "$FILE" -nt "$DIR/$DIRNAME/$FILE" ] ; then
          # overwrite without asking
          \cp -rf "$FILE" "$DIR/$DIRNAME"
        fi
      else
        cp -rf "$FILE" "$DIR/$DIRNAME"
      fi
    done
  else
    # if the dir doesn't exist, make it
    mkdir -p "$DIR" && cp -rf "$WD" "$DIR"
  fi
fi
