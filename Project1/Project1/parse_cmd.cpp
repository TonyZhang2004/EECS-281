// Project Identifier : A8A3A33EF075ACEF9B08F5B9845569ECCB423725
#include "parse_cmd.h"

void printHelp(char* argv[]) {
    cout << "Usage: " << argv[0] << " [-o map|list] | -q | -s | -h\n";
    cout << "exactly one of stack or queue must be specified, exactly once\n";
}

void getMode(int argc, char* argv[], Options& options) {
    opterr = false;
    int choice;
    int index = 0;
    option long_options[] = {
        { "output", required_argument, nullptr, 'o'},
        { "queue", no_argument, nullptr, 'q'},
        { "help", no_argument, nullptr, 'h'},
        { "stack", no_argument, nullptr, 's'},
        { nullptr, 0, nullptr, '\0' },
    };

    while ((choice = getopt_long(argc, argv, "o:qsh", long_options, &index)) != -1) {
        switch (choice) {
            case 'h': {
                printHelp(argv);
                exit(0);
            }

            case 'o': {
                string arg{ optarg };
                if (arg != "map" && arg != "list") {
                    cerr << "Error: Invalid output mode" << endl;
                    exit(1);
                }

                if (options.out_opt != OutMode::kNone) {
                    cerr << "Both map and list output modes specified" << endl;
                }

                if (arg == "map") {
                    options.out_opt = OutMode::kMap;
                }
                else {
                    options.out_opt = OutMode::kList;
                }
                break;
            }

            case 'q': {
                if (options.ds_opt != DataStruct::kNone) {
                    cerr << "Error : Both stack and queue modes specified" << endl;
                    exit(1);
                }
                else {
                    options.ds_opt = DataStruct::kQueue;
                }
                break;
            }

            case 's': {
                if (options.ds_opt != DataStruct::kNone) {
                    cerr << "Error: Can not have both stack and queue" << endl;
                    exit(1);
                }
                else {
                    options.ds_opt = DataStruct::kStack;
                }
                break;
            }

            default: {
                cerr << "Error: Unknown option" << endl;
                exit(1);
            }
        }
    }

    if (options.out_opt == OutMode::kNone) {
        options.out_opt = OutMode::kMap;
    }

    if (options.ds_opt == DataStruct::kNone) {
        cerr << "Error: Must have at least one of stack or queue" << endl;
        exit(1);
    }
}