# 41343112
第二題
## 解題說明
(a)：透過隨機生成 n 個不同的數值並插入初始為空的 BST，測量其高度 h。接著計算比值 h/log n，驗證隨機 BST 的高度是否能維持在對數層級（Logarithmic level），而非退化為線性層級（Linear level） 。

(b)：實作 Delete 函數，處理刪除節點時的三種情況（葉子、單一子節點、雙子節點），並確保刪除後仍符合 BST 性質 。
## 解題策略
- 為了模擬平均情況下的 BST，我們不直接依序插入數值。採用 Fisher–Yates Shuffle 演算法：建立一個包含 $1$ 到 $n$ 的有序陣列。從最後一個元素開始，隨機與前方任一元素交換。確保每一種排列組合出現的機率均等，從而產生真正的隨機插入序列。

- 定義模板類別 TreeNode ：使用 std::pair<K, E> 儲存鍵值對 。包含左右子節點指標 left 與 right
- 刪除策略 :當刪除具有兩個子節點的節點時，尋找後繼者：呼叫 findMin(node->right) 找到右子樹中的最小值節點 。值替代：將後繼者的資料複製到目標節點。遞迴刪除：在右子樹中遞迴刪除該後繼者節點。

## 程式實作
(a)隨機插入
```cpp
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;


template <class K, class E>
class TreeNode {
public:
    pair<K, E> data;
    TreeNode<K, E>* left;
    TreeNode<K, E>* right;

    TreeNode(pair<K, E> e) {
        this->data.first = e.first;   
        this->data.second = e.second; 
        this->left = NULL;            
        this->right = NULL;           
    }
};


template <class K, class E>
class BST_Experiment {
private:
    TreeNode<K, E>* root;

    
    TreeNode<K, E>* insert(TreeNode<K, E>* node, const pair<K, E>& e) {
        if (!node) return new TreeNode<K, E>(e);
        if (e.first < node->data.first)      
            node->left = insert(node->left, e);
        else if (e.first > node->data.first)
            node->right = insert(node->right, e);
        return node;
    }

    int getHeight(TreeNode<K, E>* node) {
        if (!node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    void destroy(TreeNode<K, E>* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST_Experiment() : root(NULL) {}
    ~BST_Experiment() { destroy(root); }
    void Insert(const pair<K, E>& e) { root = insert(root, e); }
    int Height() { return getHeight(root); }
};


void shuffleArray(int* a, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(a[i], a[j]);
    }
}

int main() {
    srand((unsigned)time(NULL));
    int ns[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };

    cout << left << setw(10) << "n" << setw(10) << "Height" << "Ratio (h/log2n)" << endl;
    cout << string(40, '-') << endl;

    for (int n : ns) {
        BST_Experiment<int, int> tree;
        int* arr = new int[n];
        for (int i = 0; i < n; ++i) arr[i] = i + 1;

        shuffleArray(arr, n);
        for (int i = 0; i < n; ++i) {
            tree.Insert(make_pair(arr[i], arr[i]));
        }

        int h = tree.Height();
        double ratio = (double)h / log2((double)n);

        cout << left << setw(10) << n << setw(10) << h
             << fixed << setprecision(4) << ratio << endl;

        delete[] arr;
    }
    return 0;
}
```
(b)
```cpp
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

static Node* findMin(Node* node) {
    while (node && node->left) node = node->left;
    return node;
}

Node* deleteKey(Node* root, int k) {
    if (!root) return nullptr;

    if (k < root->key) {
        root->left = deleteKey(root->left, k);
    } else if (k > root->key) {
        root->right = deleteKey(root->right, k);
    } else {
        // 找到要刪的節點 root
        if (!root->left && !root->right) {
            delete root;
            return nullptr;
        }
        if (!root->left) {
            Node* r = root->right;
            delete root;
            return r;
        }
        if (!root->right) {
            Node* l = root->left;
            delete root;
            return l;
        }

      
        Node* succ = findMin(root->right);
        root->key = succ->key;
        root->right = deleteKey(root->right, succ->key);
    }
    return root;
}
```
## 效能分析
|項目|時間複雜度(最壞情況)|時間複雜度(平均)|說明|
|-------|--------------|----------------|----|
|搜尋    |$O(n)$ |$O(\log n)$ |時間複雜度取決於樹的高度 $h$|
|插入    |$O(n)$ |$O(\log n)$ |須先搜尋插入位置，過程與樹高成正比|
|刪除    |$O(n)$ |$O(\log n)$ |包含搜尋目標與尋找中序後繼節點的時間|
|高度    |$O(n)$ |$O(n)$ |必須遞迴遍歷所有節點才能確定最長路徑|

#### 空間複雜度
- 每個節點：𝑂(1)
- 總空間：𝑂(𝑛)

## 測試與驗證

| 測試案例 | 輸入參數 n | 預期輸出 height, height/log₂n | 實際輸出 height, height/log₂n |
|----------|------------|------------------------------|------------------------------|
| 測試一   | 100        | 12,1.80618                   | 12,1.80618                   |
| 測試二   | 500        | 18,2.00763                   | 18,2.00763                   |
| 測試三   | 1000       | 19,1.90652                   | 19,1.90652                   |
| 測試四   | 2000       | 23,2.09743                   | 23,2.09743                   |
| 測試五   | 3000       | 30,2.59723                   | 30,2.59723                   |
| 測試六   | 4000       | 30,2.50715                   | 30,2.50715                   |
| 測試七   | 5000       | 27,2.19732                   | 27,2.19732                   |
| 測試八   | 6000       | 29,2.31062                   | 29,2.31062                   |
| 測試九   | 7000       | 28,2.1921                    | 28,2.1921                    |
| 測試十   | 8000       | 30,2.31378                   | 30,2.31378                   |
| 測試十一 | 9000       | 32,2.43611                   | 32,2.43611                   |
| 測試十二 | 10000      | 29,2.18247                   | 29,2.18247                   |

## 申論及開發報告
 
