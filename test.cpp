#include <iostream>
#include <vector>
#include "kdtree.h"

const int MAX = 100;
const int N =  20;
const int K = 2;

bool find(const std::vector<std::vector<int>> &vec, const std::vector<int> &x) {
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

int rand(int l, int r) {
	return l + rand() % (r - l + 1);
}

void populate_vector(std::vector<int> &vec) {
	for (int &x : vec) {
		x = rand(1, N);
	}
}

int main() {
	KDTree<int, K> kdtree;

	std::vector<std::vector<int>> test(N);

	srand(time(0));

	for (int i = 0; i < N; i++) {
		test[i].resize(K);
		populate_vector(test[i]);
	}

	for (std::vector<int> p : test) {
		kdtree.insert(p);
	}

	for (std::vector<int> p : test) {
		assert(kdtree.find(p));
	}

	for (int i = 0; i < N; i++) {
		std::vector<int> p(K);
		populate_vector(p);
		assert(find(test, p) == kdtree.find(p));
	}

	for (int i = 1; i <= 10; i++) {
		std::vector<int> v(K);
		populate_vector(v);

		KDTree<int, 2>::Point p(v);
		KDTree<int, 2>::Point q = kdtree.nearest(p);

		std::cout << "-------Test " << i << "-------" << std::endl;
		std::cout << "Query Point: ";
		p.print();
		std::cout << std::endl << "NN found: ";
		q.print();
		std::cout << std::endl;
		std::cout << "--------------------" << std::endl;
	}

	kdtree.print();

	return 0;
}

