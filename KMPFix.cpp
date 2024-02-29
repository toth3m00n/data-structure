#include <iostream>
#include <fstream>
#define min(a, b) a > b ? b : a


struct Location {
    unsigned long x;
    unsigned long y;
};

template<class T>
class Queue {
private:
    size_t size;
    size_t capacity;
    size_t header;
    T* array;

public:
    Queue(int m) {
        header = size = 0;
        capacity = m;
        array = new T[m];

    }

    ~Queue() { // вообще-то занулять не надо, но пусть будет
        capacity = 0;
        size = 0;
        header = 0;
        if (array) {
            delete[] array;
        }
    }

    T& Front() {
        return array[header];
    }
    
    void Push(const T &element) {
        if (size == capacity) {
            header = 0;
        }
        array[(header + size)%capacity] = element;
        ++size;
    }

    void Pop() {
        --size;
        ++header;
    }
};

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

Vector<unsigned long> ComputeZfunction(Vector<unsigned long> &pattern) {
    Vector<unsigned long> z_function(pattern.size(), 0);
    int left = 0;
    int right = 0, matches = 0;
    size_t lenght = pattern.size();
    for (int i = 1; i < lenght; ++i) {
        if (i <= right){
            z_function[i] = min(right - i + 1, z_function[i - left]);
        }
        while (i + z_function[i] < lenght && pattern[z_function[i]] == pattern[i + z_function[i]]) {
            ++z_function[i];
        }
        if (i + z_function[i] - 1 > right) {
            left = i;
            right = i + z_function[i] - 1;
        }
    }
    return z_function;
}

Vector<unsigned long>  ComputeStrongSP(Vector<unsigned long> &z_function) {
    Vector<unsigned long> strong_sp(z_function.size(), 0);
    int index = 0;
    for(int i = z_function.size() - 1; i >= 1; --i) {
        if (z_function[i] != 0 ) {
            index = i + z_function[i] - 1;
            strong_sp[index] = z_function[i];
        }
    }
    return strong_sp;
}



void KMP(Vector<unsigned long> &pattern,  Vector<unsigned long> &text, Vector<unsigned long> &strong_sp,Vector<unsigned long> &x, Vector<unsigned long> &y) {
    size_t text_current = 0;
    size_t pattern_current = 0;
    size_t patternt_size = pattern.size();
    size_t text_size  = text.size(); 
    std::ofstream out("output.txt");
    while (text_current + (patternt_size - 1 - pattern_current) < text_size) {
        while ( pattern_current <= patternt_size - 1 && pattern[pattern_current] == text[text_current]) {
            ++pattern_current;
            ++text_current;
        } 
        if (pattern_current == patternt_size) {
            out << x[text_current - patternt_size] << ", ";
            out << y[text_current - patternt_size] << "\n";
           // std::cout << occurrences; 
        }
        if (pattern_current == 0) {
            ++text_current;
        } else { pattern_current = strong_sp[pattern_current - 1]; }
    }
}

void SpaceEater(std::ifstream &in) {
    char space_victim;
    do {
        space_victim = in.get();
    }
    while (space_victim == ' ' || space_victim == '\t');
    in.unget();
}

void EnterPattern(std::ifstream &in, Vector<unsigned long> &pattern) {
    unsigned long number;
    char symbol;

    in >> number;
    pattern.push_back(number);
    SpaceEater(in);

    while ((symbol = in.get()) != '\n') {
        in.unget();
        in >> number;
        SpaceEater(in);
        pattern.push_back(number);
    }
    in.unget();
}

void EnterText(std::ifstream &in, Vector <unsigned long> &text, Vector <unsigned long> &x_coor, Vector <unsigned long> &y_coor) {
    unsigned long number;
    unsigned long count_colomn = 1;
    unsigned long count_row = 1;
    char symbol;
    SpaceEater(in);
    while((symbol = in.get()) == '\n') {
        SpaceEater(in);
        ++count_row;
    }
    in.unget();
    --count_row; 

    while (in >> number) {
        text.push_back(number);
        x_coor.push_back(count_row);
        y_coor.push_back(count_colomn);
        SpaceEater(in);
        ++count_colomn;
        while((symbol = in.get()) == '\n') {
            ++count_row;
            count_colomn = 1;
        }
        in.unget();
    }
}

void PrintResult( Vector<unsigned long> &occurences, Vector<unsigned long> &x, Vector<unsigned long> &y) {
    for (int i = 0; i < occurences.size(); ++i) {
        std::cout << x[occurences[i]] << ", " << y[occurences[i]] << "\n";
    }
}

int main() {
    Location location;
    Queue<Location> position(2);

    /*std::ifstream in("input.txt");
    Vector<unsigned long> pattern;
    Vector<unsigned long> z_function;
    Vector<unsigned long> strong_sp;
    Vector <unsigned long> text;
    Vector <unsigned long> x_coordinat;
    Vector <unsigned long> y_coordinat;
    unsigned long number;

    EnterPattern(in, pattern);
    EnterText(in, text, x_coordinat, y_coordinat);
    //std::cout << pattern;
    //std::cout << text;
    z_function = ComputeZfunction(pattern);
    //std::cout << z_function;
    strong_sp = ComputeStrongSP(z_function);
    //std::cout << strong_sp;
    KMP(pattern, text, strong_sp, x_coordinat, y_coordinat);
    in.close();
    return 0;*/
}

