#include <vector>
#include <iostream>

using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};

// что умеет дерево?
// добавление узла, удаление узла (правильное), kill, печать дерева, удаление дерева pingall()

class BinaryTree {
    

    Node* root;


    Node* insert(Node* root, int value) {
        if (root == NULL) {
            root = new Node;
            root->data = value;
            root->left = root->right = NULL;
        } else if (value < root->data) {
            root->left = insert(root->left, value);
        } else if (value > root->data) {
           root->right = insert(root->right, value);
        }
        return root;
    }


    Node* clean(Node* node){
        if (node == NULL) {
            return NULL;
        } else {
            clean(node->left);
            clean(node->right);
            delete node;
        }
        return NULL;
    }


    Node* minNode(Node* node) {
        if (node == NULL) { return NULL;}
        else if (node->left == NULL) {
            return node;
        } else {
           return minNode(node->left);
        }
    }


    Node* remove(Node* node, int value) {
        Node* tempraryNode = new Node;

        if (node == NULL) { return NULL;}
        else if (node->data < value) { 
            node->right = remove(node->right, value);
        } 
        else if (node->data > value) {
            node->left = remove(node->left, value);
        } 
        else if ((node->right != NULL) && (node->left != NULL)) {
            tempraryNode = minNode(node->right);
            std::cout << " Temp1: " << tempraryNode->data << endl;
            node->data = tempraryNode->data;
            node->right = remove(node->right, node->data);
        } else {
            tempraryNode = node;
            std::cout << " Temp: " << tempraryNode->data << endl;
            if (node->left == NULL) { node = node->right;}
            else if (node->right == NULL) { node = node->left;}
            delete tempraryNode;
        }
        return node;
    }

    void show1(Node* t, int number) {
        if(t == NULL)
            return;
        show1(t->right, number + 1);
        PrintTab(number);
        cout << t->data << endl;
        show1(t->left, number + 1);
    }

    void show(Node* t, int number) {
        if(t == NULL)
            return;
        show(t->right, number + 1);
        //PrintTab(number);
        cout << t->data << endl;
        show(t->left, number + 1);
    }

    void PrintTab(int n)
    {
        for (int i = 0; i < n; ++i) {
            cout << "      ";
        }
    }

    Node* kill(Node* node, int killValue) {
        if (node == NULL) {
            return NULL;
        }
        else if (killValue < node->data) {
            node->left = kill(node->left, killValue);
        } else if (killValue > node->data) {
           node->right = kill(node->right, killValue);
        } else {
            return NULL;
        }
       return node; 
    }

    vector<int> gettingNode(Node* node) {
        vector<int> aliveNodes;
        if (node == NULL) {
            return vector<int>();
        }
        if (node->right != NULL) {
            vector<int> rightNodes = gettingNode(node->right);
            for (int everynode : rightNodes) {
                aliveNodes.push_back(everynode);
            }
        } 
        if (node->left != NULL) {
            vector<int> leftNodes = gettingNode(node->left);
            for (int everynode : leftNodes) {
                aliveNodes.push_back(everynode);
            }
        }
        aliveNodes.push_back(node->data);
        return aliveNodes; 
    }

public:  

    BinaryTree() {
        root = NULL;
    }

    ~BinaryTree() {
        root = clean(root);
    }

    void insert(int nodeValue) {
        root = insert(root, nodeValue);
    }

    void display() {
        show(root, 0);
        cout << "_______________________________________\n";
        show1(root, 0);
    }

    void remove(int nodeValue) {
        root = remove(root, nodeValue);
    }

    void kill(int nodeValue) {
        root = kill(root, nodeValue);
    }

    void gettingNodes() {
       vector<int> aliveNode = gettingNode(root);
        for (int everynode : aliveNode) {
            cout << everynode << " ";
        }
        cout << endl;
    }

};

int main() {
    BinaryTree t;
    t.insert(20);
    t.insert(25);
    t.insert(15);
    t.insert(10);
    t.insert(30);
    t.insert(17);
    t.insert(24);
    //t.display();
    t.remove(15);
    //t.kill(15);
    t.display();
    return 0;
}