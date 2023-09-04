#ifndef MAZE_H
#define MAZE_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

#include "parse_cmd.h"

class maze {
public:
	maze(Options opt);
	~maze();
	void display();
	void solve();
	void check_ch(char ch);
	void input(int cur_row, string str);
private:
	uint32_t num_colors;
	uint32_t height;
	uint32_t width;
	vector<vector<char>> puzzle;
	Options option;
	bool has_start;
	bool has_target;
};

#endif // !MAZE_H

