#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "mem_sim_cache.cpp"

using namespace std;

cache c;

void log_error (int line, string message) {
    cout << "ERROR on line " << line << ": " << message << endl;
}

void log_verbose (int line, string message) {
    //cout << "# Msg on line " << line << ": " << message << endl;
}

unsigned char * parseData(int wordsize, string rawdata) {
    string temp;
    unsigned int byte;
    unsigned char * bytes = new unsigned char[wordsize];

    for (int i = 0; i <= wordsize / 2; i ++) {
        stringstream ss;
        temp = rawdata.substr(i * 2, 2);
        ss << hex << temp;
        ss >> byte;
        bytes[i] = byte;
    }
    return bytes;
}

int parseInfile() {
    //ifstream infile(inPath.c_str());
    string line;
    int linec = 0;
    int wordsize = 2;
   
    while (getline(cin, line))
    {
        istringstream iss(line);
        string command;
        linec++;

        if (iss >> command) {
            if (command == "read-req") {
                int address;
                string spare = " ";
                
                if (! (iss >> address)) {
                    log_error(linec, "invalid usage of read-req, expected: 'read-req address'");
                    //infile.close();
                    return -1;
                }
                
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of read-req, expected: 'read-req address'");
                    //infile.close();
                    return -1;
                }
                
                log_verbose(linec, "read req, addr: " + to_string(address));
                c.read(address);
                // Trigger read request
                
            } else if (command == "write-req") {
                int address;
                string rawdata;
                unsigned char * parsedData;        
                
                string spare = " ";
                
                if (! (iss >> address)) {
                    log_error(linec, "invalid usage of write-req, expected: 'write-req address data'");
                    //infile.close();
                    return -1;
                }
                
                if (! (iss >> rawdata)) {
                    log_error(linec, "invalid usage of write-req, expected: 'write-req address data'");
                    //infile.close();
                    return -1;
                }
                
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of write-req, expected: 'write-req address data'");
                    //infile.close();
                    return -1;
                }
                
                parsedData = parseData(wordsize, rawdata);                
                log_verbose(linec, "write req");
                
                c.write(address, parsedData);
                //log_verbose(linec, "read req, addr: " + to_string(address));
                // Trigger WRITE request

            
            } else if (command == "flush-req") {
                string spare = " ";
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of flush-req, expected: 'read-req'");
                    //infile.close();
                    return -1;
                }
                c.flush_request();
                
                // cout << "flush-ack 0" << endl;
                // Trigger flush request
                log_verbose(linec, "flush req");
                
            } else if (command == "debug-req") {
                string spare = " ";
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of flush-req, expected: 'read-req'");
                    //infile.close();
                    return -1;
                }
                
                // Trigger DEBUG request
                cout << "debug-ack-begin" << endl
                        << "debug-ack-end" << endl;
                log_verbose(linec, "debug req");
            } else if (command[0] == '#') {
                log_verbose(linec, "comment line");
            } else {
                log_error(linec, "INVALID instruction: " + command);
                //infile.close();
                return -1;	
            }
            
        } else {
            log_verbose(linec, "line is empty");
        }

    }

    //infile.close();
}


int runParse() {
    /*
    c.address_bits = 8;
    c.bytes_word = 2;     // Power of 2
    c.words_block = 2;    // Power of 2
    c.blocks_set = 1;     // Power of 2
    c.sets_cache = 2;     // Power of 2
    c.cycles_hit = 1;
    c.cycles_read = 2;
    c.cycles_write = 2;
    
    */    
    c.create();
      
    parseInfile();
 
    return 0;
}