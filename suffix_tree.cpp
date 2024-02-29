#include <vector>
#include <iostream>
#include <fstream>
#include <string> 
#include <algorithm>
#include <map>

using namespace std;


struct SuffixNode{
    int start, leaf_index;
    int* end;
    SuffixNode* suffix_link;
    map<char, SuffixNode*> child; 

    //для листьев
    SuffixNode(int start, int* end, SuffixNode* suffix_link, int leaf_index) : 
        start(start), end(end), suffix_link(suffix_link), leaf_index(leaf_index) {};

    int edje_lenght() {
        return *end - start + 1;
    }
};

class SuffixTree {
    SuffixNode* root = new SuffixNode(-1, new int(-1), nullptr, -1);
    SuffixNode* active_node = nullptr;
    int active_lenght = 0;
    int active_edge = 0;
    int remains_suffix = 0;
    int leaf_end = -1;
    string text;

    void add_vertex(int index, int remains_suffix) {

        auto node_child =  active_node->child;
        auto vertex_start_index = active_node->child[text[active_edge]]->start; 

        SuffixNode* new_symbol = new SuffixNode(index, &leaf_end, nullptr, index - remains_suffix + 2);

        //оставшийся суффикс 
        SuffixNode* rest_suffix = node_child[text[active_edge]];
        rest_suffix->start = vertex_start_index + active_lenght;

        //проспличенная вершина
        SuffixNode* original_suffix = new SuffixNode(vertex_start_index, new int(vertex_start_index + active_lenght - 1), nullptr, -1);

        active_node->child[text[active_edge]] = original_suffix;
        original_suffix->child.insert({text[rest_suffix->start], rest_suffix});
        original_suffix->child.insert({text[index], new_symbol});
    }

    void ExtendSuffixTree(int index) {
        ++leaf_end;
        ++remains_suffix;

        //созданная на предыдущем шаге вершина
        SuffixNode* last_vertex = nullptr;
        
        // пока не возведем деерво для текущего суффикса
        while (remains_suffix != 0) {

            //на каждой фазе определяем ребро, с которым работаем
            if (active_lenght == 0) {
                active_edge = index;
            }

            auto find_node = active_node->child.find(text[active_edge]);

            if (find_node == active_node->child.end()) {

                // вставка листьев - новых символов, которые ещё не встречались
                active_node->child.insert({text[index], new SuffixNode(index, &leaf_end, nullptr, index - remains_suffix + 2)});

                //в случае если был создан лист из текущей внутренней вершины и эта вершина не корень, потому что я не кидаю ссылки в корень
                if (active_node != root) {
                    if ( last_vertex != nullptr && active_node->suffix_link == nullptr) {
                        last_vertex->suffix_link = active_node;
                    }
                    last_vertex = active_node;
                }


            } else {

                auto start_vertex_index = active_node->child[text[active_edge]]->start;
                    //переход по счетчику
                    if (active_lenght > active_node->child[text[active_edge]]->edje_lenght()) {
                        active_node = active_node->child[text[active_edge]];
                        active_lenght -= active_node->edje_lenght();
                        active_edge += active_node->edje_lenght();

                        if (active_node != root && last_vertex != nullptr) {
                            last_vertex->suffix_link = active_node;
                        }
                        continue;
                    }

                    // при добавлении буквы из текста если обновляем активную вершину
                    else if (active_lenght == active_node->child[text[active_edge]]->edje_lenght()) {
                        active_node =  active_node->child[text[active_edge]];
                        active_lenght = 0;

                        if (active_node != root && last_vertex != nullptr) {
                            last_vertex->suffix_link = active_node;
                        }
                        continue;
                    }
                    
                    if (text[index] == text[start_vertex_index + active_lenght]) {
                        ++active_lenght;
                        break;
                    }

                    // вставка нелистовой вершины
                    add_vertex(index, remains_suffix);

                    if (last_vertex != nullptr) {
                            last_vertex->suffix_link = active_node->child[text[active_edge]];
                        }

                    last_vertex = active_node->child[text[active_edge]];
                }

                // rool 1 - вставка из корня           
                // но только если активная длина > 0 - тк при вставке напрямую и так все вставили 
                if (active_node == root && active_lenght > 0) {
                    --active_lenght;
                    active_edge += 1;
                } 
                else if (active_node != root) {

                    //если нет суффиксных ссылок
                    if (active_node->suffix_link == nullptr) {
                        active_node = root;
                    } 
                    //jump to next station shibuya
                    else {
                        active_node = active_node->suffix_link;
                    }
                }
                --remains_suffix;
        }
    }

    void PrintSuffixTree(SuffixNode* start, int level) {
        for(auto children : start->child){
            if (children.first == '\0') continue;
            cout << "start: " << children.second->start << " ";
            for(int space = 0; space < level; ++space) {
                cout << "---";
            }
            // вывод суффикса
            for(int suffix_index = children.second->start; suffix_index <= *(children.second->end); ++suffix_index){
                cout << text[suffix_index];
            }

            cout << "\t ind: " << children.second->leaf_index << "  ";
            cout << "\t sufl: ";
            if (children.second->suffix_link != nullptr) {
                cout << children.second->suffix_link->start;
            } else { cout << " ";}

            cout << endl;

            if(!(start->child.empty())){
                PrintSuffixTree(children.second, level + 1);
            }
        }
    }

    SuffixNode* FindNodePattern(string pattern) {
        if (text.empty()) return 0;
        active_node = root;
        int letter_index = 0;
        while (letter_index <= pattern.size()) {
            auto finded = active_node->child.find(pattern[letter_index]);
            if (finded == active_node->child.end()) {
                return nullptr;
            }

            for (int edje_index = finded->second->start; edje_index <= *finded->second->end; ++edje_index) {
                if (letter_index >= pattern.size()) {
                    return finded->second;
                }
                if (text[edje_index] != pattern[letter_index]) { return nullptr;}
                ++letter_index;
            }
            if (letter_index >= pattern.size()) {
                return finded->second;
            }
            active_node = finded->second;
        }
        return nullptr;
    }

    void Dfs(vector<int>& answer, SuffixNode* node) {
        if (node->leaf_index != -1) {
            answer.push_back(node->leaf_index);
        }
        else {
            for (auto child: node->child) {
                Dfs(answer, child.second);
            }
        }
    }


    void PrintArrayEntries(vector<int>& array) {
        sort(array.begin(), array.end());
        for (int i = 0; i < array.size(); i++) {
                if (i != 0) {
                    std::cout << ", ";
                }
                std::cout << array[i];
            }
    }

public:
    SuffixTree(string text) : text(text + '$') {};

    void BuildSuffixTree() {
        active_node = root;
        for (int index_letter = 0; index_letter < text.size(); ++index_letter) {
            ExtendSuffixTree(index_letter);
        }
    }

    void Print() {
        PrintSuffixTree(root, 1);
    }

    void AllEntrance (int number_pattern, string pattern) {
        SuffixNode* finded_node = FindNodePattern(pattern);
        if (finded_node != nullptr) {
            vector<int> entry;
            Dfs(entry, finded_node);
            cout << number_pattern << ": ";
            PrintArrayEntries(entry);
            cout << "\n";
        }
    }
};


int main() {
    string text;
    ifstream fin("input.txt");
    getline(fin, text);

    SuffixTree suffix_tree(text);
    suffix_tree.BuildSuffixTree();

   // suffix_tree.Print();

    int pattern_number = 1;
    string pattern;

    while (fin >> pattern) {
        suffix_tree.AllEntrance(pattern_number, pattern);
        ++pattern_number;
    }
