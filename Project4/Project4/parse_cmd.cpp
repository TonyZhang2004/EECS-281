// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include "parse_cmd.h"

void print_help() {
    cout << "Help Msg\n";
}

void get_mode(int argc, char* argv[], Options& options) {
    opterr = false;
    int choice;
    int index = 0;
    option long_options[] = {
        { "mode", required_argument, nullptr, 'm'},
        { "help", no_argument, nullptr, 'h'},
        { nullptr, 0, nullptr, '\0' },
    };

    while ((choice = getopt_long(argc, argv, "m:h", long_options, &index)) != -1) {
        switch (choice) {
            case 'h': {
                print_help();
                exit(0);
            }

            case 'm': {
                string arg{ optarg };
                if (arg != "MST" && arg != "FASTTSP" && arg != "OPTTSP") {
                    cerr << "Error  Unknown option parameter\n";
                    exit(1);
                }
                if (arg == "MST") {
                    options.mode = Mode::MST;
                }
                if (arg == "FASTTSP") {
                    options.mode = Mode::FASTTSP;
                }
                if (arg == "OPTTSP") {
                    options.mode = Mode::OPTTSP;
                }
                break;
            }

            default: {
                cerr << "Error: Unknown option" << endl;
                exit(1);
            }
        }
    }

}