#include <iostream>
#include "mem_sim_parser.h"
#include "mem_sim_cache.h"

int main(int argc, char *argv[]) {
    if (argc != 9) {
        cout << "ERROR: wrong number of CLI arguments" << endl;
        return 1;
    }
    
    ca.address_bits = atoi(argv[1]);
    ca.bytes_word = atoi(argv[2]);     // Power of 2
    ca.words_block = atoi(argv[3]);    // Power of 2
    ca.blocks_set = atoi(argv[4]);     // Power of 2
    ca.sets_cache = atoi(argv[5]);     // Power of 2
    ca.cycles_hit = atoi(argv[6]);
    ca.cycles_read = atoi(argv[7]);
    ca.cycles_write = atoi(argv[8]);    
    
    //ca.create();
    
    runParse();
    return 0;
}