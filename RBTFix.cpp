#include <iostream>
#include <string.h>
#include <fstream>
#include <chrono>


struct rbnode {
    char color;
    rbnode* left;
    rbnode* right;
    rbnode* parent;
    unsigned long long value;
    char* key;
    rbnode(char color, rbnode *left, rbnode* right, rbnode* parent, unsigned long long value, char* buffer) :
    color(color), left(left), right(right), parent(parent), value(value) {
        if (buffer != nullptr) {
            key = new char[strlen(buffer) + 1];
            key[strlen(buffer)] = 0;
            for(int i = 0; i < strlen(buffer);++i) {
                    key[i] = buffer[i];
                }
        }
     }
    rbnode(char color, rbnode* parent, unsigned long long value, char* load_key) :
    color(color), parent(parent), value(value), key(load_key) { } 

    ~rbnode() {
        delete[] key;
    }
};

class rbtree {

public:
    rbtree() {
        nil = new rbnode('b', nullptr, nullptr, nullptr, 0, nullptr);
        root = nil;
    }

    ~rbtree() {
        RemoveTree(root);
        //delete nil;
    }

    void Insert(char* key, unsigned long long value) { 
        Insert(key, value, root);
    }

    void DeleteNode(rbnode* finded) {
        RBTDelete(finded);
    }


    void Print() {
        show(root, 0);
    }

    rbnode* Find(char* key) {
        rbnode* finded = FindNode(key, root);
        if (finded == nil) {
            return nullptr;
        } else {
            return finded;
        }
    }

     void SaveDict(std::ofstream &outfile) {
        if (root == nil) {
            outfile.write("0", sizeof(char));
        } else {
            outfile.write("1", sizeof(char));
            Save(outfile, root);
        }
    }

    void LoadDict(std::ifstream &infile) {
        char is_empty;
        infile.read(&is_empty, 1);
        if ( is_empty == '0') {
            RemoveTree(root);
            root = nil;
        } else {
            RemoveTree(root);
            unsigned long long value;
            char color;
            char* key = LoadKey(infile);
            infile.read((char*)&value, sizeof(unsigned long long)); 
            infile.read(&color, 1); 
            this->root = new rbnode(color, nil, value, key);
            root->left = nil;
            root->right = nil;
            Load(infile, root);           
        }
    }

    void RemoveTree(){
        RemoveTree(root);
    }

    void ToLower(char* key) {
        ToLowerArray(key);
    }

private:
    rbnode* root;
    rbnode* nil;


    void RemoveTree(rbnode* node) {
        if (node != nil) {
            RemoveTree(node->left);
            RemoveTree(node->right);
            delete node;
        }
    }

    void LeftRotation(rbnode* nodeX) {
        rbnode* nodeY = nodeX->right;
        nodeX->right = nodeY->left;
        if (nodeY->left != nil) {
            nodeY->left->parent = nodeX;
        }
        nodeY->parent = nodeX->parent;
        if (nodeX->parent == nil) {
            root = nodeY;
        } else if (nodeX == nodeX->parent->left) {
            nodeX->parent->left = nodeY;
        } else {
            nodeX->parent->right = nodeY;
        }
        nodeY->left = nodeX;
        nodeX->parent = nodeY;
    }

    void RightRotation(rbnode* nodeY) {
        rbnode* nodeX = nodeY->left;
        nodeY->left = nodeX->right;
        if (nodeX->right != nil) {
            nodeX->right->parent = nodeY;
        }
        nodeX->parent = nodeY->parent;
        if (nodeY->parent == nil) {
            root = nodeX;
        } else if (nodeY == nodeY->parent->right) {
            nodeY->parent->right = nodeX;
        } else {
            nodeY->parent->left = nodeX;
        }
        nodeX->right = nodeY;
        nodeY->parent = nodeX;
    }

