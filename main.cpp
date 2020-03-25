#include <vector>
#include "kdtree/kdtree.h"

bool find(std::vector<std::vector<int>> &vec, std::vector<int> x) {
	for (std::vector<int> y : vec) {
		bool eq = true;
		for (int i = 0; i < x.size(); i++) {
			if (x[i] != y[i]) {
				eq = false;
				break;
			}
		}
		if (eq) {
			return true;
		}
	}

	return false;
}

const int N = 5;

int main() {
	KDTree<int, 2> kdtree;

	std::vector<std::vector<int>> test(20);

	srand(time(0));

	for (int i = 0; i < 20; i++) {
		test[i] = {rand()%N, rand()%N};
	}

	for (std::vector<int> p : test) {
		kdtree.insert(p);
	}

	for (int i = 0; i < 20; i++) {
		std::vector<int> p{rand()%N, rand()%N};
		std::cout << find(test, p) <<  " -> " << kdtree.find(p) << std::endl;
	}

	kdtree.print();

	return 0;
}

