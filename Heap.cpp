#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Heap {
    vector<int> array;

public:

    Heap(vector<int> enter_array) : array(enter_array) {}; 

    int RigthChild(int index) {
        return (2*index + 2);
    }

    int LeftChild(int index) {
       return (2*index + 1);
    }

    int Parent(int index) {
        return (index - 1) / 2;
    }

    void ShiftUp(int index) {
        int parent = Parent(index);
        if (parent >= 0) {
            if (array[index] < array[parent]) {
                swap(array[index], array[parent]);
                ShiftUp(parent);
            }
        }
    }

    void ShiftDown(int index) {
        int left = LeftChild(index);
        if (left < array.size()) {
            int rigth = RigthChild(index);
            int minimum = left;

            if (rigth < array.size()) {
                minimum = (array[left] < array[rigth]) ? left : rigth;
            }     

            if (array[index] > array[minimum]) {
                swap(array[index], array[minimum]);
                ShiftDown(minimum);
            }
        }
    }

    void Heapify() {
        for (int i = array.size()/2 - 1; i >= 0; --i) {
            ShiftDown(i);
        }
    }

    void Insert(int el) {
        array.push_back(el);
        ShiftUp(array.size() - 1);
    }

    void Delete() {
        swap(array[0], array[array.size() - 1]);
        array.pop_back();
        ShiftDown(0);
    }

    void Print() {
        for (auto i: array) {
            cout << i << " ";
        }
        cout << "\n";
    }

};

int main(){
    Heap heap({1, 9, 3, 7, 11, 12, 14, 1});
    heap.Print();
    heap.Heapify();
   /* heap.Insert(2);
    heap.Print();
    heap.Delete();*/
    heap.Print();
    return 0;
}