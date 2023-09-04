//This is Project 1 puzzle solver in UMich EECS 281.
//This project is made by Guanyu Zhang.

#include <iostream>
#include <iomanip>

#include "parse_cmd.h"
#include "maze.h"

using namespace std;

int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);

    Options options;
    getMode(argc, argv, options);
    maze maze(options);
    maze.display();
    /*if (options.ds_opt == DataStruct::kStack)
        cout << "Data Structure Mode : Stack\n";
    else if (options.ds_opt == DataStruct::kQueue)
        cout << "Data Structure Mode : Queue\n";

    if (options.out_opt == OutMode::kMap)
        cout << "Output Mode : Map\n";
    else if (options.out_opt == OutMode::kList)
        cout << "Output Mode : List\n";*/
}