    void InsertFixUp(rbnode* node) { 
        while (node->parent->color == 'r') {
            if (node->parent == node->parent->parent->left) {

                rbnode* nodeUncle = node->parent->parent->right;
                if (nodeUncle->color == 'r') {
                    node->parent->color = 'b';
                    nodeUncle->color = 'b';
                    node->parent->parent->color = 'r';
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        LeftRotation(node);
                    }  
                    node->parent->color = 'b';
                    node->parent->parent->color = 'r';
                    RightRotation(node->parent->parent);
                }
            }
            else {
                rbnode* nodeUncle = node->parent->parent->left;
                
                if (nodeUncle->color == 'r') {
                    node->parent->color = 'b';
                    nodeUncle->color = 'b';
                    node->parent->parent->color = 'r';
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        RightRotation(node);
                    }
                    node->parent->color = 'b';
                    node->parent->parent->color = 'r';
                    LeftRotation(node->parent->parent);
                }
            }
        }
        root->color = 'b';
    }

    void Insert(char* key, unsigned long long value, rbnode* root) {
        if (root == nil) {
            this->root = new rbnode('b', nil, nil, nil, value, key);  
        } else if (strcmp(key, root->key) < 0) {
            if(root->left != nil){
			    Insert(key, value, root->left);
		    } else{
                root->left = new rbnode('r', nil, nil, root, value, key);
                InsertFixUp(root->left);
		    }
        } else if (strcmp(key, root->key) > 0){
            if(root->right != nil) {
			    Insert(key, value, root->right);
		    } else{
                root->right = new rbnode('r', nil, nil, root, value, key);
                InsertFixUp(root->right);
            }
        }
    }

    rbnode* FindNode(char* key, rbnode* node) {
        if (node == nil) {
            return nil;
        } else {
            if (strcmp(key, node->key) == 0) {
                return node;
            }
            else if (strcmp(key, node->key) < 0) {
                return FindNode(key, node->left);
            } else {
               return FindNode(key, node->right);
            }
        }
    }

    rbnode* minNode(rbnode* node) {
        if (node == nil) { return nil;}
        else if (node->left == nil) {
            return node;
        }
        else {
            return minNode(node->left);
        }
    } 

    void RBTDeleteRoot() {
        if (root != nil) {
            rbnode* delete_ptr = root;
            // 0 children
            if(root->left == nil && root->right == nil){
                root = nil;
                delete delete_ptr;
            } 
            // 1 child
            else if (root->left == nil && root->right != nil) {
                root = root->right;
                delete_ptr->right = nil;
                delete delete_ptr;
                root->color = 'b';
            }
            else if (root->right == nil && root->left != nil) {
                root = root->left;
                delete_ptr->left = nil;
                delete delete_ptr;
                root->color = 'b';
            }
            
        } else {
            return;
        }
    };

 void RBTDelete(rbnode* node) {
        if (node == nil) {
            return;
        }  else {
            rbnode* parent;
            rbnode* delPtr;
            parent = node->parent;

            if(node->left == nil && node->right == nil) { 
                if (node == root) {
                    RBTDeleteRoot();
                }
                else if (parent->left == node) {
                    if (parent->left->color == 'b') {RBTDeleteFixUp(parent->left);}
                    delete parent->left;
                    parent->left = nil;
                } else {
                    if (parent->right->color == 'b') {RBTDeleteFixUp(parent->right);}
                    delete parent->right;
                    parent->right = nil;
                }
                
            }
            else if (node->left != nil && node->right != nil) { 
                rbnode* min_node = minNode(node->right);
                char* min_node_key = min_node->key;
                min_node->key = nullptr;
                unsigned long long min_node_value = min_node->value;
                RBTDelete(min_node);
                delete[] node->key;
                node->key = min_node_key;
                node->value = min_node_value;
            }
            else {
                if (node == root) {
                    RBTDeleteRoot();
                } else {
                rbnode* child = (node->right != nil) ? node->right : node->left;      
                node == parent->left ?  parent->left = child : parent->right = child;
                child->parent = parent;
                if (node->color ==  'b') {RBTDeleteFixUp(child);} 
                delete node;
                }
            }
        }
    }
    
    
    void CaseSiblingRed(rbnode* node, rbnode*& sibling, char side) {
        sibling->color = 'b';
        node->parent->color = 'r';
        if (side == 'l') {
            LeftRotation(node->parent);
            sibling = node->parent->right;
        }
        else {
            RightRotation(node->parent);
            sibling = node->parent->left;
        }
    }

    void CaseSiblingBlack_1(rbnode*& node, rbnode* sibling) {
        sibling->color = 'r';   
        node = node->parent;
    }

    void CaseSiblingBlack_2(rbnode* node, rbnode*& sibling, char side) {
        sibling->color = 'r';
        if (side == 'l') {
            sibling->left->color = 'b';
            RightRotation(sibling); 
            sibling = node->parent->right;
        } else {
            sibling->right->color = 'b';
            LeftRotation(sibling); 
            sibling = node->parent->left;
        }
    }

    void CaseSiblingBlack_3(rbnode*& node, rbnode* sibling, char side) {
        sibling->color = node->parent->color;
        node->parent->color = 'b';
        if (side == 'l') {
            sibling->right->color = 'b';
            LeftRotation(node->parent);
        }
        else {
            sibling->left->color = 'b';
            RightRotation(node->parent);
        }
        node = root;
    }
    
    void RBTDeleteFixUp(rbnode* node) { 
        while (node != root && node->color == 'b') {

            rbnode* sibling;
            if (node == node->parent->left) {
                sibling = node->parent->right;
                if (sibling->color == 'r') {
                    // сводим к кейсам c черным братом 
                    CaseSiblingRed(node, sibling, 'l');
                }
                if (sibling->left->color == 'b' && sibling->right->color == 'b') {
                    CaseSiblingBlack_1(node, sibling);  
                }
                else {
                    if (sibling->right->color == 'b') {
                        CaseSiblingBlack_2(node, sibling, 'l');
                    }
                    CaseSiblingBlack_3(node, sibling, 'l');
                } 
            }
            else {
                sibling = node->parent->left;
                if (sibling->color == 'r') {
                    CaseSiblingRed(node, sibling, 'r');
                }
                if (sibling->left->color == 'b' && sibling->right->color == 'b') {
                   CaseSiblingBlack_1(node, sibling);  
                }
                else {
                    if (sibling->left->color == 'b') {
                        CaseSiblingBlack_2(node, sibling, 'r');
                    }
                    CaseSiblingBlack_3(node, sibling, 'r');
                } 
            }
        }
        node->color = 'b';
    }

     void Save(std::ofstream &outfile, rbnode* node) {
        outfile.write(node->key, strlen(node->key) + 1);
        outfile.write((char*)&node->value, sizeof(unsigned long long));
        outfile.write(&node->color, sizeof(char));
        if (node->left != nil) {
            outfile.write("1", sizeof(char));
            Save(outfile, node->left);
        } else {
            outfile.write("0", sizeof(char));
        }
        if (node->right != nil) {
            outfile.write("1", sizeof(char));
            Save(outfile, node->right);
        } else {
            outfile.write("0", sizeof(char));
        }     
    }

    char* LoadKey(std::ifstream &infile) {
        char* load_key;
        char character;
        char key[257];
        int count = 0;
        infile.read(&character, 1);
        while (character != 0) {
            key[count] = character;
            infile.read(&character, 1);
            count += 1;
        }
        key[count] = 0;
        load_key = new char[strlen(key) + 1];
        for (int i = 0; i < strlen(key); ++i) {
            load_key[i] = key[i];
        }
        load_key[strlen(key)] = 0;
        return load_key;
    }

    void Load(std::ifstream &infile, rbnode* node) {
        char is_empty;
        rbnode* create_node;
        unsigned long long value;
        char color;
        infile.read(&is_empty, 1);
        if (is_empty == '1') {
            unsigned long long value;
            char color;
            char* key = LoadKey(infile);
            infile.read((char*)&value, sizeof(unsigned long long));
            infile.read(&color, 1); 
            create_node = new rbnode(color, node, value, key);
            create_node->left = nil;
            create_node->right = nil;
            node->left = create_node;
            Load(infile, create_node);
        }
        infile.read(&is_empty, 1);
        if (is_empty == '1') {
            char* key = LoadKey(infile);
            infile.read((char*)&value, sizeof(unsigned long long)); 
            infile.read(&color, 1);
            create_node = new rbnode(color, node, value, key);
            create_node->left = nil;
            create_node->right = nil;
           // create_node = new rbnode(color, nil, nil, node, value, key);
            node->right = create_node;
            Load(infile, create_node);
        }
    }

    void PrintTab(int n) {
        for (int i = 0; i < n; ++i) {
            std::cout << "      ";
        }
    }

    void show(rbnode* t, int number) {
        if(t == nil)
            return;
        show(t->right, number + 1);
        PrintTab(number);
        std::cout << "(" << t->key << ":"<< t->color << " " <<t->value << ")\n";
        show(t->left, number + 1);
    }

    void ToLowerArray(char* key) {
        for (int i = 0; i < strlen(key); ++i) {
            key[i] = tolower(key[i]);
        }
    }
};

