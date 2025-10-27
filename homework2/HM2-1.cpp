#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;  // forward declaration

// ------------------ Term ���O ------------------
class Term {
    friend class Polynomial; // �� Polynomial �i�H�����s�� Term ������
private:
    float coef;  // �Y��
    int exp;     // ����
};

// ------------------ Polynomial ���O ------------------
class Polynomial {
private:
    Term* termArray;  // �x�s�D�s�����}�C
    int capacity;     // �}�C�e�q
    int terms;        // ��ڨϥΤ�������

public:
    // �غc�l�G�إ� p(x)=0
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    // �����غc�l
    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }



    // ���J�@�ӷs���]�����ϥΡ^
    void NewTerm(float c, int e) {
        if (c == 0) return; // ���[�J�s�Y��

        // �Y���ۦP�����ƶ��A�h�X��
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    // �Y�X�֫��ܦ� 0�A�h�R������
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }

        // �Y�}�C���A�X�R�e�q
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        // ���J�s��
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;

        // �O���H���ƻ���Ƨ�
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    // ------------------ Add ------------------
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
        // �Ѿl��
        for (; aPos < terms; aPos++)
            result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < poly.terms; bPos++)
            result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
        return result;
    }

    // ------------------ Mult ------------------
    Polynomial Mult(Polynomial poly) {
        Polynomial result;

        // �� this ���C�@��
        for (int i = 0; i < terms; i++) {
            Polynomial temp;

            // �� poly ���C�@��
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;

                // �Ȯɭ��X���C�@����i temp
                temp.NewTerm(newCoef, newExp);
            }

            // �֥[�쵲�G�]�� Add() �X�֦P�����^
            result = result.Add(temp);
        }

        return result;
    }

    // ------------------ Eval ------------------
    float Eval(float f) {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(f, termArray[i].exp);
        return result;
    }

    // ------------------ ��X�h���� ------------------
    void Show() {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }

        for (int i = 0; i < terms; i++) {
            float c = termArray[i].coef;
            int e = termArray[i].exp;

            // �L�X�Ÿ�
            if (i > 0) {
                if (c >= 0) cout << " + ";
                else {
                    cout << " - ";
                    c = -c; // �N�Y���ର���ƿ�X
                }
            }
            else {
                if (c < 0) {
                    cout << "-";
                    c = -c;
                }
            }

            // �ھڤ��P���ƿ�X�榡
            if (e == 0)
                cout << c;               // �`�ƶ�
            else if (e == 1)
                cout << c << "x";        // �@����
            else
                cout << c << "x^" << e;  // ��L����
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