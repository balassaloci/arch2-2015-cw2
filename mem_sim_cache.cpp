#include "mem_sim_cache.h"

unsigned int get_memory_size(unsigned int address_bits) {
    return pow(2, address_bits);
}

unsigned int block_align(unsigned int byte_address,
                            unsigned int bytes_word,
                            unsigned int words_block) {
    
    unsigned int divider = bytes_word * words_block;
    unsigned int divided = byte_address / divider;
    divided = divided * divider;
    return divided;
}

unsigned int set_index(unsigned int block_address,
                       unsigned int sets_cache) {
    //block_address /= (2 * 2);
    unsigned int index = block_address % sets_cache;
             
    return index; 
}

unsigned int tag_index(unsigned int sets_cache,
                     unsigned int block_address) {
                         
    unsigned int index = block_address / sets_cache;              
    
    return index;
}

void cache::create() {
    //Creating cache
    for (int i = 0; i < sets_cache; i++) {
        vector<cache_block> set;
        data.push_back(set);
    }
    
    //Creating memory
    unsigned int memory_size = get_memory_size(address_bits); 
    memory = new unsigned char [memory_size];
    for (unsigned int i = 0; i < memory_size; i++) {
        memory[i] = 0;
    }
}

int cache::find_index(unsigned int tag, unsigned int set) {
    for (int i = 0; i < data[set].size(); i++) {
        if (data[set][i].tag == tag) {
            //cout << "{index for tag " << tag << " set "
            //     << set << " found: " << i << "}" << endl;
            return i;
        }
    }
    
    return -1;
}

bool cache::flush_block(unsigned int index, unsigned int set) {
    if (data[set].size() <= index) {
        cout << "# Invaild set index is being flushed" << endl;
        return false;
    }
    if (index < 0) {
        return false;
    }
    if (data[set][index].dirty) {
        cout << "#set: " << set << " index " << index << " tag " << data[set][index].tag
                << " sets/cache " << sets_cache << endl;
        
        unsigned int blockaddress = set + data[set][index].tag * sets_cache;
        unsigned int blocksize = bytes_word * words_block;
        blockaddress *= blocksize;
        cout << "# Flushing back to addr " << hex << blockaddress << dec << endl;
        //copying content
        for (unsigned int i = 0; i < blocksize; i++) {
            memory[i + blockaddress] = data[set][index].bytes[i];
        }
        data[set][index].dirty = false;
        return true;
    } else {
        return false;
    }
}

void cache::block_from_memory(unsigned int block_address, 
                        unsigned int set, 
                        unsigned int tag) {
    unsigned int blocksize = bytes_word * words_block;
    cache_block block;
    block.dirty = false;
    block.bytes = new unsigned char [blocksize];
    block.tag = tag;
    
    for (unsigned int i = 0; i < blocksize; i++) {
        block.bytes[i] = memory[i + block_address];
    }
    
    data[set].push_back(block);
}


void cache::print_cache_state() {
    //cout << data.size() << " : " << data[0].size() << endl;
    //return;
    cout << "# ";
    
    for (int i = 0; i < data.size(); i++) {
        cout << "S" << i << ": ";
        for (int j = 0; j < data[i].size(); j++) {
            cout << data[i][j].tag << ".";
            if (data[i][j].dirty) {
                cout << "D";
            } else {
                cout << "c";
            }
            cout <<", ";
        }
        cout << "| ";
    }
    cout << endl;
}

void cache::read(unsigned int byte_address) {
    unsigned int block_address = block_align(byte_address, bytes_word, words_block);
    unsigned int block_index = block_address / (bytes_word * words_block);
    unsigned int tag = tag_index(sets_cache, block_index);
    unsigned int set = set_index(block_index, sets_cache);
    
    int index = find_index(tag, set);
    bool hit;
    int time = 1;
    if (index >= 0) {
        //noo mem request
        cache_block block = data[set][index];
        data[set].erase(data[set].begin() + index);
        data[set].push_back(block);
        hit = true;
        //print_word();
    } else {
        bool flushed;
        if (data[set].size() == blocks_set) {
            flushed = flush_block(0, set);
            data[set].erase(data[set].begin());
            block_from_memory(block_address, set, tag);
            
            if (flushed) {
                time += cycles_write;
            }                
        } else {
            block_from_memory(block_address, set, tag);
        }
        time += cycles_read;
        hit = false;
    }
    
    cout << "read-ack " << set;
    if (hit) {
        cout << " hit ";
    } else {
        cout << " miss ";
    }
    
    cout << time; // TIME COMING HERE
    
    cout << " ";
    unsigned int readblock = data[set].size() - 1;
    unsigned int reladdress =  byte_address - block_address;
    for (int i = 0; i < bytes_word; i++) {
        unsigned int printbyte = data[set][readblock].bytes[reladdress + i];
        cout << hex << uppercase << printbyte << nouppercase;
        cout << dec;
    }
    
    cout << endl;
    print_cache_state();
    //cout << endl;
    //cout << "read req triggered" << endl;
}

void cache::write(unsigned int byte_address, unsigned char * newdata) {
    
    unsigned int block_address = block_align(byte_address, bytes_word, words_block);
    unsigned int block_index = block_address / (bytes_word * words_block);
    unsigned int tag = tag_index(sets_cache, block_index);
    unsigned int set = set_index(block_index, sets_cache);
    
    int index = find_index(tag, set);
    int time = 1;
    bool hit;
    
    if (index >= 0) {
        cache_block block = data[set][index];
        data[set].erase(data[set].begin() + index);
        data[set].push_back(block);
        //print_word();
        hit = true;
    } else {
        
        bool flushed;
        
        if (data[set].size() == blocks_set) {
            flushed = flush_block(0, set);
            data[set].erase(data[set].begin());
            block_from_memory(block_address, set, tag);
            time += cycles_write;
        } else {
            cout << "# requesting blocks from set" << endl; 
            block_from_memory(block_address, set, tag);
        }
        
        //Optimisation for write misses
        if (words_block > 1) {
            time += cycles_read;
        }
        
        hit = false;
    }
    
    //writing data to cache
    unsigned int writeblock = data[set].size() - 1;
    unsigned int reladdress = byte_address - block_address;
    for (int i = 0; i < bytes_word; i++) {
        data[set][writeblock].bytes[reladdress + i] = newdata[i];
    }
    
    data[set][writeblock].dirty = true;
    
    cout << "write-ack " << set;
    if (hit) {
        cout << " hit ";
    } else {
        cout << " miss ";
    }
    
    cout << time;
    cout << endl;
    print_cache_state();
}

void cache::flush_request() {
    unsigned int flushtime = 0;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (flush_block(j, i)) {
                flushtime += cycles_write;
            }
        }
    }
    cout << "flush-ack " << flushtime << endl;
}

cache ca;