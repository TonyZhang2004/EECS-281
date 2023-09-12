// Project Identifier : A8A3A33EF075ACEF9B08F5B9845569ECCB423725
#ifndef MAZE_H
#define MAZE_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <deque>

#include "parse_cmd.h"
#include "state.h"

class maze {
public:
	explicit maze(Options opt);
	~maze();
	void display_puzzle();
	void solve();
	void check_ch(char ch);
	void input(uint32_t cur_row, const string& str);
	bool discover_and_investigate(char cur_ch, uint32_t cur_row, uint32_t cur_col);
	bool valid_to_discover(char to_be_check, char cur_color, bool& solved);
	void output();
	void navigate_route();
	void display_list();
private:
	uint32_t num_colors;
	uint32_t height;
	uint32_t width;
	uint32_t target_row = 0;
	uint32_t target_col = 0;
	State cur_state;
	vector<vector<char>> puzzle;
	/*remember to - 'a' + 1 if accessing the 1st dim of backtrace!
	'$' means undiscovered!
	'%' means discovered!
	0 in the 1st dim is color '^'*/
	vector<vector<vector<char>>> backtrace; 
	Options option;
	bool has_start;
	bool has_target;
	bool has_solution;
	deque<State> search_container;
	vector<State> route;
	vector<vector<vector<char>>> map;
};

#endif // !MAZE_H

