// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include "zoo.h"

Zoo::Zoo(int argc, char* argv[]) {
	get_mode(argc, argv, this->option);
	get_input();
	fast_path.resize(size + 1);
	opt_best_path.resize(size);
	opt_working_path.resize(size);
	fast_cost = 0.0;
	opt_cur_cost = 0.0;
	opt_best_cost = 0.0;
}

void Zoo::get_input() {
	cin >> size;
	this->master_list.reserve(size);
	int x, y;
	string junk;
	while (cin >> x >> y) {
		Point pt(x, y);
		master_list.push_back(pt);
		getline(cin, junk);
	}
}

void Zoo::solve_MST() {
	// use for-loop Prim's Algo
	// initialize everything
	vector<bool> visited(size, false);
	vector<double> squared_dist_list;
	squared_dist_list.resize(size);
	vector<int> parents(size, -1);
	squared_dist_list[0] = 0;
	for (uint32_t i = 1; i < squared_dist_list.size(); i++) {
		squared_dist_list[i] = numeric_limits<double>::infinity();
	}
	// start the algorithm
	for (uint32_t _ = 0; _ < squared_dist_list.size(); _++) { // loop V times
		// find the smallest k_v from unvisited
		int index = 0;
		double dist = numeric_limits<double>::infinity();
		for (uint32_t i = 0; i < master_list.size(); i++) {
			if (visited[i] == true) { // visited
				continue;
			}
			if (squared_dist_list[i] < dist) {
				index = i;
				dist = squared_dist_list[i];
			}
		}
		// set k_v to be true
		visited[index] = true;
		// update parent list and dist list
		for (uint32_t i = 1; i < master_list.size(); i++) {
			if (!connected(master_list[i], master_list[index]) || visited[i] == true) {
				continue;
			}
			double dist = calc_squared_dist(master_list[i], master_list[index]);
			if (dist < squared_dist_list[i]) {
				squared_dist_list[i] = dist;
				parents[i] = index;
			}
		}
	}
	
	// output
	double weight = 0;
	for (uint32_t i = 0; i < squared_dist_list.size(); i++) {
		weight += sqrt(squared_dist_list[i]);
	}
	cout << weight << '\n';
	for (uint32_t i = 1; i < parents.size(); i++) {
		cout << min(static_cast<int>(i), parents[i]) << " " << max(static_cast<int>(i), parents[i]) << '\n';
	}
}

void Zoo::solve_FASTTSP() {
	// Using insertion heuristic
	fast_path.clear();
	fast_path.reserve((unsigned long long)this->size + 1);
	fast_path.push_back(0);
	fast_path.push_back(0);
	fast_path.push_back(0);
	fast_path.push_back(0);
	fast_path[1] = 1;
	fast_path[2] = 2;
	fast_cost += calc_dist(master_list[0], master_list[1]);
	fast_cost += calc_dist(master_list[2], master_list[1]);
	fast_cost += calc_dist(master_list[0], master_list[2]);

	for (uint32_t i = 3; i < (unsigned long long)master_list.size(); i++) { // find the correct place to insert each node
		double cur_smallest = numeric_limits<double>::infinity();
		uint32_t insert_pos = 1;
		for (uint32_t j = 1; j < (unsigned long long)fast_path.size(); j++) {
			double tent_smallest = calc_dist(master_list[i], master_list[fast_path[j - 1]])
				+ calc_dist(master_list[i], master_list[fast_path[j]])
				- calc_dist(master_list[fast_path[j]], master_list[fast_path[j - 1]]);
			if (tent_smallest < cur_smallest) {
				cur_smallest = tent_smallest;
				insert_pos = j;
			}
		}
		fast_cost += cur_smallest;
		fast_path.insert(fast_path.begin() + insert_pos, i);
	}

	if (this->option.mode == Mode::FASTTSP) {
		cout << fast_cost << '\n';
		for (int i = 0; i < size; i++) {
			cout << fast_path[i] << " ";
		}
		cout << "\n";
	}
}


