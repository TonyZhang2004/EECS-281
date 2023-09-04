#include "maze.h"

using namespace std;

maze::maze(Options opt) : has_start(false), has_target(false) {
	this->option = opt;

	cin >> this->num_colors >> this->height >> this->width;
	if (this->num_colors > 26) {
		cerr << "Error: Invalid numColor\n";
		exit(1);
	}
	if (this->height < 1) {
		cerr << "Error: Invalid height\n";
		exit(1);
	}
	if (this->width < 1) {
		cerr << "Error: Invalid width\n";
		exit(1);
	}
	this->puzzle.resize(this->height, vector<char>(this->width, '0'));

	string str;
	getline(cin, str);
	uint32_t row = 0;
	while (getline(cin, str)) {
		if (!(str[0] == '/' && str[1] == '/')) {
			input(row, str);
			row++;
		}
	}
}

maze::~maze() {}

void maze::input(int cur_row, string str) {
	for (uint32_t i = 0; i < this->width; i++) {
		check_ch(str[i]);
		if (str[i] == '@') {
			if (this->has_start) {
				cerr << "Error: Puzzle must have exactly one start and one target\n";
				exit(1);
			}
			this->has_start = true;
		}
		if (str[i] == '?') {
			if (this->has_target) {
				cerr << "Error: Puzzle must have exactly one start and one target\n";
				exit(1);
			}
			this->has_target = true;
		}
		this->puzzle[cur_row][i] = str[i];
	}
}

void maze::check_ch(char ch) {
	if (ch != '@' && ch != '?' && ch != '.' && ch != '#' && ch != '^' &&
		(ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z')) {
		cerr << "Error: Invalid character in map\n";
		exit(1);
	}
	if (ch >= 'a' && ch <= 'z') {
		if (ch > char('a' + this->num_colors)) {
			cerr << "Error: Invalid button in map\n";
			exit(1);
		}
	}
	if (ch >= 'A' && ch <= 'Z') {
		if (ch > char('A' + this->num_colors)) {
			cerr << "Error: Invalid door in map\n";
			exit(1);
		}
	}
}

void maze::display() {
	for (uint32_t i = 0; i < this->height; i++) {
		for (uint32_t j = 0; j < this->width; j++) {
			cout << this->puzzle[i][j];
		}
		cout << '\n';
	}
}

