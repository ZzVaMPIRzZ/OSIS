#!/bin/bash
SIZE=150;
# while (( $SIZE > 0 )); do
RANDOM_INT=$(od -An -t d -N"$(($SIZE*4))" /dev/random);
echo -n $RANDOM_INT > numbers.txt;
# echo -n " " >> numbers.txt;
# let "SIZE-=1";
# done