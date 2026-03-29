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
class Dictionary {
public:
    virtual bool IsEmpty() const = 0;
    virtual void Insert(const pair<K, E>&) = 0;
    virtual void Delete(const K&) = 0;
};


template <class K, class E>
class BST : public Dictionary<K, E> {
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


    TreeNode<K, E>* findMin(TreeNode<K, E>* node) {
        if (node->left == NULL) return node; 
        else return findMin(node->left);    
    }


    TreeNode<K, E>* deleteNode(TreeNode<K, E>* node, const K& k) {
        if (!node) return NULL;

        if (k < node->data.first) {
            node->left = deleteNode(node->left, k);
        }
        else if (k > node->data.first) {
            node->right = deleteNode(node->right, k);
        }
        else {
       
            if (!node->left) { 
                TreeNode<K, E>* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                TreeNode<K, E>* temp = node->left;
                delete node;
                return temp;
            }
        
            TreeNode<K, E>* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.first);
        }
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
    BST() : root(NULL) {}
    ~BST() { destroy(root); }
    bool IsEmpty() const { return root == NULL; }
    void Insert(const pair<K, E>& e) { root = insert(root, e); }
    void Delete(const K& k) { root = deleteNode(root, k); }
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
        BST<int, int> tree;
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
