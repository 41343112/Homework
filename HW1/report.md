# 41343112
第一題
## 解題說明
實作一個最小堆積優先權隊列 (Min-Heap Priority Queue)，使用完全二元樹(complete Binary Tree)，並以動態陣列儲存。  
對於樹中任一節點i，其父節點P的值要滿足Value(P)<=Value(i)。
## 解題策略
使用索引1作為根結點
-  LeftChild(i) = 2*i  
-  RightChild(i) = 2*i+1
-  Parent(i) = i/2
 ### 上浮-Push  
  當新素插入尾端時，與其父節點比較。若新元素較小則將父節點下移。  
### 下沉-Pop  
移除根節點後，將最後一個元素移至根部，隨後與子節點中「較小者」比較並下修位置，確保根部維持最小值。

## 程式實作
```cpp
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
```
## 效能分析
 |funtion|時間複雜度|說明|
 |-------|---------|-----|
 |$Push()$ |$O(log n)$|上浮|
 |$Pop()$  |$O(log n)$|下沉|
 |$Top()$  |$O(1)$|陣列第一位|
 |$IsEmpty()$|$O(1)$|檢查size|

 
 #### 空間複雜度
 $O(n)$
 n為當前capacity的大小。

 ## 測試與驗證
 |測試案例|輸入|預期輸出|實際輸出|
 |-------|---|--------|-------|
 |測試  |輸入6 15 5 20 1 10 3 |1 5 3 15 10 20|1 5 3 15 10 20|

 ## 申論及開發報告
 實作過程中，不使用heap[0]，從 heap[1] 開始儲存資料。  
 - 優點：這使得父子節點的索引關係變得極為簡潔。對於節點 $i$，左子節點為 $2i$，右子節點為 $2i+1$，父節點為 $i/2$。這省去了從 0 開始索引時頻繁出現的 +1 或 -1 偏移運算，降低了邏輯出錯的機率。  
 - 代價：雖然浪費了一個 T 型別的空間，但在現代記憶體環境下，換取的程式碼可讀性與穩定性。


 在
$Pop()$
函式中，不能隨機選擇一個子節點交換。必須先比較左右子節點，選出其中的較小者，再與當前節點進行比較。


透過
$ChangeSize()$
倍增法，以空間換時間，最佳化插入效率。
