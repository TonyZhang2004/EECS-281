// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include "zoo.h"

int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);
	cout << std::setprecision(2);
	cout << std::fixed;
	Zoo zoo(argc, argv);
	zoo.init();
}