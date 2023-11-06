// 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98

#include "parse_cmd.h"

void print_help() {
    cout << "Help Msg\n";
}

void get_mode(int argc, char* argv[], Options& options) {
    opterr = false;
    int choice;
    int index = 0;
    option long_options[] = {
        { "file", required_argument, nullptr, 'f'},
        { "help", no_argument, nullptr, 'h'},
        { "verbose", no_argument, nullptr, 'v'},
        { nullptr, 0, nullptr, '\0' },
    };

    while ((choice = getopt_long(argc, argv, "f:hv", long_options, &index)) != -1) {
        switch (choice) {
            case 'h': {
                print_help();
                exit(0);
            }

            case 'f': {
                string arg{ optarg };
                options.filename = arg;
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