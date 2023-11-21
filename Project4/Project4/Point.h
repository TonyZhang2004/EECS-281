// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#ifndef POINT
#define POINT

enum Point_Type {
	Wild, Safe, Wall
};

class Point {
public:
	Point(double _x, double _y);
	double get_x() { return coord_x; }
	double get_y() { return coord_y; }
	Point_Type get_type() { return type; }
private:
	double coord_x;
	double coord_y;
	Point_Type type;
};

#endif // !POINT

