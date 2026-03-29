#include <iostream>
using namespace std;

template <class T>
class 
	MinPQ {
public:
	virtual~MinPQ() {}
	virtual bool IsEmpty()const = 0;
	virtual const T& Top()const = 0;
	virtual void Push(const T&) = 0;
	virtual void Pop() = 0;
};

template <class T>
class MinHeap:public MinPQ<T> {
private:
	T* heap;
	int capacity;
	int size;
	void ChangeSize() {
		T* oldheap = heap;
		capacity *= 2;
		heap = new T[capacity];
		for (int i = 1; i <= size; i++) {
			heap[i] = oldheap[i];
		}
		delete[] oldheap;
	}
public:
	MinHeap(int Capacity = 10) {
		capacity = Capacity;
		heap = new T[capacity];
		size = 0;
	}
	~MinHeap() {
		delete[] heap;
	}
	bool IsEmpty() const{
		return size == 0;
	}
	const T& Top() const{
		if (IsEmpty()) throw"Is Empty";
		return heap[1];
	}
	void Push(const T& e) {
		if (size+1 == capacity) {
			ChangeSize();
		}
		int index = ++size;

		while (index != 1 && heap[index / 2] > e) {
			heap[index] = heap[index / 2];
			index /= 2;
		}
		heap[index] = e;

	}
	void Pop() {
		if (IsEmpty())throw "Is Empty";
		heap[1].~T();

		T lastE = heap[size--];

		int currentNode = 1;
		int child = 2;
		while (child <= size) {
			if (child < size && heap[child] > heap[child + 1])	child++;

			
			if (lastE <= heap[child])break;

			heap[currentNode] = heap[child];
			currentNode = child;
			child *= 2;

		}
		heap[currentNode] = lastE;


	}
	void print() {
		for (int i = 1; i <= size; i++) {
			cout << heap[i] << endl;
		}
	}
};


int main() {

	MinHeap<int>  myheap;
	int n, e;
	cout << "輸入個數" << endl;
	cin >> n;
	
	for (int i = 0; i < n; i++) {
		cout << "輸入第"<<i+1<<"個元素" << endl;
		cin >> e;
		myheap.Push(e);
	}
	myheap.print();

}