int main() {

    /*std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);*/
    rbtree t;
    double time;
    double result_time = 0;
    char buffer[257];
    unsigned long long value;
    rbnode* finded;
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    start = std::chrono::steady_clock::now();
    while (in >> buffer) {
        if (strcmp(buffer, "+") == 0) {
            in >> buffer;
            t.ToLower(buffer);
            in >> value;
            finded = t.Find(buffer);
            if (finded != nullptr) {
                out << "Exist\n";
            } else {
                t.Insert(buffer, value);
                out << "OK\n";
            }
        }
        else if (strcmp(buffer, "-") == 0) {
                in >> buffer;
                t.ToLower(buffer);
                finded = t.Find(buffer);
                if (finded == nullptr) {
                    out << "NoSuchWord\n";
                } else {
                    t.DeleteNode(finded);
                    out << "OK\n";
                }
        }
         else if (strcmp(buffer, "!") == 0) {
            in >> buffer;

            if (strcmp(buffer, "Save") == 0) {
                in >> buffer;
                std::ofstream outfile;
                outfile.open(buffer, std::ios::binary | std::ios::out | std::ios::trunc);
                t.SaveDict(outfile);
                out << "OK\n";
                outfile.close();
                //t.Print();
            }
            else {
                in >> buffer;
                std::ifstream infile;
                infile.open(buffer, std::ios::binary | std::ios::in);
                t.LoadDict(infile);
                out << "OK\n";
                infile.close();
            }
        }
        else {
            t.ToLower(buffer);
            finded = t.Find(buffer);
            if (finded == nullptr) {
                out << "NoSuchWord\n";
            } else {
                out << "OK: " << finded->value << "\n";
            }
        }
        //std::cout << "------------\n";
        //t.Print();
    }
    end = std::chrono::steady_clock::now();
    time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time: " << time << std::endl;
    in.close();
    out.close();
    return 0;
}


