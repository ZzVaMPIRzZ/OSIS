#!/bin/bash
SIZE=150;
RANDOM_INT=$(od -An -t d -N"$(($SIZE*4))" /dev/random);
echo -n $RANDOM_INT > numbers.txt;