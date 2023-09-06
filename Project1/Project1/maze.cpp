// Project Identifier : A8A3A33EF075ACEF9B08F5B9845569ECCB423725
#include "maze.h"

using namespace std;

maze::maze(Options opt) : has_start(false), has_target(false) {
	this->option = opt;
	this->has_solution = false;

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
	this->backtrace.resize(this->num_colors + 1,
		vector<vector<char>>(this->height, vector<char>(this->width, '$'))); 


	string str;
	getline(cin, str);
	uint32_t row = 0;
	while (getline(cin, str)) {
		if (!(str[0] == '/' && str[1] == '/')) {
			input(row, str);
			row++;
		}
	}
	if (!this->has_start || !this->has_target) {
		cerr << "Error: Puzzle must have exactly one start and one target\n";
		exit(1);
	}
}

maze::~maze() {}

void maze::input(uint32_t cur_row, string str) {
	for (uint32_t i = 0; i < this->width; i++) {
		check_ch(str[i]);
		if (str[i] == '@') {
			if (this->has_start) {
				cerr << "Error: Puzzle must have exactly one start and one target\n";
				exit(1);
			}
			this->has_start = true;
			this->cur_state = State('0', 0, 0);
			this->search_container.push_back(State('^', cur_row, i));
			this->backtrace[0][cur_row][i] = '@';
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

void maze::display_puzzle() {
	for (uint32_t i = 0; i < this->height; i++) {
		for (uint32_t j = 0; j < this->width; j++) {
			cout << this->puzzle[i][j];
		}
		cout << '\n';
	}
}

void maze::display_backtrace() {
	for (uint32_t i = 0; i < this->num_colors + 1; i++) {
		if (i == 0) {
			cout << "// color ^\n";
		}
		else {
			cout << "// color " << static_cast<char>(i + 'a' - 1) << "\n";
		}
		
		for (uint32_t j = 0; j < this->height; j++) {
			for (uint32_t k = 0; k < this->width; k++) {
				cout << this->backtrace[i][j][k];
			}
			cout << "\n";
		}
		cout << "\n";
	}
}

bool maze::discover_and_investigate(char cur_ch, uint32_t cur_row, uint32_t cur_col) {
	char cur_color = this->cur_state.get_color(); // '^', 'a', 'b', 'c', etc.
	//assigning variables to be checked iff it's valid && not discovered
	uint32_t layer;
	if (cur_color == '^') {
		layer = 0;
	}
	else {
		layer = cur_color - 'a' + 1;
	}
	char north = '0', east = '0', south = '0', west = '0';
	if (cur_row != 0 && backtrace[layer][cur_row - 1][cur_col] == '$')
		north = this->puzzle[cur_row - 1][cur_col];
	if (cur_col != this->width - 1 && backtrace[layer][cur_row][cur_col + 1] == '$')
		east = this->puzzle[cur_row][cur_col + 1];
	if (cur_row != this->height - 1 && backtrace[layer][cur_row + 1][cur_col] == '$')
		south = this->puzzle[cur_row + 1][cur_col];
	if (cur_col != 0 && backtrace[layer][cur_row][cur_col - 1] == '$')
		west = this->puzzle[cur_row][cur_col - 1];
	
	if (cur_ch == '?') {  // successfully solved!
		return true;
	}

	if ((cur_ch >= 'a' && uint32_t(cur_ch) <= 'a' + this->num_colors - 1) || cur_ch == '^') { // step on a button
		uint32_t id;
		if (cur_ch == '^') {
			id = 0;
		}
		else {
			id = cur_ch - 'a' + 1;
		}

		if (this->backtrace[id][cur_row][cur_col] != '$') // discovered or not
		{
			if (valid_to_discover(north, cur_color)) {
				search_container.push_back(State(cur_color, cur_row - 1, cur_col));
				backtrace[layer][cur_row - 1][cur_col] = 'S';
			}
			if (valid_to_discover(east, cur_color)) {
				search_container.push_back(State(cur_color, cur_row, cur_col + 1));
				backtrace[layer][cur_row][cur_col + 1] = 'W';
			}
			if (valid_to_discover(south, cur_color)) {
				search_container.push_back(State(cur_color, cur_row + 1, cur_col));
				backtrace[layer][cur_row + 1][cur_col] = 'N';
			}
			if (valid_to_discover(west, cur_color)) {
				search_container.push_back(State(cur_color, cur_row, cur_col - 1));
				backtrace[layer][cur_row][cur_col - 1] = 'E';
			}
			return false;
		}
		// undiscovered
		backtrace[id][cur_row][cur_col] = cur_color;
		search_container.push_back(State(cur_ch, cur_row, cur_col));
	}

	if (cur_ch == '.' || cur_ch == '@' || (cur_ch >= 'A' && cur_ch <= 'Z')) { // feel free to discover & investigate
		if (valid_to_discover(north, cur_color)) {
			search_container.push_back(State(cur_color, cur_row - 1, cur_col));
			backtrace[layer][cur_row - 1][cur_col] = 'S';
		}
		if (valid_to_discover(east, cur_color)) {
			search_container.push_back(State(cur_color, cur_row, cur_col + 1));
			backtrace[layer][cur_row][cur_col + 1] = 'W';
		}
		if (valid_to_discover(south, cur_color)) {
			search_container.push_back(State(cur_color, cur_row + 1, cur_col));
			backtrace[layer][cur_row + 1][cur_col] = 'N';
		}
		if (valid_to_discover(west, cur_color)) {
			search_container.push_back(State(cur_color, cur_row, cur_col - 1));
			backtrace[layer][cur_row][cur_col - 1] = 'E';
		}
	}
	return false;
}

bool maze::valid_to_discover(char to_be_check, char cur_color) {
	return (to_be_check != '0' && to_be_check != '#') && (to_be_check == cur_color - 'a' + 'A' || to_be_check == '.'
		|| to_be_check == '^' || (to_be_check >= 'a' && to_be_check <= 'z') || to_be_check == '?' || to_be_check == '@');
}

void maze::solve() {
	if (this->option.ds_opt == DataStruct::kQueue) {
		while (!this->search_container.empty()) {
			// pop out the 1st element in the queue
			this->cur_state = this->search_container.front();
			this->search_container.pop_front();
			uint32_t cur_row = cur_state.get_row();
			uint32_t cur_col = cur_state.get_col();
			char cur_ch = this->puzzle[cur_row][cur_col];

			bool success = this->discover_and_investigate(cur_ch, cur_row, cur_col);
			if (success) {
				this->has_solution = true;
				return;
			}
		}
	}

	if (this->option.ds_opt == DataStruct::kStack) {
		while (!this->search_container.empty()) {
			// pop out the 1st element in the queue
			this->cur_state = this->search_container.back();
			this->search_container.pop_back();
			uint32_t cur_row = cur_state.get_row();
			uint32_t cur_col = cur_state.get_col();
			char cur_ch = this->puzzle[cur_row][cur_col];

			bool success = this->discover_and_investigate(cur_ch, cur_row, cur_col);
			if (success) {
				this->has_solution = true;
				return;
			}
		}
	}
}

State maze::get_cur_state() {
	return this->cur_state;
}

void maze::navigate_route() {
	uint32_t cur_row = cur_state.get_row();
	uint32_t cur_col = cur_state.get_col();
	while (puzzle[cur_row][cur_col] != '@') {
		cur_row = cur_state.get_row();
		cur_col = cur_state.get_col();
		uint32_t layer = 0;
		char cur_color = cur_state.get_color();
		if (cur_color == '^')
			layer = 0;
		if (cur_color >= 'a' && cur_color <= 'z')
			layer = cur_color - 'a' + 1;
		this->route.push_back(cur_state);
		char token = backtrace[layer][cur_row][cur_col];
		if (token == 'E')
			cur_state = State(cur_color, cur_row, cur_col + 1);
		if (token == 'S')
			cur_state = State(cur_color, cur_row + 1, cur_col);
		if (token == 'N')
			cur_state = State(cur_color, cur_row - 1, cur_col);
		if (token == 'W')
			cur_state = State(cur_color, cur_row, cur_col - 1);
		if (token == '^')
			cur_state = State('^', cur_row, cur_col);
		if (token >= 'a' && token <= 'z')
			cur_state = State(token, cur_row, cur_col);
	}
}

void maze::display_list() {
	for (auto iter = this->route.rbegin(); iter != this->route.rend(); iter++)
		cout << "(" << (*iter).get_color() << ", " << "("
		<< (*iter).get_row() << ", " << (*iter).get_col() << "))\n";
}

void maze::output() {
	this->navigate_route();
	if (this->option.out_opt == OutMode::kList && this->has_solution == true) {
		this->display_list();
	}

	if (this->option.out_opt == OutMode::kMap && this->has_solution == true) {
		this->map.resize(this->num_colors + 1, this->puzzle);

		while (!(this->route.size() == 1)) {
			char cur_color = this->route.back().get_color();
			int layer;
			if (cur_color == '^')
				layer = 0;
			else
				layer = cur_color - 'a' + 1;
			char cur_row = this->route.back().get_row();
			char cur_col = this->route.back().get_col();
			this->route.pop_back();
			if (puzzle[cur_row][cur_col] == '?')
				continue;
			if ((puzzle[cur_row][cur_col] >= 'a' && puzzle[cur_row][cur_col] <= 'z')
				|| puzzle[cur_row][cur_col] == '^') {
				if (puzzle[cur_row][cur_col] == cur_color)
					this->map[layer][cur_row][cur_col] = '@';
				else
					this->map[layer][cur_row][cur_col] = '%';
			}

			else {
				map[layer][cur_row][cur_col] = '+';
			}

		}

		for (uint32_t i = 0; i < this->num_colors + 1; i++) {
			char color;
			if (i == 0)
				color = '^';
			else
				color = 'a' + i - 1;
			cout << "// color " << color << "\n";
			for (uint32_t j = 0; j < this->height; j++) {
				for (uint32_t k = 0; k < this->width; k++) {
					if (map[i][j][k] == color && i == 0)
						map[i][j][k] = '.';
					if (i != 0 && (map[i][j][k] == color || map[i][j][k] == color - 'a' + 'A'))
						map[i][j][k] = '.';
					if (i != 0 && puzzle[j][k] == '@')
						map[i][j][k] = '.';
					if (i == 0 && puzzle[j][k] == '@')
						map[i][j][k] = '@';
					cout << map[i][j][k];
				}
				cout << "\n";
			}
		}
	}

	if (this->has_solution == false) {
		cout << "No solution.\nDiscovered:\n";
	}
}