// rbnode* CopyParentRedCase(rbnode* original, rbnode*& copy_node) {

//         rbnode* new_node = new rbnode('b',
//                                       original->left,
//                                       original->right,
//                                       original->parent,
//                                       original->coordinate,
//                                       original->left_subtree_number,
//                                       original->duplicates);
        
//         if (original->coordinate > copy_node->coordinate) {
//             new_node->left = copy_node;
//             if (original->right != nil) {
//                 new_node->right = original->right;
//                 original->right->parent = new_node;
//             }
//         } 
//         else {
//             new_node->right = copy_node;
//             if (original->left != nil) {
//                 new_node->left = original->left;
//                 original->left->parent = new_node;
//             }
//         }

//         copy_node->parent = new_node;
//         return new_node;
//     }

//     rbnode* CopyUncleRedCase(rbnode*& original) {

//         rbnode* new_node = new rbnode('b',
//                                       original->left,
//                                       original->right,
//                                       original->parent,
//                                       original->coordinate,
//                                       original->left_subtree_number);
//         if (original->right != nil) {
//             original->right->parent = new_node;
//         }
//         if (original->left != nil) {
//             original->left->parent = new_node;
//         }
//         return new_node;
    
//     }

//     rbnode* CopyGrandPaRedCase(rbnode* original, rbnode*& parent, rbnode*& uncle) {
//         rbnode* grandpa = original->parent->parent;
//          rbnode* new_node = new rbnode('r',
//                                        original->left,
//                                        original->right,
//                                        original->parent,
//                                        original->coordinate,
//                                        original->left_subtree_number
//                                       );
//         if (original == original->parent->left) {
//             new_node->left = parent;
//             new_node->right = uncle;
//         }
//         else{
//             new_node->left = uncle;
//             new_node->right = parent;
//         }
//         uncle->parent = new_node;
//         parent->parent = new_node;

//         return new_node;
//     }


#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ostream& operator << (ostream &out, vector<pair<int, pair<int, int>>>& segments) {
    out << "[ ";
    for (int index = 0; index < segments.size(); ++index) {
        out << segments[index].first << " " << segments[index].second.first << " " << segments[index].second.second << ", ";
    }
    out << " ]\n";
    return out;
}

