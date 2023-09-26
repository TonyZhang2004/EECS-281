// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <iostream>

using namespace std;

class Zombie {
public:
	Zombie(string name, uint32_t dist, uint32_t speed, uint32_t health);
	Zombie();
	~Zombie();

	void display();
	void update();
	void being_shot();
	uint32_t get_ETA();
	uint32_t get_health();
	uint32_t get_dist();
	string get_name();
	uint32_t get_act_rounds();
	bool get_active();
	void set_dead();
private:
	bool active;
	string name;
	uint32_t dist;
	uint32_t speed;
	uint32_t health;
	uint32_t act_rounds;
	uint32_t ETA;
};

#endif // !ZOMBIE_H

