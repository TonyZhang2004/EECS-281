// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#ifndef ZOO
#define ZOO

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <algorithm>

#include "parse_cmd.h"
#include "Point.h"

using namespace std;

class Zoo {
public:
	Zoo(int argc, char* argv[]);
	void init();
	void solve_MST();
	void solve_FASTTSP();
	void solve_OPTTSP();
	void get_input();
	bool connected(Point pt_a, Point pt_b) {
		return !((pt_a.get_type() == Wild && pt_b.get_type() == Safe)
			|| (pt_a.get_type() == Safe && pt_b.get_type() == Wild));
	};
	inline double calc_squared_dist(Point& pt_a, Point& pt_b) {
		return (pt_a.get_x() - pt_b.get_x()) * (pt_a.get_x() - pt_b.get_x())
			+ (pt_a.get_y() - pt_b.get_y()) * (pt_a.get_y() - pt_b.get_y());
	}
	inline double calc_dist(Point& pt_a, Point& pt_b) {
		return sqrt(calc_squared_dist(pt_a, pt_b));
	}
	void genPerms(size_t permLength);
	bool promising(size_t permLength);
private:
	vector<Point> master_list;
	vector<double> squared_dist_list;
	vector<int> fast_path;
	vector<int> opt_best_path;
	vector<int> opt_working_path;
	vector<bool> visited;
	Options option;
	double fast_cost;
	double opt_cur_cost;
	double opt_best_cost;
	int size;
};

#endif // !ZOO