struct rbnode {
    char color;
    rbnode* left;
    rbnode* right;
    rbnode* parent;
    int coordinate;
    int left_subtree_number;
    int duplicates;
    rbnode(char color, rbnode *left, rbnode* right, rbnode* parent, int coordinate, int left_subtree_number):
        color(color), left(left), right(right), parent(parent), coordinate(coordinate), left_subtree_number(left_subtree_number),
        duplicates(0) {}

    rbnode(char color, rbnode *left, rbnode* right, rbnode* parent, int coordinate, int left_subtree_number, int duplicates):
        color(color), left(left), right(right), parent(parent), coordinate(coordinate), left_subtree_number(left_subtree_number),
        duplicates(duplicates) {}
};


class PersistentRBT {
public:
    PersistentRBT(int n) {
        nil = new rbnode('b', nullptr, nullptr, nullptr, 0, 0);
        root = nil;
        versions = vector<pair<int, rbnode*>>(n);
        version_index = 0;
    }

    ~PersistentRBT() {
        //delete nil;
    }

    void PrintCertainVersion(int v) {
        rbnode* vroot = versions[v].second;
        Show(vroot, 0);
    }

    void Print() {
        Show(root, 0);
    }

    void Insert(pair<int, int> key) {
        cout << "Versions:  ";
        if (root != nil) {
            cout << versions[version_index - 1].first << " " <<  versions[version_index - 1].second->coordinate << " " <<
            versions[version_index - 1].second << "\n";
        }
        cout << "\n";
        Insert(key, root);
    }

    rbnode* GetRoot() {
        return this->root;
    }

private:
    rbnode* nil;
    rbnode* root;
    vector<pair<int, rbnode*>> versions;
    int version_index;

    void LeftRotation(rbnode* nodeX) {
        rbnode* nodeY = nodeX->right;
        nodeX->right = nodeY->left;
        if (nodeY->left != nil) {
            nodeY->left->parent = nodeX;
        }
        nodeY->parent = nodeX->parent;
        if (nodeX->parent == nil) {
            root = nodeY;
        }
        nodeY->left = nodeX;
        nodeX->parent = nodeY;
    }

    void RightRotation(rbnode* nodeY) {
        rbnode* nodeX = nodeY->left;
        nodeY->left = nodeX->right;
        if (nodeX->right != nil) {
            nodeX->right->parent = nodeY;
        }
        nodeX->parent = nodeY->parent;
        if (nodeY->parent == nil) {
            root = nodeX;
        }
        nodeX->right = nodeY;
        nodeY->parent = nodeX;
    }


    void CopyNodePassToRoot(rbnode* node, rbnode* parent) {
        rbnode* new_parent = new rbnode(parent->color, parent->left, parent->right, parent->parent,
                                        parent->coordinate, parent->left_subtree_number, parent->duplicates);

        if (node->coordinate < parent->coordinate) {
            new_parent->right = parent->right;
            parent->right->parent = new_parent;
            new_parent->left = node;
        }
        else {
            new_parent->left = parent->left;
            parent->left->parent = new_parent;
            new_parent->right = node;
        }

        node->parent = new_parent;
    }

    rbnode* CopyPathToRoot(rbnode* node) {
        while (node->parent != nil) {
            CopyNodePassToRoot(node, node->parent);
            node=node->parent;
        }
        return node; // возращаю новый корень
    }

    void Insert(pair<int, int>& heigth_and_coordinate, rbnode* root) {
        if (root == nil) {
            this->root = new rbnode('b', nil, nil, nil, heigth_and_coordinate.second, 0);
            versions[version_index] = {heigth_and_coordinate.first, this->root};
            ++version_index; 
        }
        else if (heigth_and_coordinate.second < root->coordinate) {
            if(root->left != nil) {
			    Insert(heigth_and_coordinate, root->left);
		    } else {
                rbnode* fictitous_node = root->left;
                fictitous_node = new rbnode('r', nil, nil, root, heigth_and_coordinate.second, 0);
                rbnode* current_node = PersistentInsertFixUp(fictitous_node);
                rbnode* new_root = CopyPathToRoot(current_node);
                this->root = new_root;
                this->root->color = 'b';
                versions[version_index] = {heigth_and_coordinate.first, new_root};
                ++version_index;
		    }
        }
        else if (heigth_and_coordinate.second > root->coordinate){
            if(root->right != nil) { 
			    Insert(heigth_and_coordinate, root->right);
		    } else{
                rbnode* fictitous_node = root->right;
                fictitous_node = new rbnode('r', nil, nil, root, heigth_and_coordinate.second, 0);
                rbnode* current_node = PersistentInsertFixUp(fictitous_node);
                rbnode* new_root = CopyPathToRoot(current_node);
                this->root = new_root;
                this->root->color = 'b';
                versions[version_index] = {heigth_and_coordinate.first, new_root};
                ++version_index;
            }
        }
        else {
            root->duplicates += 1;
        }
    }

