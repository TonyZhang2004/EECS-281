// Project Identifier : A8A3A33EF075ACEF9B08F5B9845569ECCB423725

#include "state.h"

void State::print() {
	cout << "(" << this->color << ", (" << this->row << ", " << this - col << "))\n";
}

uint32_t State::get_col() {
	return this->col;
}

uint32_t State::get_row() {
	return this->row;
}

char State::get_color() {
	return this->color;
}
