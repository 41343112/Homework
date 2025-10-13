#  41343112
第一題
## 解題說明
1.輸入兩個整數m、n(皆為正整數)。  
2.根據遞迴定義，計算A(m,n)並輸出結果。  
3.Ackermann遞迴定義  
![Ackermann遞迴定義](https://github.com/41343112/HomeWork/blob/main/homework1/src/ackermann.png)
### 解題策略  
1.當m==0時，回傳n+1。  
2.當n==0時，回傳A(m-1,1)。  
3.其他狀況時，回傳A(m-1,A(m,n-1))。
## 程式實作
以下為主要程式碼  
```cpp
#include<iostream>
using namespace std;
int A(int m, int n) {
	if (m == 0) return n + 1;
	if (n == 0)return A(m - 1, 1);
	return A(m - 1, A(m, n - 1));
}

int main() {
	int m, n;
	cout << "輸入m, n : ";
	cin >> m >> n;
	cout << A(m, n)<<endl;
	
}
```
## 效能分析
| m值 |時間複雜度|空間複雜度|  
| --- | ------- | ------- |
| 0   | $O(1)$  | $O(1)$  |
| 1   | $O(n)$  | $O(n)$  |
| 2   | $O(n)$  | $O(n)$  |
| 3   | $O(2^n)$| $O(2^n)$ |
| >=4 | $O(A(m,n))$ | $O(A(m,n))$ |
## 測試與驗證
|測試案例|輸入m,n|預期輸出|實際輸出|
| ----- | ----- | ----- | ----- |
| 測試一| m=1,n=1| 3    | 3     |
| 測試二| m=2,n=1| 5    | 5     |
| 測試三| m=3,n=4| 125  | 125   |
| 測試四| m=4,n=1| 65533|堆疊溢出|

```shell
$ g++ HW1-1.cpp --std=c++21 -o  HW1-1.exe
$ .\HW1-1.exe
輸入m, n : 2 1
5
```
### 結論  
每次函式呼叫都會在記憶體中分一塊區域存放，因此只要呼叫函式到達一定的次數，就會超過記憶體容量，導致stack overflow(堆疊溢出)。
## 申論及開發報告
1.Ackermann函式本身就是高度遞迴的數學定義。  
2.主要使用**函式呼叫堆疊**，每次呼叫函式時，會將目前狀態放入堆疊，直到達到目標條件再逐層返回。  
3.用遞迴可以直接使用定義，結構清晰。  
```cpp
int A(int m, int n) {
	if (m == 0) return n + 1;		//當m==0時，回傳n+1。  
	if (n == 0)return A(m - 1, 1);	//當n==0時，回傳A(m-1,1)。  
	return A(m - 1, A(m, n - 1));	//其他狀況時，回傳A(m-1,A(m,n-1))。
}
```