    rbnode* PersistentInsertFixUp(rbnode*& copy_son) { 

        while (copy_son->parent->color == 'r' && copy_son->color == 'r') {
            if (copy_son->parent == copy_son->parent->parent->left) {
                rbnode* nodeUncle = copy_son->parent->parent->right;
                if (nodeUncle->color == 'r') {
                    CopyFixUpRedCase(copy_son);
                    copy_son = copy_son->parent->parent;
                } else {
                    rbnode* alpha = copy_son->right;
                    rbnode* beta = copy_son->left;
                    rbnode* gamma = copy_son->parent->right;

                    rbnode* new_parent = new rbnode(copy_son->parent->color,
                                                    copy_son->parent->left,
                                                    copy_son->parent->right,
                                                    copy_son->parent->parent,
                                                    copy_son->parent->coordinate,
                                                    copy_son->parent->left_subtree_number,
                                                    copy_son->parent->duplicates);

                    rbnode* new_grandpa = new rbnode(copy_son->parent->parent->color,
                                                    new_parent,
                                                    copy_son->parent->parent->right,
                                                    copy_son->parent->parent->parent,
                                                    copy_son->parent->parent->coordinate,
                                                    copy_son->parent->parent->left_subtree_number,
                                                    copy_son->parent->parent->duplicates);

                    copy_son->parent = new_parent;
                    new_parent->parent = new_grandpa;

                    if (copy_son->coordinate > copy_son->parent->coordinate) {
                        new_parent->right = copy_son;
                        rbnode* gamma = copy_son->parent->left;

                        // copy_son->left_subtree_number = new_grandpa->left_subtree_number + 

                        copy_son = copy_son->parent;
                        LeftRotation(copy_son);
                        new_grandpa->left= copy_son->parent;
                    }
                    else {
                        new_parent->left = copy_son;
                    }  
                    copy_son->parent->color = 'b';
                    copy_son->parent->parent->color = 'r';
                    RightRotation(copy_son->parent->parent);

                    copy_son = copy_son->parent;

                    alpha->parent = alpha == nil ? nullptr : copy_son->left;
                    beta->parent = beta == nil ? nullptr: copy_son->right;
                    gamma->parent = gamma == nil ? nullptr : new_grandpa;

                    nodeUncle->parent = nodeUncle == nil ? nullptr : copy_son->right;
                }
            }
            else {
                rbnode* nodeUncle = copy_son->parent->parent->left;
                
                if (nodeUncle->color == 'r') {
                    CopyFixUpRedCase(copy_son);
                    copy_son = copy_son->parent->parent;
                } else {
                    rbnode* alpha = copy_son->left;
                    rbnode* beta = copy_son->right;
                    rbnode* gamma = copy_son->parent->left;

                    rbnode* new_parent = new rbnode(copy_son->parent->color,
                                                    copy_son->parent->left,
                                                    copy_son->parent->right,
                                                    copy_son->parent->parent,
                                                    copy_son->parent->coordinate,
                                                    copy_son->parent->left_subtree_number,
                                                    copy_son->parent->duplicates);

                    rbnode* new_grandpa = new rbnode(copy_son->parent->parent->color,
                                                    copy_son->parent->parent->left,
                                                    new_parent,
                                                    copy_son->parent->parent->parent,
                                                    copy_son->parent->parent->coordinate,
                                                    copy_son->parent->parent->left_subtree_number,
                                                    copy_son->parent->parent->duplicates);

                    copy_son->parent = new_parent;
                    new_parent->parent = new_grandpa;

                    if (copy_son->coordinate < copy_son->parent->coordinate) {
                        new_parent->left = copy_son;
                        rbnode* gamma = copy_son->parent->right;
                        copy_son = copy_son->parent;
                        RightRotation(copy_son);
                        new_grandpa->right = copy_son->parent;

                    }
                    else {
                        new_parent->right = copy_son;
                    }  
                    copy_son->parent->color = 'b';
                    copy_son->parent->parent->color = 'r';
                    LeftRotation(copy_son->parent->parent);

                    copy_son = copy_son->parent;   // это теперь вершина апосля поворота

                    alpha->parent = alpha == nil ? nullptr : copy_son->left;
                    beta->parent = beta == nil ? nullptr: copy_son->right;
                    gamma->parent = gamma == nil ? nullptr : copy_son->right;
                    nodeUncle->parent = nodeUncle == nil ? nullptr : copy_son->left;             
                }
            }
        }
        return copy_son;
    }
    

