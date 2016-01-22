#ifndef MEM_SIM_PARSER
#define MEM_SIM_PARSER

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "mem_sim_cache.h"

#ifndef cachedef
#define cachedef
    //cache c;
#endif
//int ilona;


void log_error (int line, string message);

void log_verbose (int line, string message);

unsigned char * parseData(int wordsize, string rawdata);

int parseInfile();

int runParse();

#endif