#include <iostream>

template <class T>
struct Node {
    T data;
    Node* next;
    Node(T data = T(), Node* next = nullptr) : data(data), next(next) {};
};

template <class T>
class Stack {
    Node<T>* head;

public:
    Stack() {
        head = nullptr;
    }

    Stack(T data) {
        head->data = data;
        head->next = nullptr; // maybe work without it -- check default value
    }

    ~Stack() {
        while (head != nullptr) {
            Node<T>* current = head;
            head = head->next;
            delete current;
        }
    }

    void push(T data) {
        if (head == nullptr) {
            head = new Node(data);
        } else {
            Node<T>* append_element = new Node(data);
            append_element->next = head;
            head = append_element;
        }
    }
    
    T pop() {
        T returned_data = head->data;
        if (head != nullptr) {
            Node<T>* pnt_head = head;
            head = head->next;
            delete pnt_head; 
            return returned_data;
        }
        else {
            std::cout << "Stack is empty, silly\n";
        }
    }

    T peek() {
        if (head != nullptr) {
            return head->data;
        } else {
            std::cout << "Stack is empty, silly\n";
            exit(1);
        }
    }

    void display() {
        Node<T>* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << "\n";
    }
};

int main() {
    Stack<int> first;

    for (int i = 0; i < 2; ++i) {
        first.push(i);
    }

    first.display();
    int head = first.peek();
    std::cout << "Head: " << head << "\n";

    head = first.pop();
    std::cout << "Head: " << head << "\n";
    first.display();
    
    head = first.pop();

    first.display();
    return 0;
}