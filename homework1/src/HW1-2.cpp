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
