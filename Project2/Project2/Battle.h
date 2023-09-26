// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#ifndef BATTLE_H
#define BATTLE_H

#include <iostream>
#include <deque>
#include <queue>

#include "parse_cmd.h"
#include "Zombie.h"
#include "Comparators.h"

using namespace std;

class Battle {
public:
	Battle(int argc, char* argv[]);
	~Battle();
	void set_param();
	void init();
	void input_zombies();
	void get_next_generating_round();
	void push_med_heap(Zombie* zb);
	void print_end_message();
	void player_shoot_zombies();
	void print_stats();
	void push_stats_heap(Zombie* zb);
	uint32_t get_median_life();
private:
	uint32_t quiver_capacity;
	uint32_t quiver;
	uint32_t random_seed;
	uint32_t max_rand_distance;
	uint32_t max_rand_speed;
	uint32_t max_rand_health;
	uint32_t cur_round;
	uint32_t next_generating_round;
	Options option;
	size_t stats_num;
	bool player_dead;
	deque<Zombie*> master_list;
	vector<Zombie*> killed_list;
	priority_queue<Zombie*, deque<Zombie*>, Comparator_ETA> pq;
	priority_queue<Zombie*, vector<Zombie*>, Comparator_Max_Life> max_med_heap;
	priority_queue<Zombie*, vector<Zombie*>, Comparator_Min_Life> min_med_heap;
	priority_queue<Zombie*, vector<Zombie*>, Comparator_Max_Life> max_stats_heap;
	priority_queue<Zombie*, vector<Zombie*>, Comparator_Min_Life> min_stats_heap;
	Zombie* killer;
	Zombie* last_zombie;
};

#endif // !BATTLE_H

