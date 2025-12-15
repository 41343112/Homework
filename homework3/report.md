# 41343112
作業三
## 解題說明
本程式實作一個多項式運算系統（Polynomial），透過鏈結串列（Chain）與可用節點串列（Available List）來管理記憶體，並支援多項式的加法、減法、乘法以及代值計算。  
   
每一項多項式以(係數 coef, 指數 exp)表示，並在插入新項時自動合併指數相同的項目，依照指數由大到小排序儲存。
## 解題策略
  - ### 使用鏈結串列儲存多項式
    -  由於多項式的項數不固定，且在運算過程中常需要插入、合併或刪除項目，若使用陣列結構將造成頻繁的資料搬移，效能不佳。因此，使用單向鏈結串列（Chain）來儲存多項式的各項。
-  ###  維持多項式項目排序  
   -  為了簡化加法、減法與乘法運算，本程式在插入新項目時即維持「依指數由大到小排序」的規則。
-  ### 導入 Available List 機制  
   -  多項式運算會頻繁產生與銷毀節點，若每次皆呼叫 new 與 delete，將造成效能下降與記憶體碎片化問題。因此，本程式實作 Available List（可      用節點串列） 作為節點回收機制。
   -  當多項式物件被解構或覆寫時，節點不立即刪除，改由 AvailableList 將節點回收保存，新增節點時優先取用回收節點，無可用節點時才配置新記憶體。
- ###   集中管理節點生命週期
   - 透過 release() 與 getBack() 的設計，節點的生命週期統一由 Available List 管理，使程式在記憶體配置與釋放上更具一致性與安全性。
- ###  使用 Iterator 進行走訪
   - 操作方式直覺、易於理解，讓多項式運算程式碼更簡潔。
- ###  運算子多載提升可讀性
## 程式實作
### ChainNode 類別
  ```cpp
template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode)
        : element(elem), next(nextNode) {}
    void setElement(const T& elem) {
        element = elem;
    }
    void setNext(ChainNode<T>* nextNode) {
        next = nextNode;
    }
};

```
- 每個節點儲存一個元素 element，並指向下一個節點 next。
- 提供建構子與設置方法，可初始化或修改節點。
- friend 讓 Chain、Iterator 與 AvailableList 可以直接操作節點。
### ChainIterator 類別
```cpp
template <class T>
class ChainIterator {
private:
    ChainNode<T>* currentNode;
public:
    ChainIterator(ChainNode<T>* startNode = nullptr) : currentNode(startNode) {}

    T& operator*() const { return currentNode->element; }
    T* operator->() const { return &currentNode->element; }
    
    ChainIterator& operator++() {
        if (currentNode) currentNode = currentNode->next;
        return *this;
    }

    ChainIterator operator++(int) {
        ChainIterator old = *this;
        if (currentNode) currentNode = currentNode->next;
        return old;
    }

    bool operator!=(const ChainIterator& right) const {
        return currentNode != right.currentNode;
    }
    bool operator==(const ChainIterator& right) const {
        return currentNode == right.currentNode;
    }

    int operator-(const ChainIterator& right) const {
        int distance = 0;
        ChainNode<T>* temp = right.currentNode;
        while (temp != currentNode) {
            if (temp == nullptr) {
                return -1;
            }
            temp = temp->next;
            ++distance;
        }
        return distance;
    }
};


```
- currentNode 指向目前節點
- *it 取得節點資料
- ++it 移動至下一節點
- 用於多項式加、減、乘與輸出
### Available List（可用節點串列）
```cpp
template <class T>
class AvailableList {
private:
    static ChainNode<T>* availableHead;
public:
    AvailableList() {}
    ~AvailableList() {
        while (availableHead) {
            ChainNode<T>* tmp = availableHead;
            availableHead = availableHead->next;
            delete tmp;
        }
    }

    static ChainNode<T>* getNode() {
        if (!availableHead) return nullptr;
        ChainNode<T>* nodeToReturn = availableHead;
        availableHead = availableHead->next;
        nodeToReturn->next = nullptr;
        return nodeToReturn;
    }

    static void getBack(ChainNode<T>* firstNode) {
        if (!firstNode) return;
        ChainNode<T>* current = firstNode;
        while (current->next) {
            current = current->next;
        }
        current->next = availableHead;
        availableHead = firstNode;
    }

    static bool isEmpty() {
        return availableHead == nullptr;
    }

    static ChainNode<T>* getOneNodeOrNew(const T& elem) {
        ChainNode<T>* node = getNode();
        if (!node) node = new ChainNode<T>(elem);
        else node->setElement(elem);
        return node;
    }
};

```
Available List 用來回收不再使用的節點，避免頻繁配置與釋放記憶體。
 - getNode()：取得一個可重複使用的節點。
 - getBack()：將整串節點回收到 Available List
 - etOneNodeOrNew() 可以自動重用或新建節點功能。
