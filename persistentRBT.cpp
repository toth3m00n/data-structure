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

    void PrintVersions() {
        int ii = 0;
        for (auto i: versions) {
            cout << ii << " " << i.first << " ," << i.second << " | ";
            ++ii;
        }
        cout << "\n";
    }

    void PrintCertainVersion(int v) {
        rbnode* vroot = versions[v].second;
        Show(vroot, 0);
    }

    vector<pair<int, rbnode*>> GetVectorVersions() {
        return versions;
    }

    void Print() {
        Show(root, 0);
    }

    void Insert(pair<int, int> key) {
        Insert(key, root);
    }

    rbnode* GetRoot() {
        return this->root;
    }

    int SearchCountStart(int coordinate, int version_index) {
        rbnode* right_version_root = versions[version_index].second;
        int count = SearchNodeStart(right_version_root, coordinate, 0);
        return count;
    }

    int SearchCountEnd(int coordinate, int version_index) {
        rbnode* right_version_root = versions[version_index].second;
        int res_end = SearchNodeEnd(right_version_root, coordinate, 0);
        return res_end;
    }


private:
    rbnode* nil;
    rbnode* root;
    vector<pair<int, rbnode*>> versions;
    int version_index;

    void LeftRotation(rbnode* nodeX) {
        rbnode* nodeY = nodeX->right;
        nodeX->right = nodeY->left;

        nodeY->left_subtree_number += nodeX->left_subtree_number + nodeX->duplicates + 1;

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

        nodeY->left_subtree_number -= (nodeX->left_subtree_number + nodeX->duplicates);
        --nodeY->left_subtree_number;

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
            new_parent->left_subtree_number += 1;

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
            rbnode* duplicate = new rbnode(root->color, root->left, root->right, 
                                           root->parent, root->coordinate, root->left_subtree_number, 
                                           root->duplicates+1);
            duplicate->left->parent = duplicate->left== nil ? nullptr : duplicate;
            duplicate->right->parent = duplicate->right == nil ? nullptr : duplicate;
            rbnode* new_root = CopyPathToRoot(duplicate);
            this->root = new_root;
            versions[version_index] = {heigth_and_coordinate.first, new_root};
            ++version_index;
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

                    new_grandpa->left_subtree_number += 1;

                    if (copy_son->coordinate > copy_son->parent->coordinate) {
                        // copy_son->left_subtree_number += new_parent->left_subtree_number + 1;

                        new_parent->right = copy_son;

                        copy_son = copy_son->parent;
                        LeftRotation(copy_son);
                        new_grandpa->left= copy_son->parent;
                    }
                    else {
                        new_parent->left_subtree_number += 1;
                        new_parent->left = copy_son;
                    }

                    // new_grandpa->left_subtree_number -= (copy_son->parent->left_subtree_number + 1);

                    copy_son->parent->color = 'b';
                    copy_son->parent->parent->color = 'r';
                    RightRotation(copy_son->parent->parent);

                    copy_son = copy_son->parent;

                    copy_son->left->left->parent = copy_son->left->left == nil ? nullptr : copy_son->left;
                    copy_son->left->right->parent = copy_son->left->right == nil ? nullptr : copy_son->left;

                    copy_son->right->left->parent = copy_son->right->left == nil ? nullptr : copy_son->right;
                    copy_son->right->right->parent = copy_son->right->right == nil ? nullptr : copy_son->right;

                }
            }
            else {
                rbnode* nodeUncle = copy_son->parent->parent->left;
                
                if (nodeUncle->color == 'r') {
                    CopyFixUpRedCase(copy_son);
                    copy_son = copy_son->parent->parent;
                } else {

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
                        // new_parent->left_subtree_number -= copy_son->left_subtree_number;
                        new_parent->left_subtree_number +=1;

                        new_parent->left = copy_son;
                        copy_son = copy_son->parent;
                        RightRotation(copy_son);
                        new_grandpa->right = copy_son->parent;

                    }
                    else {
                        new_parent->right = copy_son;
                    }

                    // copy_son->parent->left_subtree_number += new_grandpa->left_subtree_number + 1;

                    copy_son->parent->color = 'b';
                    copy_son->parent->parent->color = 'r';
                    LeftRotation(copy_son->parent->parent);

                    copy_son = copy_son->parent;   // это теперь вершина апосля поворота

                    copy_son->left->left->parent = copy_son->left->left == nil ? nullptr : copy_son->left;
                    copy_son->left->right->parent = copy_son->left->right == nil ? nullptr : copy_son->left;

                    copy_son->right->left->parent = copy_son->right->left == nil ? nullptr : copy_son->right;
                    copy_son->right->right->parent = copy_son->right->right == nil ? nullptr : copy_son->right;           
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
            new_parent->right = original;
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

    int SearchNodeStart(rbnode* node, int coordinate, int count) {
        if (node == nil) {
            return count;
        }
        else if (coordinate < node->coordinate) {
            count = SearchNodeStart(node->left, coordinate, count);
            return count;
        }
        else if (coordinate > node->coordinate) {
            count += node->left_subtree_number + node->duplicates + 1;
            count = SearchNodeStart(node->right, coordinate, count);
            return count;
            }
        else {
            count+= node->left_subtree_number + node->duplicates + 1;
            return count;
        }
    }

    int SearchNodeEnd(rbnode* node, int coordinate, int count) {
        if (node == nil) {
            return count;
        }
        else if (coordinate < node->coordinate) {
            count = SearchNodeEnd(node->left, coordinate, count);
            return count;
        }
        else if (coordinate > node->coordinate) {
            count += node->left_subtree_number + node->duplicates + 1;
            count = SearchNodeEnd(node->right, coordinate, count);
            return count;
            }
        else {
            count += node->left_subtree_number;
            return count;
        }
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
    // cout << segments << "\n";

    for (int i = 0; i < segments.size(); ++i) {
        int h = segments[i].first;
        int x = segments[i].second.first;
        int y = segments[i].second.second;
        // cout << " Insertion: " << x << "\n";
        ltree.Insert({h, x});
        rtree.Insert({h, y});
        // ltree.Print();
        // cout << "--------------------\n";
        // rtree.Print();
        // cout << "--------------------\n";
    }

    // ltree.PrintVersions();
    // cout << "\n";

    vector<pair<int, rbnode*>> versions_start = ltree.GetVectorVersions();
    vector<pair<int, rbnode*>> versions_end = rtree.GetVectorVersions();

    // ltree.PrintVersions();
    // rtree.PrintVersions();
    // cout << "\n";

    int dot_x, dot_y;
    rbnode* test = new rbnode('r', nullptr, nullptr, nullptr, 0, 0);
    for (int dot_index = 0; dot_index < m; ++dot_index) { 
        in >> dot_x >> dot_y;
        int best_version_index = lower_bound(versions_start.begin(), versions_start.end(), pair<int, rbnode*>(dot_y, nullptr),
        [](const pair<int, rbnode*>& a, const pair<int, rbnode*>& b){return a.first > b.first;}) - versions_start.begin();
        if (best_version_index == 0) {
            out << "0\n";
            continue;
        }
        --best_version_index;
        // ltree.PrintCertainVersion(best_version_index);
        // // cout << "------------------------------\n";
        // rtree.PrintCertainVersion(best_version_index);
        // cout << "------------------------------\n";
        int start = ltree.SearchCountStart(dot_x, best_version_index);
        int end = rtree.SearchCountEnd(dot_x, best_version_index);
        out << start - end << "\n";
    }

    return 0;
}

// 9 0
// -5 4 9
// 8 12 8
// 5 3 7
// 1 5 6
// 7 8 5
// 2 3 4
// -2 5 3
// 0 1 2
// -3 4 1

//             (b:20, 2, 2 )
//                   (r:19, 0, 1 )
//       (b:18, 5, 3 )
//             (b:17, 1, 3 )
//                   (r:16, 0, 0 )
// (b:14, 27, 3 )
//                         (r:13, 0, 2 )
//                   (b:12, 1, 0 )
//                         (r:11, 0, 0 )
//             (r:10, 6, 1 )
//                         (r:9, 0, 2 )
//                   (b:8, 0, 2 )
//       (b:7, 9, 4 )
//                   (b:6, 0, 1 )
//             (r:5, 6, 0 )
//                         (r:4, 0, 0 )
//                   (b:2, 3, 1 )
//                         (r:1, 0, 2 )