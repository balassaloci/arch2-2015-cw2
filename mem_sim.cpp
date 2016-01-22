#include <iostream>
#include "mem_sim_parser.cpp"

int main(int argc, char *argv[]) {
    if (argc != 9) {
        cout << "ERROR: wrong number of CLI arguments" << endl;
        return 1;
    }
    
    c.address_bits = atoi(argv[1]);
    c.bytes_word = atoi(argv[2]);     // Power of 2
    c.words_block = atoi(argv[3]);    // Power of 2
    c.blocks_set = atoi(argv[4]);     // Power of 2
    c.sets_cache = atoi(argv[5]);     // Power of 2
    c.cycles_hit = atoi(argv[6]);
    c.cycles_read = atoi(argv[7]);
    c.cycles_write = atoi(argv[8]);    
    
    //c.create();
    
    runParse();
    return 0;
}