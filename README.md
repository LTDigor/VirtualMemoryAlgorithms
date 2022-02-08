# VirtualMemoryAlgorithms
Implementation of various algorithms, and comparison of their efficiency and performance

- The number of frames available to the process is fixed.
- When a process needs some page, the operating system must make sure that it is loaded into RAM. The situation when there is no page in RAM at the time of the request is called a page fault.
- At this moment, the operating system should load it into one of the allocated process frames of RAM. If there are no free frames, then the operating system must unload one of the previously loaded pages and load the required one in its place.
- To determine the page to be unloaded, the operating system uses the page replacement algorithm. This program implements and compares several basic substitution algorithms that form the basis of those algorithms that are used in practice.
- Compaired algs is FIFO, LRU, CLOCK and OPT (optimal algorithm)
