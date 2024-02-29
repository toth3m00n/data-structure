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

    ~Queue() {
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