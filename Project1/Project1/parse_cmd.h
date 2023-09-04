// Project Identifier : A8A3A33EF075ACEF9B08F5B9845569ECCB423725
#ifndef PARSE_CMD_H
#define PARSE_CMD_H

#include <iostream>
#include <iomanip>
#include <getopt.h>

using namespace std;

enum class DataStruct {
    kNone = 0,
    kQueue,
    kStack,
};

enum class OutMode {
    kNone = 0,
    kMap,
    kList,
};

struct Options {
    DataStruct ds_opt = DataStruct::kNone;
    OutMode out_opt = OutMode::kNone;
};

void printHelp(char* argv[]);
void getMode(int argc, char* argv[], Options& options);

#endif // !PARSE_CMD_H


