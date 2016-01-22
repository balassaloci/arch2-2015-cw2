#ifndef MEM_SIM_CACHE
#define MEM_SIM_CACHE
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>


using namespace std;


unsigned int get_memory_size(unsigned int address_bits);

unsigned int block_align(unsigned int byte_address,
                            unsigned int bytes_word,
                            unsigned int words_block);

unsigned int set_index(unsigned int block_address,
                       unsigned int sets_cache);

unsigned int tag_index(unsigned int sets_cache,
                     unsigned int block_address);

struct cache_block {
public:
    unsigned char * bytes;
    bool dirty;
    int tag;
};

struct cache {
public:
    unsigned int address_bits;
    unsigned int bytes_word;     // Power of 2
    unsigned int words_block;    // Power of 2
    unsigned int blocks_set;     // Power of 2
    unsigned int sets_cache;     // Power of 2
    unsigned int cycles_hit;
    unsigned int cycles_read;
    unsigned int cycles_write;
    ////////////
    vector<vector<cache_block>> data;
    unsigned char * memory;
    
    void create();
    
    int find_index(unsigned int tag, unsigned int set);
    
    bool flush_block(unsigned int index, unsigned int set);
    
    void block_from_memory(unsigned int block_address, 
                            unsigned int set, 
                            unsigned int tag);
    
    void print_cache_state();
    
    void read(unsigned int byte_address);
    
    void write(unsigned int byte_address, unsigned char * newdata);
    
    void flush_request();
};

//Global variable ugh
extern cache ca;

#endif