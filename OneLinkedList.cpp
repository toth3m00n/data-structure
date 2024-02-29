#include <iostream>

template <class T>
struct Node {
    T data;
    Node* Next;
    Node(T data = T(), Node* Next = nullptr) : data(data), Next(Next) { };
    ~Node() {};
};

template <class T>
class OneLinkedList {
    Node<T>* head;
    int Size;

public:

    OneLinkedList() {
        Size = 0;
        head = nullptr;
    }

    OneLinkedList(T data) {
        Size = 1;
        head->data = data;
        head->Next = nullptr;
    }

    ~OneLinkedList() {
        while (head != nullptr) {
            Node<T>* current = head;
            head = head->Next;
            delete current;
        }
    }

    void push_back(T data) {
        if (head == nullptr) {
            head = new Node<T>(data);
        }
        else {
            Node<T>* current = head;
            while (current->Next != nullptr) {
                current = current->Next;
            }
            current->Next = new Node<T>(data);
        }
        ++Size;
    }

    void push_front(T data) {
        Node<T>* current = new Node<T>(data);
        current->Next = head;
        head = current;
        ++Size;
    }

    void pop_front() {
        Node<T>* templ = head;
        head = head->Next;
        delete templ;
        --Size;
    }

    void pop_back(){
        if (head == nullptr) { return;}
        else if (head->Next == nullptr) {
            delete head;
            head = nullptr;
        }
        else {
            Node<T>* templ = head;
            Node<T>* previous = head;
            while (templ->Next != nullptr) {
                previous = templ;
                templ = templ->Next;
            }
            delete templ;
            previous->Next = nullptr;
        }
        --Size;
    }

    void insert(T data, const int index) {
        if (index > Size) {
            std::cout << "Index out of range\n";
            return;
        }
        else if (head == nullptr || index == 0) {
            push_front(data);
            ++Size;
        }
        else {
            Node<T>* insert_node = new Node<T>(data);
            Node<T>* left = head;
            Node<T>* rigth = head;
            int counter = 0;
            while (counter != index){
                left = rigth;
                rigth = rigth->Next;
                ++counter;
            }
            left->Next = insert_node;
            insert_node->Next = rigth;
            ++Size;
        }
    }

    T& operator[](const int index) {
        Node<T>* current = head;
        int counter = 0;
        while (current != nullptr && counter != index) {
            current = current->Next;
            ++counter;
        }
        return current->data;
    }

    int GetSize() {
        return Size;
    }

    void Print() {
        Node<T>* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->Next;
        }
        std::cout << "\n";
    }
};

int main() {
    OneLinkedList<int> list;

    list.push_back(1);
    list.push_back(2);
    //list.push_back(3);
    //list.push_back(4);
    //list.push_back(5);

    list.Print();
    list.Print();

    return 0;
}

