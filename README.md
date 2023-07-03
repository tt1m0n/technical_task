To build app open it with VS Code and build project. Or to quickly test you can use executable file `main` which uses `Main Solution` logic.

To generate file with random words use next command:

- `./generate_file_with_sin_in_MB.sh 100` where 100 is ~100MB. It will generate `random_words_file.txt` file with 2,15M(const) unique words.
- `./generate_file_with_sin_in_MB.sh 30000` It will generate `random_words_file.txt` with size ~30GB. and 2,15M(const) unique words

To start `main` app:
`./main randow_words_file.txt`


### Main Solution

- size: 24MB,   unique_words: 2 150 000, time: 4410 ms
- size: 120MB,  unique_words: 2 150 000, time: 6493 ms
- size: 600MB,  unique_words: 2 150 000, time: 9425 ms
- size: 2.4GB,  unique_words: 2 150 000, time: 20004 ms
- size: 12.5GB, unique_words: 2 150 000, time: 90976 ms
- size: 24GB,   unique_words: 2 150 000, time: ~115328 ms
- size: 36GB,   unique_words: 2 150 000, time: ~123374 ms
  
[ParallelIfstreamRecursive](https://github.com/tt1m0n/technical_task/blob/main/include/ParallelIfstreamRecursive.hpp)
The idea is:
  1) Split file into chunks.
  2) for each chunk make call std::async recursively:
  - open ifstream
  - read word by word directly from the stream until until sum of characters >= lenght.
  - save into unordered_set (before it I called rehash to create buckets)
 3) merged all unordered_sets from different threads
 4) return size


### Second Solution
[ParallelMmapRecursive](https://github.com/tt1m0n/technical_task/blob/main/include/ParallelMmapRecursive.hpp)
Same as Main but use file mapping.


- size: 24MB, unique_words: 2 150 000, time: 2416 ms
- size: 120MB, unique_words: 2 150 000, time: 6555 ms
- size: 600MB, unique_words: 2 150 000, time: 10035 ms
- size: 2.4GB, unique_words: 2 150 000, time: 21240 ms
- size: 12.5GB, unique_words: 2 150 000, time: process killed by system because of huge memory usage. It happened because here I save info from mapped file to std::string 

### Thoughts, Ideas, Drafts
[drafts](https://github.com/tt1m0n/technical_task/blob/main/include/basic_draft_ideas.hpp)

In this file I just wanted to show some part of my thoughts while trying to find better solution. Here I tried such variants:

1) parallel: open ifstream -> read chunk, save to string and close fstream ->split string and save to unordered -> merged all unordered

- size: 24MB, unique_words: 2 150 000, time: 1900 ms
- size: 120MB,  unique_words: 2 150 000, time: 6167 ms
- size: 600MB, unique_words: 2 150 000, time:  10025 ms
- size: 2.4GB, unique_words: 2 150 000, time: 21805 ms
- size: 12.5GB, unique_words: 2 150 000, time: process killed by system because of huge memory usage. It happened because here I save info from mapped file to std::string 

2) 1 thread: read using stream -> save to vector -> sort vector (can do with std::execution::par) -> std::unique

- size: 24MB unique_words: 2 150 000 time: 4500 ms
- size: 120MB unique_words: 2 150 000 time: 21534 ms
- size: 600MB unique_words: 2 150 000 time:  103867 ms
- no check - slow


3) 1 thread: read using stream -> save to unordered_set

- size: 24MB unique_words: 2 150 000 time: 1389 ms
- size: 120MB unique_words: 2 150 000 time: 4627 ms
- size: 600MB unique_words: 2 150 000 time:  20022 ms
- size: 2.4GB unique_words: 2 150 000 time: 78815 ms
- no check - slow

4) 1 thread: read using stream -> save to reserved unordered_set

- size: 24MB unique_words: 2 150 000 time: 954 ms
- size: 120MB unique_words: 2 150 000 time: 3982 ms
- size: 600MB unique_words: 2 150 000 time: 18576 ms
- size: 2.4GB unique_words: 2 150 000 time: 73334 ms
- no check - slow

5) 1 thread: map file -> save to unordered set

- size: 24MB unique_words: 2 150 000 time: 961 ms
- size: 120MB unique_words: 2 150 000 time: 4090 ms
- size: 600MB unique_words: 2 150 000 time: 19059 ms
- size: 2.4GB unique_words: 2 150 000 time: 75239 ms
- no check - slow



### Points for possible improvements:
1) use custom hashmap which has better performance. [Here](https://martin.ankerl.com/2019/04/01/hashmap-benchmarks-01-overview/) the bunch of variants with explanation, links, benchmarks. If it is not possible to use custom so we can try to use custom hash function, make rehash for creating buckets to reduce memory operation for copy info, find free memory, etc
2) use string_view in unordered_map instead of std::string. But I am not sure it gives a lot of improvements.
3) more research how efficienly read huge file and better work with data
4) we can more play with package_task, thread pools, consumer-producer logic (some threads save info to stack/queue of unordered sets, another threads merging them). Great book to read. [C++ Concurrency in Action](https://www.amazon.com/C-Concurrency-Action-Practical-Multithreading/dp/1933988770). Great course-video to watch [Modern C++ Concurrency in Depth ( C++17/20)](https://ciklum.udemy.com/course/modern-cpp-concurrency-in-depth/learn/lecture/7670484#overview)
5) think about I/O bound, CPU bound
6) Use another container instead of hashmap. For example Trie.

### Useful Links:

- [Want fast C++? Know your hardware!](https://www.youtube.com/watch?v=BP6NxVxDQIs&t=0)
- [Explanation and thoughts about this task](https://levelup.gitconnected.com/complex-solution-to-a-dead-simple-concurrency-task-5a66ae21e3b)
- [What do t he term i/o and cpu bound mean](https://stackoverflow.com/questions/868568/what-do-the-terms-cpu-bound-and-i-o-bound-mean/33510470#33510470)
- [reddit discussion about mmap](https://www.reddit.com/r/cpp/comments/ng9xz1/fastest_way_to_read_very_large_filegb_to_tb_in_c/)
- [using c++ containers efficiently](https://blog.quasar.ai/using-c-containers-efficiently)
- [different hashmap implementation and benchmarks](https://martin.ankerl.com/2019/04/01/hashmap-benchmarks-01-overview/)
- [Udemy course about multithreading](https://ciklum.udemy.com/course/modern-cpp-concurrency-in-depth/learn/lecture/7670484#overview)
- [C++ Concurrency in Action](https://www.amazon.com/C-Concurrency-Action-Practical-Multithreading/dp/1933988770)
