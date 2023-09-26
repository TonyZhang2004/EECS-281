// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include "Battle.h"
#include "P2random.h"
#include <string>

Battle::Battle(int argc, char* argv[]) : cur_round(0),
	player_dead(false), killer(nullptr), last_zombie(nullptr) {
	get_mode(argc, argv, this->option);
	this->set_param();
	this->get_next_generating_round();
}

Battle::~Battle() {
	// delete master list, delete priority queue
	for (Zombie* zb : this->master_list) {
		delete zb;
	}
}

void Battle::set_param() {
	string junk;
	getline(cin, junk);
	cin >> junk >> this->quiver_capacity;
	getline(cin, junk);
	cin >> junk >> this->random_seed;
	getline(cin, junk);
	cin >> junk >> this->max_rand_distance;
	getline(cin, junk);
	cin >> junk >> this->max_rand_speed;
	getline(cin, junk);
	cin >> junk >> this->max_rand_health;
	getline(cin, junk);
	P2random::initialize(random_seed, max_rand_distance, max_rand_speed, max_rand_health);
}


void Battle::push_med_heap(Zombie* zb) {
	if (this->max_med_heap.empty() && this->min_med_heap.empty()) {
		min_med_heap.push(zb);
	}
	if (this->max_med_heap.size() > this->min_med_heap.size()) {
		Zombie* tmp = max_med_heap.top();
		max_med_heap.pop();
		if (zb->get_act_rounds() > tmp->get_act_rounds()) {
			min_med_heap.push(zb);
			max_med_heap.push(tmp);
		}
		else {
			min_med_heap.push(tmp);
			max_med_heap.push(zb);
		}
	}
	else {
		Zombie* tmp = min_med_heap.top();
		min_med_heap.pop();
		if (zb->get_act_rounds() > tmp->get_act_rounds()) {
			min_med_heap.push(zb);
			max_med_heap.push(tmp);
		}
		else {
			min_med_heap.push(tmp);
			max_med_heap.push(zb);
		}
	}
}

uint32_t Battle::get_median_life() {
	if (this->max_med_heap.size() > this->min_med_heap.size())
		return max_med_heap.top()->get_act_rounds();
	if (this->max_med_heap.size() < this->min_med_heap.size())
		return min_med_heap.top()->get_act_rounds();
	else
		return (max_med_heap.top()->get_act_rounds() + min_med_heap.top()->get_act_rounds()) / 2;
}

void Battle::get_next_generating_round() {
	string junk;
	getline(cin, junk);
	cin >> junk;
	cin >> this->next_generating_round;
	if (cin.fail()) {
		this->next_generating_round = 0;
		return;
	}
	getline(cin, junk);
}

void Battle::input_zombies() {
	string junk;
	int rand_zombies;
	int named_zombies;
	cin >> junk >> rand_zombies;
	getline(cin, junk);
	cin >> junk >> named_zombies;
	getline(cin, junk);
	for (int i = 0; i < rand_zombies; i++) {
		Zombie* zb = new Zombie();

		if (this->option.verbose) {
			cout << "Created: ";
			zb->display();
		}
		this->master_list.push_back(zb);
		this->pq.push(zb);
	}
	for (int i = 0; i < named_zombies; i++) {
		string name;
		uint32_t dist;
		uint32_t speed;
		uint32_t health;
		cin >> name >> junk >> dist >> junk >> speed >> junk >> health;
		getline(cin, junk);
		Zombie* zb = new Zombie(name, dist, speed, health);

		if (this->option.verbose) {
			cout << "Created: ";
			zb->display();
		}
		this->master_list.push_back(zb);
		this->pq.push(zb);
	}
}

