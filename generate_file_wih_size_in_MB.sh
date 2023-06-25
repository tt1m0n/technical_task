# #!/bin/bash

# delete if exist
rm random_words_file.txt

# generates near 1MB size with >100 000 unique words. In my ubuntu 18.04 system this file has 102 305 unique words.
shuf -n 1000000 /usr/share/dict/words | tr '\n' ' ' > tmp_file.txt

touch random_words_file.txt

# get MB size from cmd
MB_loop_count=$1

for i in $(seq 1 $MB_loop_count)
do
    cat "tmp_file.txt" >> "random_words_file.txt"
done

rm tmp_file.txt