### Chain 類別
```cpp
template <class T>
class Chain {
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() {
        ChainNode<T>* current = head;
        while (current != nullptr) {
            ChainNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }

    ChainNode<T>* release() {
        ChainNode<T>* oldHead = head;
        head = nullptr;
        return oldHead;
    }

    ChainIterator<T> begin() const {
        return ChainIterator<T>(head);
    }
    ChainIterator<T> end() const {
        return ChainIterator<T>(nullptr);
    }
```
- head：指向鏈結串列第一個節點
- 建構子將 head 初始化為 nullptr，代表空串列
- 解構子呼叫 clear()，確保所有節點被正確回收
- begin()、end() 提供 iterator 介面，方便走訪

###
  
### 鏈結串列（Chain）插入
```cpp
   
void insert(int idx, const T& element) {
    ChainNode<T>* newNode = AvailableList<T>::getNode();
    if (!newNode)
        newNode = new ChainNode<T>(element);
    else
        newNode->setElement(element);

    if (idx == 0) {
        newNode->next = head;
        head = newNode;
    } else {
        ChainNode<T>* prev = head;
        for (int i = 0; i < idx - 1 && prev; ++i)
            prev = prev->next;

        if (prev) {
            newNode->next = prev->next;
            prev->next = newNode;
        } else {
            AvailableList<T>::getBack(newNode);
        }
    }
}
```
優先從 Available List 取得節點，若索引不合法，節點會被回收，確保鏈結串列結構正確。
### Term 結構
struct Term {
    double coef;
    int exp;
    Term() : coef(0.0), exp(0) {}
    Term(double c, int e) : coef(c), exp(e) {}
};

### 多項式新增項目（newTerm）
```cpp
void newTerm(double coef, int exp) {
    int index = 0;
    for (ChainIterator<Term> it = terms.begin();
         it != terms.end(); ++it, ++index) {

        if (it->exp < exp) {
            terms.insert(index, Term(coef, exp));
            return;
        } else if (it->exp == exp) {
            it->coef += coef;
            return;
        }
    }
    terms.insert(index, Term(coef, exp));
}
```
依指數大小插入適當位置，若指數相同，合併係數，保證多項式指數由大到小排序。



### 多項式加法運算實作
```cpp
Polynomial operator+(const Polynomial& other) const {
    Polynomial result;
    auto it1 = begin();
    auto it2 = other.begin();

    while (it1 != end() && it2 != other.end()) {
        if (it1->exp > it2->exp)
            result.newTerm(it1->coef, it1->exp), ++it1;
        else if (it1->exp < it2->exp)
            result.newTerm(it2->coef, it2->exp), ++it2;
        else {
            result.newTerm(it1->coef + it2->coef, it1->exp);
            ++it1; ++it2;
        }
    }
    return result;
}
```
同時走訪兩個多項式，指數大的項先加入結果，指數相同則合併係數。
### 多項式乘法運算實作
```cpp
Polynomial operator*(const Polynomial& other) const {
    Polynomial result;
    for (auto it1 = begin(); it1 != end(); ++it1)
        for (auto it2 = other.begin(); it2 != other.end(); ++it2)
            result.newTerm(it1->coef * it2->coef,
                           it1->exp + it2->exp);
    return result;
}
```
乘法使用雙層迴圈，將每一項互相相乘，並透過 newTerm() 自動合併同指數項。
### 多項式代值計算（Evaluate）
```cpp
float Evaluate(float x) const {
    float result = 0.0f;
    for (auto it = begin(); it != end(); ++it) {
        float power = 1.0f;
        for (int i = 0; i < it->exp; ++i)
            power *= x;
        result += it->coef * power;
    }
    return result;
}
```
此函式計算多項式在指定 x 值下的結果，透過逐次乘法計算次方，並將每一項累加。

