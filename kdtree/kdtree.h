#ifndef KDTREE_H
#define KDTREE_H

const int PADDING = 15;

#include <iterator>
#include <vector>
#include <iostream>

template <class T, size_t K>
class KDTree {
	public:
		class Point {
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

		class Node {
			public:
				Point value;
				Node* leftChild = nullptr;
				Node* rightChild = nullptr;

				Node(const Point &value): value(value) {};
		};

		Node* root = nullptr;
		size_t size = 0;

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

		void print(Node*, int level);
		void print();
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
void KDTree<T, K>::Point::print() {
	std::cout << '(';
	for (int x : value) {
		std::cout << x << ", ";
	}
	std::cout << "\b\b)";
}

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
