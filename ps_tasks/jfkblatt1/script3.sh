#!/bin/bash
# Script 3
INFILE=$1
OUTFILE=$2
if [ -e "$INFILE" ]
then
if [ -w "$OUTFILE" ]
then
cat "$INFILE" >> "$OUTFILE"
else
echo "can not write to $OUTFILE"
fi
else
echo "can not read from $INFILE"
fi
