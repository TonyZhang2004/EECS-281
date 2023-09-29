// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#include "Comparators.h"

bool Comparator_ETA::operator()(Zombie* a, Zombie* b) {
	if (a->get_ETA() > b->get_ETA())
		return true;
	if (a->get_ETA() < b->get_ETA())
		return false;
	if (a->get_health() > b->get_health())
		return true;
	if (a->get_health() < b->get_health())
		return false;
	if (a->get_name() > b->get_name())
		return true;
	else
		return false;
}

bool Comparator_Max_Life::operator()( Zombie* const a, Zombie* const b) {
	if (a->get_act_rounds() < b->get_act_rounds())
		return true;
	if (a->get_act_rounds() > b->get_act_rounds())
		return false;
	if (a->get_name() > b->get_name())
		return true;
	else
		return false;
}

bool Comparator_Min_Life::operator()(Zombie* a, Zombie* b) {
	if (a->get_act_rounds() > b->get_act_rounds())
		return true;
	if (a->get_act_rounds() < b->get_act_rounds())
		return false;
	if (a->get_name() > b->get_name())
		return true;
	else
		return false;
}

bool Comparator_Min_Life_Reverse_Name::operator()(Zombie* const a, Zombie* const b) {
	if (a->get_act_rounds() > b->get_act_rounds())
		return true;
	if (a->get_act_rounds() < b->get_act_rounds())
		return false;
	if (a->get_name() < b->get_name())
		return true;
	else
		return false;
}
