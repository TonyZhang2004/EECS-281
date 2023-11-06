// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#ifndef PARSE_CMD_H
#define PARSE_CMD_H

#include <iostream>
#include <getopt.h>

using namespace std;

struct Options {
    bool verbose = false;
    string filename = "";
};

void print_help();
void get_mode(int argc, char* argv[], Options& options);

#endif // !PARSE_CMD_H

