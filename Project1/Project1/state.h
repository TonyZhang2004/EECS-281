// Project Identifier : A8A3A33EF075ACEF9B08F5B9845569ECCB423725
#ifndef STATE_H
#define STATE_H

#include <iostream>

using namespace std;

class State {
public:
	State() : color('^'), row(0), col(0) {};
	State(char color, uint32_t row, uint32_t col) {
		this->color = color;
		this->row = row;
		this->col = col;
	}
	void print();
	uint32_t get_row();
	uint32_t get_col();
	char get_color();
private:
	char color;
	uint32_t row;
	uint32_t col;
};

#endif // !STATE_H

