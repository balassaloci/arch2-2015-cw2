#include "mem_sim_parser.h"

using namespace std;

void log_error (int line, string message) {
    cout << "ERROR on line " << line << ": " << message << endl;
}

void log_verbose (int line, string message) {
    //This message has been disabled intentionally
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
                    return -1;
                }
                
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of read-req, expected: 'read-req address'");
                    return -1;
                }
                
                log_verbose(linec, "read req, addr: " + to_string(address));
                ca.read(address);
                
            } else if (command == "write-req") {
                int address;
                string rawdata;
                unsigned char * parsedData;        
                
                string spare = " ";
                
                if (! (iss >> address)) {
                    log_error(linec, "invalid usage of write-req, expected: 'write-req address data'");
                    return -1;
                }
                
                if (! (iss >> rawdata)) {
                    log_error(linec, "invalid usage of write-req, expected: 'write-req address data'");
                    return -1;
                }
                
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of write-req, expected: 'write-req address data'");
                    return -1;
                }
                
                parsedData = parseData(wordsize, rawdata);                
                log_verbose(linec, "write req");
                
                ca.write(address, parsedData);
           
            } else if (command == "flush-req") {
                string spare = " ";
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of flush-req, expected: 'read-req'");
                    return -1;
                }
                ca.flush_request();

                log_verbose(linec, "flush req");
                
            } else if (command == "debug-req") {
                string spare = " ";
                if (iss >> spare && spare[0] != '#') {
                    log_error(linec, "invalid usage of flush-req, expected: 'read-req'");
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
                return -1;	
            }
            
        } else {
            log_verbose(linec, "line is empty");
        }

    }

}


int runParse() {
    /*
    ca.address_bits = 8;
    ca.bytes_word = 2;     // Power of 2
    ca.words_block = 2;    // Power of 2
    ca.blocks_set = 1;     // Power of 2
    ca.sets_cache = 2;     // Power of 2
    ca.cycles_hit = 1;
    ca.cycles_read = 2;
    ca.cycles_write = 2;
    
    */    
    ca.create();
      
    parseInfile();
 
    return 0;
}
