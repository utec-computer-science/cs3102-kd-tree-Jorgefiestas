#ifndef KDTREE_H
#define KDTREE_H

const int PADDING = 15;

#include <iterator>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

template <class T, size_t K = 2>
class KDTree {
	public:
		class Point;

	private:
		using len_t = double;
		class Node;

		Node* root = nullptr;
		size_t size = 0;

		len_t dist(const Point&, const Point&);

	public:
		KDTree() = default;

		void insert(const Point&);
		template <class Iterator> 
			void insert(Iterator, Iterator);
		template <class Container> 
			void insert(Container);

		bool find(const Point&);
		template <class Iterator> 
			bool find(Iterator, Iterator);
		template <class Container> 
			bool find(Container);

		Node* nearest(Node*, const Point&, size_t, Node*, len_t);
		Point nearest(const Point&);
		template <class Iterator> 
			Point nearest(Iterator, Iterator);
		template <class Container> 
			Point nearest(Container);

		void print(Node*, int level);
		void print();
};

template <class T, size_t K>
class KDTree<T, K>::Point {
	private:
		T value[K];

	public:
		Point(const Point&);

		template <class Iterator>
			Point(Iterator, Iterator);

		template <class Container>
			Point(Container container):
				Point(cbegin(container), cend(container)) {}

		T& operator [](size_t);
		T operator [](size_t) const;
		T operator ==(const Point&) const;
		void print();
};

template <class T, size_t K>
class KDTree<T, K>::Node {
	private:
		Point value;
		Node* leftChild = nullptr;
		Node* rightChild = nullptr;
		friend class KDTree;

	public:
		Node(const Point &value): value(value) {};
};
template <class T, size_t K>
template<class Iterator>
KDTree<T, K>::Point::Point(Iterator first, Iterator last) {
	if (std::distance(first, last) != K) {
		throw "Invalid iterator range!";
	}

	size_t i = 0;
	for (Iterator it = first; it != last; it++, i++) {
		value[i] = *it;
	}
}

template <class T, size_t K>
KDTree<T, K>::Point::Point(const Point &_value) {
	for (int i = 0; i < K; i++) {
		value[i] = _value[i];
	}
}

template <class T, size_t K>
T& KDTree<T, K>::Point::operator [](size_t idx) {
	return value[idx];
}

template <class T, size_t K>
T KDTree<T, K>::Point::operator [](size_t idx) const {
	return value[idx];
}

template <class T, size_t K>
T KDTree<T, K>::Point::operator ==(const Point &point) const {
	for (int i = 0; i < K; i++) {
		if (value[i] != point[i]) {
			return false;
		}
	}
	return true;
}

template <class T, size_t K>
typename KDTree<T, K>::len_t KDTree<T, K>::dist(const Point &x, const Point &y) {
	len_t d = 0;
	for (size_t i = 0; i < K; i++) {
		d += (x[i] - y[i]) * (x[i] - y[i]);
	}
	return std::sqrt(d);
}

template <class T, size_t K>
void KDTree<T, K>::Point::print() {
	std::cout << '(';
	for (int x : value) {
		std::cout << x << ", ";
	}
	std::cout << "\b\b)";
}


// Insert

template <class T, size_t K>
void KDTree<T, K>::insert(const Point &point) {
	
	Node **node = &root;
	int dim = 0;

	while (*node) {
		if (point[dim] <= (*node)->value[dim]) {
			node = &(*node)->leftChild;
		}
		else {
			node = &(*node)->rightChild;
		}

		dim = (dim + 1) % K;
	}

	*node = new Node(point);
}

template <class T, size_t K>
template <class Iterator>
void KDTree<T, K>::insert(Iterator first, Iterator last) {
	Point point(first, last);
	insert(point);
}

template <class T, size_t K>
template <class Container>
void KDTree<T, K>::insert(Container container) {
	Point point(container);
	insert(point);
}


// Find

template <class T, size_t K>
bool KDTree<T, K>::find(const Point &point) {
	Node **node = &root;
	int dim = 0;

	while (*node) {
		if (point == (*node)->value) {
			return true;
		}
		else if (point[dim] <= (*node)->value[dim]) {
			node = &(*node)->leftChild;
		}
		else {
			node = &(*node)->rightChild;
		}

		dim = (dim + 1) % K;
	}
	
	return false;
}	

template <class T, size_t K>
template <class Iterator>
bool KDTree<T, K>::find(Iterator first, Iterator last) {
	Point point(first, last);
	return find(point);
}

template <class T, size_t K>
template <class Container>
bool KDTree<T, K>::find(Container container) {
	Point point(container);
	return find(point);
}

// NN Search 

template <class T, size_t K>
typename KDTree<T, K>::Node* KDTree<T, K>::nearest(
		Node* node, const Point &q, size_t cd, Node* best, len_t best_dist) {

	if (!node) {
		return nullptr;
	}

	len_t d = dist(node->value, q);
	len_t dx = node->value[cd] - q[cd];
	len_t dx2 = dx * dx;


	if (d < best_dist) {
		best_dist = d;
		best = node;
	}

	size_t cd_next = (cd + 1);
	if (cd >= K) {
		cd = 0;
	}

	Node* first = node->leftChild;
	Node* second = node->rightChild;
	if (q[cd] > node->value[cd]) {
		std::swap(first, second);
	}

	Node *candidate = nearest(first, q, cd_next, best, best_dist);
	if (candidate) {
		len_t dl = dist(candidate->value, q);
		if (dl < best_dist) {
			best_dist = dl;
			best = candidate;
		}
	}

	if (dx2 >= best_dist) {
		return best;
	}

	candidate = nearest(second, q, cd_next, best, best_dist);
	if (candidate) {
		len_t dl = dist(candidate->value, q);
		if (dl < best_dist) {
			best_dist = dl;
			best = candidate;
		}
	}

	return best;
}

template <class T, size_t K>
typename KDTree<T, K>::Point KDTree<T, K>::nearest(const Point &point) {
	Node* nn = nearest(root, point, 0, nullptr, 1e18);
	return nn->value;
}	

template <class T, size_t K>
template <class Iterator>
typename KDTree<T, K>::Point KDTree<T, K>::nearest(Iterator first, Iterator last) {
	Point point(first, last);
	return nearest(point);
}

template <class T, size_t K>
template <class Container>
typename KDTree<T, K>::Point KDTree<T, K>::nearest(Container container) {
	Point point(container);
	return nearest(point);
}	


// Utilies

template <class T, size_t K>
void KDTree<T, K>::print(Node* node, int level) {
	if (!node) {
		return;
	}
	
	print(node->rightChild, level + 1);

	for (int i = 0; i < level * PADDING; i++) {
		std::cout << ' ';
	}
	node->value.print();
	std::cout << std::endl;

	print(node->leftChild, level + 1);
}

template <class T, size_t K>
void KDTree<T, K>::print() {
	print(root, 0);
}

#endif
