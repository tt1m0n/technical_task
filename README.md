# technical_task

### Main Solution

[ParallelIfstreamRecursive](https://github.com/tt1m0n/technical_task/blob/main/include/ParallelIfstreamRecursive.hpp)
The idea is:
  1) Split file into chunks.
  2) for each chunk make call std::async recursively:
  - open ifstream
  - read word by word directly from the stream until until sum of characters >= lenght.
  - save into unordered_set
 3) merged all unordered_sets from different threads
 4) return size


### Second Solution
[ParallelMmapRecursive](https://github.com/tt1m0n/technical_task/blob/main/include/ParallelMmapRecursive.hpp)
Same as Main but use file mapping.

### Thoughts, Ideas, Drafts
[drafts](https://github.com/tt1m0n/technical_task/blob/main/include/basic_draft_ideas.hpp)

In this file I just wanted to show some part of my thoughts while trying to find solution. Here I tried such 
