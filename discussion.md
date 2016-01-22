Discussion
==========
This file describes how I developed, tested and debugged the simulator
 
Development
-----------
The cache simulator consists of two main parts:

  - The input parser module `mem_sim_parser.cpp`
  - The memory and cache simulator `mem_sim_cache.cpp`

### The input parser module
I began by writing the parser module so I could later test
the actual cache simulator easier.

The parser reads from the stdin line by line and has low tolerance for error.
If wrong number of arguments are given for any instruction, the parser displays
a user friendly error message, which includes the line number and the
description of the error.

If a line has been successfully parsed, the parser calls the appropriate function
in the cache simulator.

### The cache simulator
The cache is implemented using a vector of vectors.
The cache is a vector of sets and each set is a vector of type `cache_block`.
Each cache_block includes an array of bytes that store the data, a dirty bit and a tag.

To implement the LRU policy, each `cache_block` is being put at the
end of the set on every access. When a cache block needs to be removed,
it is the one with index [0] in the set that's been used least recently.

At the start of the program, all sets are empty vectors. 

Testing & Debugging
-------------------
I tested the simulator with the given test files as well as with some of my own input files.
I included some of these.

The code outputs a constant stream of comments which include what blocks are currently in the
cache, whether they're dirty or not. There is a separate indication every time a block is being
written back to the memory.