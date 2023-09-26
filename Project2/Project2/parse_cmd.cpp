// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include "parse_cmd.h"

void print_help(char* argv[]) {
    cout << "Usage: " << argv[0] << " [-s <num>] | -v | -m | -h]\n";
}

void get_mode(int argc, char* argv[], Options& options) {
    opterr = false;
    int choice;
    int index = 0;
    option long_options[] = {
        { "median", no_argument, nullptr, 'm'},
        { "statistics", required_argument, nullptr, 's'},
        { "help", no_argument, nullptr, 'h'},
        { "verbose", no_argument, nullptr, 'v'},
        { nullptr, 0, nullptr, '\0' },
    };

    while ((choice = getopt_long(argc, argv, "s:mhv", long_options, &index)) != -1) {
        switch (choice) {
            case 'h': {
                print_help(argv);
                exit(0);
            }

            case 's': {
                string arg{ optarg };
                options.statistics = true;
                for (char c : arg) {
                    if (c >= '0' && c <= '9') {
                        options.N = options.N * 10 + c - '0';
                    }
                }
                break;
            }

            case 'm': {
                options.median = true;
                break;
            }

            case 'v': {
                options.verbose = true;
                break;
            }

            default: {
                cerr << "Error: Unknown option" << endl;
                exit(1);
            }
        }
    }

}