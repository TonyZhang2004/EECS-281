// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#ifndef PARSE_CMD_H
#define PARSE_CMD_H

#include <iostream>
#include <getopt.h>

using namespace std;

enum Mode {
    MST, FASTTSP, OPTTSP
};

struct Options {
    Mode mode = MST;
};

void print_help();
void get_mode(int argc, char* argv[], Options& options);

#endif // !PARSE_CMD_H

