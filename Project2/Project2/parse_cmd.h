// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#ifndef PARSE_CMD_H
#define PARSE_CMD_H

#include <iostream>
#include <getopt.h>

using namespace std;

struct Options {
    bool verbose = false;
    bool statistics = false;
    bool median = false;
    size_t N = 0;
};

void print_help(char* argv[]);
void get_mode(int argc, char* argv[], Options& options);

#endif // !PARSE_CMD_H