void Battle::print_stats() {
	cout << "Zombies still active: " << this->pq.size() << '\n'
		<< "First zombies killed:\n";
	size_t cnt = 1;
	for (auto iter = killed_list.begin(); iter != killed_list.end(); iter++) {
		cout << (*iter)->get_name() << " " << cnt++ << '\n';
		if (cnt > this->option.N)
			break;
	}
	cout << "Last zombies killed:\n";
	for (auto iter = killed_list.rbegin(); iter != killed_list.rend(); iter++) {
		cout << (*iter)->get_name() << " " << --cnt << '\n';
		if (cnt < 1)
			break;
	}
	priority_queue<Zombie*, vector<Zombie*>, Comparator_Max_Life> tmp_max_heap;
	while (!this->min_stats_heap.empty()) {
		tmp_max_heap.push(min_stats_heap.top());
		min_stats_heap.pop();
	}
	cout << "Most active zombies:\n";
	while (!tmp_max_heap.empty()) {
		cout << tmp_max_heap.top()->get_name() << " " << tmp_max_heap.top()->get_act_rounds() << "\n";
		tmp_max_heap.pop();
	}

	priority_queue<Zombie*, vector<Zombie*>, Comparator_Min_Life> tmp_min_heap;
	while (!this->max_stats_heap.empty()) {
		tmp_min_heap.push(max_stats_heap.top());
		max_stats_heap.pop();
	}
	cout << "Least active zombies:\n";
	while (!tmp_min_heap.empty()) {
		cout << tmp_min_heap.top()->get_name() << " " << tmp_min_heap.top()->get_act_rounds() << "\n";
		tmp_min_heap.pop();
	}
}

void Battle::print_end_message() {
	if (this->player_dead) {
		cout << "DEFEAT IN ROUND " << this->cur_round << "! "
			<< this->killer->get_name() << " ate your brains!\n";
	}
	else {
		cout << "VICTORY IN ROUND " << this->cur_round << "! "
			<< this->last_zombie->get_name() << " was the last zombie.\n";
	}
}

void Battle::player_shoot_zombies() {
	while (this->quiver != 0 && !pq.empty()) {
		// shoot the zombie on the top of pq
		pq.top()->being_shot();
		this->quiver--;
		if (pq.top()->get_health() == 0) {
			if (this->option.verbose) {
				cout << "Destroyed: ";
				pq.top()->display();
			}
			if (this->option.median) {
				this->push_med_heap(pq.top());
			}
			if (this->option.statistics) {
				this->push_stats_heap(pq.top());
			}
			if (this->pq.size() == 1 && this->next_generating_round == 0)
				this->last_zombie = pq.top();
			this->killed_list.push_back(pq.top());
			pq.pop();
		}
	}
}

void Battle::push_stats_heap(Zombie* zb) {
	if (min_stats_heap.empty() || max_stats_heap.empty()) {
		min_stats_heap.push(zb);
		max_stats_heap.push(zb);
		return;
	}
	if (min_stats_heap.top()->get_act_rounds() <= zb->get_act_rounds()) {
		if (min_stats_heap.size() > this->option.N)
			min_stats_heap.pop();
		min_stats_heap.push(zb);
	}

	if (max_stats_heap.top()->get_act_rounds() >= zb->get_act_rounds()) {
		if (max_stats_heap.size() > this->option.N)
			max_stats_heap.pop();
		max_stats_heap.push(zb);
	}
}

void Battle::init() {
	while (!player_dead) {

		this->cur_round++;
		if (this->option.verbose)
			cout << "Round: " << this->cur_round << "\n";
		// update quiver
		this->quiver = this->quiver_capacity;

		// update zombies
		for (Zombie* zb : master_list) {
			if (zb->get_health() == 0)
				continue;

			zb->update();
			if (zb->get_dist() == 0 && !this->player_dead) {
				this->player_dead = true;
				this->killer = zb;
			}

			if (this->option.verbose) {
				cout << "Moved: ";
				zb->display();
			}
		}

		// check if the player is dead
		if (this->player_dead)
			break;

		// new zombies appearing
		if (this->cur_round == this->next_generating_round) {
			this->input_zombies();
			this->get_next_generating_round();
		}

		this->player_shoot_zombies();

		if (this->option.median) {
			cout << "At the end of round " << this->cur_round
				<< ", the median zombie lifetime is " << this->get_median_life() << "\n";
		}

		// check whether to break the loop
		if (pq.empty() && this->next_generating_round == 0)
			break;
	}

	this->print_end_message();

	if (this->option.statistics)
		this->print_stats();
}

