# 41343112
第二題
## 解題說明
本題要求將顯示集合裡的的所有子集合(幂集)。
## 解題策略
1.set[]存放輸入的集合元素。  
2.subset[]存放生成的子集合。  
3.二元樹考慮每個元素「選 / 不選」。  
4.用idx控制遞回到哪個元素，subsize控制子集合大小。  
5.用target控制輸出集合大小，達成幂集條件。  
## 程式實作
主要程式碼:
```cpp
#include<iostream>
using namespace std;

void generate(char set[], char subset[], int n, int idx, int subsize, int target) {
	if (subsize > target)return;
	if (idx == n) {
		if (subsize == target) {
			cout << "{";
			for (int i = 0; i < subsize; i++) {
				cout << subset[i];
				if (i != subsize - 1)cout << ",";
			}
			cout << "}" << endl;
		}
		return;
	}

	subset[subsize] = set[idx];

	generate(set, subset, n, idx + 1, subsize + 1, target);
	generate(set, subset, n, idx + 1, subsize, target);
}
int main() {
	int size;
	cout << "輸入集合大小: ";
	cin >> size;
	char* set = new char[size];
	char* subset = new char[size];
	cout << "輸入" << size << "個元素: " << endl;
	for (int i = 0; i < size; i++) {
		cin >> set[i];
	}

	for (int target = 0; target <= size; target++) {
		generate(set, subset, size, 0, 0, target);
	}
}
```
## 效能分析
1.時間複雜度: O
$(2^n)$。  
>二元樹有
$2^n$
條路徑。  
每條路在葉節點列印一個子集合，最多列印**n**個。  
>O
>$(n*2^n)$≈
>$(2^n)$。 

2.空間複雜度為:O(n)。
>set[]=O(n)。  
>subset[]=O(n)。  
>遞迴=O(n)。  
>O(n)+O(n)+O(n)=O(3n)=O(n)。
## 測試與驗證
|測試案例|輸入參數(size)&集合(set)|預期輸出|實際輸出|
| ----- | --------------------- | ----- | ------ |
|測試一 |size=0<br>set[]不用輸入|{}|{}
|測試二 | size=2<br>set[]=a,b     |{}<br>{a}<br>{b}<br>{a,b}|{}<br>{a}<br>{b}<br>{a,b} 
|測試三 | size=3<br>set[]=a,b,c   |{}<br>{a}<br>{b}<br>{c}<br>{a,b}<br>{a,c}<br>{b,c}<br>{a,b,c}|{}<br>{a}<br>{b}<br>{c}<br>{a,b}<br>{a,c}<br>{b,c}<br>{a,b,c}

### 編譯與執行指令
```shell
$ g++ HW1-2.cpp --std=c++21 -o HW1-2.exe 
$.\HW1-2.exe
輸入集合大小: 3
輸入3個元素:
a
b
c
{}
{a}
{b}
{c}
{a,b}
{a,c}
{b,c}
{a,b,c}
```
#### 結論
1.程式能順利的寫出子集合。  
2.可以按照題意幂集。
## 申論及開發

### 1. 陣列
- 存放輸入的集合元素和暫存子集合。
 ```cpp
cout << "輸入集合大小: ";
cin >> size;
char* set = new char[size];
char* subset = new char[size];
cout << "輸入" << size << "個元素: " << endl;
for (int i = 0; i < size; i++) {
	cin >> set[i];
}
```	  
### 2. 遞迴

  - 如果目前子集合大小已經超過target，立即return，不在繼續遞迴，避免生成過大的子集合。
```cpp
if (subsize > target)return;

```
<br><br>
 - 當搜尋到底的時候就顯示子集合。
```CPP
if (idx == n) {
	if (subsize == target) {
		cout << "{";
		for (int i = 0; i < subsize; i++) {
			cout << subset[i];
			if (i != subsize - 1)cout << ",";
		}
		cout << "}" << endl;
	}
	return;
}
```
<br><br>
- 暫存子集合。
```cpp
subset[subsize] = set[idx];
```
<br><br>
- 對下一個元素選擇，(subsize+1)選元素，(subsize)保持原樣。
```cpp
generate(set, subset, n, idx + 1, subsize + 1, target);
generate(set, subset, n, idx + 1, subsize, target);
```

	  
### 3. 控制集合大小 
- 用target限制每次輸出子集合的元素量，確保生成幂集時，依序輸出，避免無效的輸出。
```cpp
	for (int target = 0; target <= size; target++) {
		generate(set, subset, size, 0, 0, target);
	}
```
