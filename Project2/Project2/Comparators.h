// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "Zombie.h"

class Comparator_ETA {
public:
	bool operator()(Zombie* a, Zombie* b);
};

class Comparator_Max_Life {
public:
	bool operator()(Zombie* a, Zombie* b);
};

class Comparator_Min_Life {
public:
	bool operator()(Zombie* a, Zombie* b);
};

#endif // !COMPARATORS_H

