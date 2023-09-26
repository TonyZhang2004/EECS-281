// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include "Zombie.h"
#include "P2random.h"

Zombie::Zombie(string name, uint32_t dist, uint32_t speed,
	uint32_t health) : name(name), dist(dist), speed(speed), 
	health(health) {
	this->ETA = dist / speed;
	this->act_rounds = 1;
	this->active = true;
}

Zombie::Zombie() {
	this->name = P2random::getNextZombieName();
	this->dist = P2random::getNextZombieDistance();
	this->speed = P2random::getNextZombieSpeed();
	this->health = P2random::getNextZombieHealth();
	this->ETA = dist / speed;
	this->act_rounds = 1;
	this->active = true;
}


Zombie::~Zombie() {}

void Zombie::update() {
	this->dist -= min(this->speed, this->dist);
	this->ETA = this->dist / this->speed;
	this->act_rounds++;
}

void Zombie::display() {
	cout << this->name << " (distance: " << this->dist 
		<< ", speed: " << this->speed << ", health: " << this->health << ")\n";
}

void Zombie::being_shot() {
	this->health--;
}

void Zombie::set_dead() {
	this->active = false;
}

uint32_t Zombie::get_ETA() {
	return this->ETA;
}

uint32_t Zombie::get_health() {
	return this->health;
}

uint32_t Zombie::get_dist() {
	return this->dist;
}

string Zombie::get_name() {
	return this->name;
}

uint32_t Zombie::get_act_rounds() {
	return this->act_rounds;
}

bool Zombie::get_active() {
	return this->active;
}