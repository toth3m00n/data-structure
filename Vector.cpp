#include <iostream>
#include <fstream>
#define min(a, b) a > b ? b : a

template<class T>
class Vector {
private:
    size_t length;
    size_t capacity;
    T* data;

    void Reallocate() {
        if (capacity == 0) {
            capacity = 2;
        } else {capacity += capacity/2;}

        T* new_data = new T[capacity];

        for (auto i = 0; i < length; ++i) {
            new_data[i] = data[i];
        }

        delete []data;
        data = new_data;
        new_data = nullptr;
    }

public:
    Vector() : length(0), capacity(0), data(nullptr) {}

    Vector(size_t size, const T &value) {
        length = capacity = size;
        data = new T[capacity];
        for (int i = 0; i < length; ++i) {
            data[i] = value;
        }
    }

    ~Vector() {
        if (data) {
           delete[] data;
        }
    }


    void push_back(const T &element) {
        if (length == capacity) {
            Reallocate();
        }
        data[length] = element;
        ++length;
    }

    void pop_back() {
        --length;
    }

    T& operator[](size_t index) {
        if (index < 0 || index > length) {
            throw "Invalid index\n";
        }
        return data[index];
    }

    size_t size() {
        return length;
    }

    Vector<T> &operator = (const Vector<T> &vector) {
        if (this == &vector) {
            return *this;
        }
        delete[] data;
        length = vector.length;
        capacity = vector.capacity;

        if (vector.data == nullptr) {
            data = nullptr;
        } else {
            data = new T[capacity];
            for (size_t i = 0; i < length; ++i) {
                data[i] = vector.data[i];
            }
        }
        return *this;
    }
};


template<class T>
std::ostream &operator << (std::ostream &out, Vector<T> &vector) {
    out << "[ ";
    for (int element = 0; element < vector.size(); ++element) {
        out << vector[element] << " ";
    }
    out << "]";
    out << "\n";
    return out;
}
