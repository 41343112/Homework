# 41343112
第一題
## 解題說明
1.建立多項式(Polynamial)  
2.插入新項(NewTerm)  
3.計算多項式的加法(Add)  
4.計算多項式的乘法(Mult)  
5.計算當x=f時的值(Eval)  
6.格式輸出(Show)
### 解題策略
#### 1.Term  
- 將多項式拆成獨立的項。  
- 例:
  $3x^2+2x+1$
可以分成(係數,指數)
- (3,2)
- (2,1)
- (1,0)
  


#### 2.Polynomial  
- 用一個陣列儲存多個項(Term* termArray)
- capacity: 陣列最大的大小。
- terms: 目前實際有多少項。
- 使用指數遞減排序，方便後續計算。

#### 3.NewTerm
- 輸入係數為0->不加入。
- 有相同係數->合併係數。
- 合併係數為0->刪除該項。
- 陣列空間滿->擴充。

#### 4.多項式加法
- 使用雙指標比較指數。
- 如果相等->係數相加。
- 如果一邊大->直接放結果。
- 處理完後->將剩下的項加進結果。
  
#### 5.多項式乘法
- 使用分配律
$(a_1x^{p_1} + a_2x^{p_2}) \times (b_1x^{q_1} + b_2x^{q_2})$
- 新項的係數：
  $a_i \times b_j$
  ，新項的指數：
$p_i + q_j$

#### 6.代入求值
- 用pow()計算次方。
- 累加每一項的值。

#### 7.顯示多項式
- 正負號控制(第一項不印+)
- 0次項只印數字。
- 1次項印成
  $x$
- 其他向印
  $x^n$


## 程式實作
  ```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial; // 讓 Polynomial 可以直接存取 Term 的成員
private:
    float coef;  // 係數
    int exp;     // 指數
};

class Polynomial {
private:
    Term* termArray;  // 儲存非零項的陣列
    int capacity;     // 陣列容量
    int terms;        // 實際使用中的項數

public:
    // 建構子：建立 p(x)=0
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    // 拷貝建構子
    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }



    // 插入一個新項（內部使用）
    void NewTerm(float c, int e) {
        if (c == 0) return; // 不加入零係數

        // 若有相同的指數項，則合併
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    // 若合併後變成 0，則刪除此項
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }

        // 擴充容量
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        // 插入新項
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;

        // 保持以指數遞減排序
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }


    Polynomial Add(Polynomial poly) {
        Polynomial result;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < poly.terms) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
                if (sum != 0)
                    result.NewTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp > poly.termArray[bPos].exp) {
                result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
                bPos++;
            }
        }
        // 剩餘項
        for (; aPos < terms; aPos++)
            result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < poly.terms; bPos++)
            result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
        return result;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial result;

        // 對 this 的每一項
        for (int i = 0; i < terms; i++) {
            Polynomial temp;

            // 對 poly 的每一項
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;

                // 暫時乘出的每一項放進 temp
                temp.NewTerm(newCoef, newExp);
            }

            // 累加到結果（用 Add() 合併同類項）
            result = result.Add(temp);
        }

        return result;
    }


    float Eval(float f) {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(f, termArray[i].exp);
        return result;
    }

    void Show() {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }

        for (int i = 0; i < terms; i++) {
            float c = termArray[i].coef;
            int e = termArray[i].exp;

            // 印出符號
            if (i > 0) {
                if (c >= 0) cout << " + ";
                else {
                    cout << " - ";
                    c = -c; // 將係數轉為正數輸出
                }
            }
            else {
                if (c < 0) {
                    cout << "-";
                    c = -c;
                }
            }

            if (e == 0)
                cout << c;               // 常數項
            else if (e == 1)
                cout << c << "x";        // 一次項
            else
                cout << c << "x^" << e;  // 其他次方
        }

        cout << endl;
    }
};


int main() {
    Polynomial p1, p2;
    p1.NewTerm(3, 2);  // 3x^2
    p1.NewTerm(2, 1);  // 2x
    p1.NewTerm(1, 0);  // 1

    p2.NewTerm(1, 1);  // x
    p2.NewTerm(1, 0);  // +1

    cout << "P1(x) = "; p1.Show();
    cout << "P2(x) = "; p2.Show();

    Polynomial sum = p1.Add(p2);
    cout << "Sum = "; sum.Show();

    Polynomial product = p1.Mult(p2);
    cout << "Product = "; product.Show();

    cout << "P1(2) = " << p1.Eval(2) << endl;

    return 0;
}
```

##  效能分析  

|函式名稱|時間複雜度|空間複雜度|
|-------|---------|---------|
|$NewTerm()$|$O(t^2)$ |$O(t)$ |
|$Add()$|$O(m+n)$ |$O(m+n)$ |
|$Mult()$|$O(m*n)$|$O(m*n)$ |
|$Eval()$|$O(k)$  |$O(1)$   |
|$Show()$|$O(k)$  |$O(1)$   |

## 測試與驗證  

|測試案例|設定參數|預期輸出|實際輸出|
|-------|-------|--------|-------|
|測試一 |p1.NewTerm(3,2)<br>p1.NewTerm(2,1)<br>p1.NewTerm(1,0)<br><br>p2.NewTerm(1,1)<br>p2.NewTerm(1,0)|P1(x) = $3x^2 + 2x + 1$<br> P2(x) =1x + 1 <br>Sum = $3x^2 + 3x + 2$<br>Product = $3x^3 + 5x^2 + 3x + 1$<br>P1(2) = 17|P1(x) = $3x^2 + 2x + 1$<br> P2(x) =1x + 1 <br>Sum = $3x^2 + 3x + 2$<br>Product = $3x^3 + 5x^2 + 3x + 1$<br>P1(2) = 17|  


### 結論  

  1.程式成功實現各種功能。  
  2.每個步驟輸出正確。  
  3.顯示順序清楚，格式易讀。  

## 申論及開發報告  

  - 本程式使用物件導向的概念，將多項式的每個功能模組化。
  - 使用動態陣列的方式來儲存多項式的非零項，當多項式增加時，會自動擴展記憶體容量，這樣可以避免預先宣告過大的陣列造成浪費。
  - 模組化有良好的重複使用和擴充性，能有效隱藏內部的實作細節。

  
