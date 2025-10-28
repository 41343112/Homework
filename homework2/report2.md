# 41343112
第二題
## 解題說明
本題要求實做一個Polynomial類別，能夠表示輸入，輸出及計算多項式的值。
### 解題策略
- 使用Term結構儲存美一項的資料(係數，指數)。  
- 使用動態陣列termArray儲存所有項目，當空間滿時，自動擴充容量。  
- 呼叫NewTerm()加入多項式，同時自動檢查是否需要合併或擴充。  
- 在NewTerm()中檢查是否有相同的指數的項，有則將係數相加。  
- 使用氣泡排序法，方便後續的計算。  
- 處理正負號合不同指數的輸出格式。  

## 程式實作

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream&, const Polynomial&);
private:
    float coef; 
    int exp;   
};

class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

 

    void NewTerm(float c, int e) {
        if (c == 0) return;

       
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                   if (termArray[i].coef == 0) {
                      for (int j = i; j < terms - 1; j++)
                          termArray[j] = termArray[j + 1];
                      terms--;
                }
                return;
            }
        }

     
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;


        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};

istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    cout << "Enter number of terms: ";
    in >> n;

    poly.terms = 0; // 清空原有內容

    for (int i = 0; i < n; i++) {
        float c;
        int e;
        cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
        in >> c >> e;
        poly.NewTerm(c, e);
    }

    return in;
}

ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;

       
        if (i > 0) {
            if (c >= 0) out << " + ";
            else { out << " - "; c = -c; }
        }
        else if (c < 0) {
            out << "-";
            c = -c;
        }

        // 處理輸出格式
        if (e == 0)
            out << c;
        else if (e == 1)
            out << c << "x";
        else
            out << c << "x^" << e;
    }

    return out;
}


int main() {
    Polynomial p;

    cin >> p;       
    cout << "p(x) = " << p << endl; 

    float x;
    cout << "Enter x value: ";
    cin >> x;
    cout << "p(" << x << ") = " << p.Eval(x) << endl;

    return 0;
}
```
## 效能分析
|操作       |時間複雜度|空間複雜度|說明|
|-----------|--------|----------|----|
|NewTerm()  |$O(n)$ <br>氣泡排序法$O(n^2)$ |$O(n)$ |自動合併同指數項、刪除零係數項並排序|
|Eval($x$)  |$O(n)$  |$O(1)$    |每一項需計算一次乘冪和乘法|
|operator>> |$O(n^2)$|$O(n)$    |每新增一項都可能會處發排續|
|operator<< |$O(n)$  |$O(1)$    |輸出所有項目一次|

## 測試與驗證
|測試案例|輸入參數 項目數量($n$)、(係數,指數)、($x$)值|預期輸出|實際輸出|
|-------|---------------------------------|-------|-------|
|測試一 |n=3<br>(-5,5)、(5,5)、(100,0)<br>$x$=100|p(x) = 100<br>p(100) = 100|p(x) = 100<br>p(100) = 100|
|測試二 |n=4<br>(4,3)、(3,2)、(2,1)、(1,0)<br>$x$=2|p(x) = 4x^3 + 3x^2 + 2x + 1<br>p(2) = 49|p(x) = 4x^3 + 3x^2 + 2x + 1<br>p(2) = 49|
|測試三 |n=5<br>(-3,3)、(4,2)、(-6,4)、(9,1)、(-5,5)<br>$x$=5|p(x) = -5x^5 - 6x^4 - 3x^3 + 4x^2 + 9x<br>p(5) = -19605|p(x) = -5x^5 - 6x^4 - 3x^3 + 4x^2 + 9x<br>p(5) = -19605|

### 結論
1.正負號輸出無誤。  
2.同指數係數相加為0時會刪除。  
3.運算結果正確。  

## 申論及開發報告
1.    程式邏輯簡單直觀  
      - Polynomial類別將多項式的每一項封裝成Term類別。  
      - 新增項目時，程式會檢檢查是否存在相同指數，有則合併。
      

2. 易於理解與實現  
    - NewTerm()：新增或合併多項式項目  
    - Eval(x)：計算多項式在 x 的值
    - operator>> 與 operator<<：以數學格式輸入與輸出多項式
  
 3. 語意清楚
    - 每次新增項目對應一個「子問題」：若存在相同指數，則合併；若不存在則插入並排序。
    - Eval(x) 函式中，每一項獨立計算，最終逐項累加得到多項式值。
    - 這種設計簡化了邏輯，不需額外變數追蹤中間狀態，使程式結構清楚、易於維護。

   
透過這種設計方式，成功實現了多項式的計算與輸入輸出功能，程式邏輯直觀、容易理解，且維護與擴充成本低，但是當多項式項數非常多時，排序與合併的時間複雜度較高，可能影響效率。