bool Zoo::promising(size_t permLength) {
	size_t num_unvisited = opt_working_path.size() - permLength;
	if (num_unvisited < 5) {
		return true;
	}
	// calculate the MST constructed by the remaining vertices
	vector<bool> visited(num_unvisited, false);
	vector<double> squared_dist_list;
	squared_dist_list.resize(num_unvisited);
	vector<int> parents(num_unvisited, -1);
	squared_dist_list[0] = 0;
	for (uint32_t i = 1; i < squared_dist_list.size(); i++) {
		squared_dist_list[i] = numeric_limits<double>::infinity();
	}
	// start the algorithm
	for (uint32_t _ = 0; _ < squared_dist_list.size(); _++) { // loop V times
		// find the smallest k_v from unvisited
		int index = 0;
		double dist = numeric_limits<double>::infinity();
		for (uint32_t i = 0; i < num_unvisited; i++) {
			if (visited[i] == true) { // visited
				continue;
			}
			if (squared_dist_list[i] < dist) {
				index = i;
				dist = squared_dist_list[i];
			}
		}
		// set k_v to be true
		visited[index] = true;
		// update parent list and dist list
		for (uint32_t i = 1; i < num_unvisited; i++) {
			if (visited[i] == true) {
				continue;
			}
			double dist = calc_squared_dist(master_list[opt_working_path[i + permLength]],
				master_list[opt_working_path[index + permLength]]);
			if (dist < squared_dist_list[i]) {
				squared_dist_list[i] = dist;
			}
		}
	}

	// output
	double MST_weight = 0;
	for (uint32_t i = 0; i < squared_dist_list.size(); i++) {
		MST_weight += sqrt(squared_dist_list[i]);
	}
	// end of MST, start to calc arms weight
	Point pt_a = master_list[0];
	double weight_a = numeric_limits<double>::infinity();
	for (size_t i = permLength; i < opt_working_path.size(); i++) {
		double temp = calc_dist(pt_a, master_list[opt_working_path[i]]);
		if (temp < weight_a) {
			weight_a = temp;
		}
	}
	Point pt_b = master_list[opt_working_path[permLength - 1]];
	double weight_b = numeric_limits<double>::infinity();
	for (size_t i = permLength; i < opt_working_path.size(); i++) {

		double temp = calc_dist(pt_b, master_list[opt_working_path[i]]);

		if (temp < weight_b) {
			weight_b = temp;
		}
	}
	double arms_weight = weight_a + weight_b;
	bool promise = arms_weight + opt_cur_cost + MST_weight < opt_best_cost;

	/*for (size_t i = 0; i < opt_working_path.size(); ++i)
		cerr << setw(2) << opt_working_path[i] << ' ';
	cerr << setw(4) << permLength << setw(10) << opt_cur_cost;
	cerr << setw(10) << weight_a << setw(10) << weight_b;
	cerr << setw(10) << MST_weight << setw(10) << arms_weight + opt_cur_cost + MST_weight << "  " << promise << '\n';*/

	return promise;
}

void Zoo::genPerms(size_t permLength) {
	if (permLength == (unsigned long long)opt_working_path.size()) {
		double closing_edge_weight = calc_dist(master_list[opt_working_path[permLength - 1]],
			master_list[opt_working_path[0]]);
		opt_cur_cost += closing_edge_weight;
		if (opt_cur_cost < opt_best_cost) { // update things
			opt_best_path = opt_working_path;
			opt_best_cost = opt_cur_cost;
		}
		opt_cur_cost -= closing_edge_weight;
		return;
	}  // if ..complete path

	if (!promising(permLength)) {
		return;
	}  // if ..not promising

	for (size_t i = permLength; i < (unsigned long long)opt_working_path.size(); ++i) {
		swap(opt_working_path[permLength], opt_working_path[i]);
		// curCost += (cost of new edge)
		double edge_weight = calc_dist(master_list[opt_working_path[permLength - 1]],
			master_list[opt_working_path[permLength]]);
		opt_cur_cost += edge_weight;
		genPerms(permLength + 1);
		// curCost -= (cost of new edge)
		opt_cur_cost -= edge_weight;
		swap(opt_working_path[permLength], opt_working_path[i]);
	}  // for ..unpermuted elements
}  // genPerms()

void Zoo::solve_OPTTSP() {
	solve_FASTTSP();
	this->opt_best_path = fast_path;
	opt_best_path.pop_back();
	this->opt_best_cost = fast_cost;
	if (this->size < 15) { // modify the threshold
		this->opt_working_path.resize(opt_best_path.size());
		for (uint32_t i = 0; i < (unsigned long long)opt_working_path.size(); i++) {
			opt_working_path[i] = i;
		}
	}
	else {
		this->opt_working_path = fast_path;
		this->opt_working_path.pop_back();
	}
	size_t perm_length = 1;
	genPerms(perm_length);
	cout << opt_best_cost << "\n";
	for (uint32_t i = 0; i < (unsigned long long)opt_best_path.size(); i++) {
		cout << opt_best_path[i] << " ";
	}
	cout << "\n";
}


void Zoo::init() {
	if (this->option.mode == Mode::MST) {
		solve_MST();
	}
	if (this->option.mode == Mode::FASTTSP) {
		solve_FASTTSP();
	}
	if (this->option.mode == Mode::OPTTSP) {
		solve_OPTTSP();
	}
}