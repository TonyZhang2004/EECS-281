// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "Point.h"

Point::Point(double _x, double _y) : coord_x(_x), coord_y(_y) {
	if ((_x == 0 && _y <= 0) || (_x <= 0 && _y == 0)) {
		type = Wall;
	}
	else if (_x < 0 && _y < 0) {
		type = Wild;
	}
	else {
		type = Safe;
	}
}