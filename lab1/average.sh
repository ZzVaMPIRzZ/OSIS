#!/bin/bash
sum=0;
a=( $* );
word_count=${#a[@]};
for param in ${a[@]};
do
	sum=$(($sum+$param));
done
echo "Count: $word_count";
r=$(echo "scale=3; $sum/$word_count" | bc);
echo "Answer: $r";
