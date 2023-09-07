// Project Identifier : A8A3A33EF075ACEF9B08F5B9845569ECCB423725
// This is Project 1 puzzle solver in UMich EECS 281.
// This implementation is owned by Guanyu Zhang.
#include <iostream>

#include "parse_cmd.h"
#include "maze.h"

using namespace std;

int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);
    Options options;
    getMode(argc, argv, options);
    maze maze(options);
    maze.solve();
    maze.output();
}