    void CopyFixUpRedCase(rbnode* original) {
        rbnode* original_parent = original->parent;
        rbnode* original_grandpa = original->parent->parent;
        rbnode* original_uncle = original_parent == original_grandpa->left ? original_grandpa->right : original_grandpa->left;

        // создала отца, кинула ссылку отца на ноду и ноду на нового отца + всех детей
        rbnode* new_parent = new rbnode('b',
                                        original_parent->left,
                                        original_parent->right,
                                        original_parent->parent,
                                        original_parent->coordinate,
                                        original_parent->left_subtree_number,
                                        original_parent->duplicates);

        if (original->coordinate < original_parent->coordinate) {
            new_parent->left_subtree_number += 1;
            new_parent->left = original;
            new_parent->right = original_parent->right;
            if (original_parent->right != nil) {
                original_parent->right->parent = new_parent;
            }
        }
        else {
            original->left_subtree_number += 1;
            new_parent->left = original_parent->left;
            if (original_parent->left != nil) {
                original_parent->left->parent = new_parent;
            }
        }
        original->parent = new_parent;

        // копирую дядю
        rbnode* new_uncle = new rbnode('b',
                                      original_uncle->left,
                                      original_uncle->right,
                                      original_uncle->parent,
                                      original_uncle->coordinate,
                                      original_uncle->left_subtree_number,
                                      original_uncle->duplicates);

        if (original_uncle->right != nil) {
            original_uncle->right->parent = new_uncle;
        }
        if (original_uncle->left != nil) {
            original_uncle->left->parent = new_uncle;
        }

        // копирую деда
        rbnode* new_grandpa = new rbnode('r',
                                      original_grandpa->left,
                                      original_grandpa->right,
                                      original_grandpa->parent,
                                      original_grandpa->coordinate,
                                      original_grandpa->left_subtree_number,
                                      original_grandpa->duplicates);

        if (original_parent == original_grandpa->left) {
            new_grandpa->left_subtree_number += 1;
            new_grandpa->left = new_parent;
            new_grandpa->right = new_uncle;
        } 
        else {
            new_grandpa->right = new_parent;
            new_grandpa->left = new_uncle;
        }
        new_uncle->parent = new_grandpa;
        new_parent->parent = new_grandpa;

    }

    void PrintTab(int n) {
        for (int i = 0; i < n; ++i) {
            std::cout << "      ";
        }
    }

    void Show(rbnode* t, int number) {
        if(t == nil)
            return;
        Show(t->right, number + 1);
        PrintTab(number);
        std::cout << "(" << t->color << ":" << t->coordinate << ", "
        << t->left_subtree_number << ", " << t->duplicates <<  " )\n";
        Show(t->left, number + 1);
    }
};




int main() {
    int n, m;
    int segment_height, segment_start, segment_end;

    ifstream in("input.txt");
    ofstream out("output.txt");

    in >> n >> m;

    PersistentRBT ltree(n);
    PersistentRBT rtree(n);

    vector<pair<int, pair<int, int>>> segments(n);

    for (int index_segment = 0; index_segment < n; ++index_segment) {
        in >> segment_start >> segment_end >> segment_height;
        segments[index_segment].first = segment_height;
        segments[index_segment].second.first = segment_start;
        segments[index_segment].second.second = segment_end;
    }

    sort(segments.begin(), segments.end(), greater<>());
    cout << segments << "\n";

    for (int i = 0; i < segments.size(); ++i) {
        int h = segments[i].first;
        int x = segments[i].second.first;
        int y = segments[i].second.second;
        cout << " Insertion: " << x << "\n";
        ltree.Insert({h, x});
        rtree.Insert({h, y});
        ltree.Print();
        cout << "--------------------\n";
        rtree.Print();
        cout << "--------------------\n";
    }

    return 1;
}