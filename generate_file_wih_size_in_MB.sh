#!/bin/bash

# delete if exist
rm random_words_file.txt


 
# for item in "${arr[@]}"
# do
#     echo ${arr[1 % 10]}
# done


arr=( "a" "b" "c" "d" "n" "m" "f" "o" "x" "k" "q")

# generates near 1MB size with >100 000 unique words. In my ubuntu 18.04 system this file has 102 305 unique words.
shuf -n 1000000 /usr/share/dict/words | tr '\n' ' ' | tr "'" 's' > tmp_file.txt

# replace and creating >1 000 000 unique words
prev_val=' '
touch tmp_file1.txt
for i in $(seq 0 11)
do
    sed -i "s+$prev_val+${arr[i % 10]} +g" tmp_file.txt
    prev_val="${arr[i % 10]} "
    cat "tmp_file.txt" >> "tmp_file1.txt"
done


touch random_words_file.txt

# get MB size from cmd
division=10
loop_count=$(($1/division))
for i in $(seq 1 $loop_count)
do
    cat "tmp_file1.txt" >> "random_words_file.txt"
done

rm tmp_file.txt
rm tmp_file1.txt