# Data stucture from course Discrete analysis and my curiosity

## I've collected all data stucture, that i wrote.
### *PS: On default i wrote in C++. Another case i note in round brackets*

---

+ #### [ Binary Tree (бинарное дерево поиска)](https://github.com/toth3m00n/data-structure/main/BinaryTree.cpp)

Simple Realization BST - Binary Search Tree

--- 

+ #### [Heap](https://github.com/toth3m00n/data-structure/main/Heap.cpp)
  
+ #### [Queue](https://github.com/toth3m00n/data-structure/main/Queue.cpp)
  
+ #### [Stack](https://github.com/toth3m00n/data-structure/main/Stack.cpp)
  
+ #### [Vector](https://github.com/toth3m00n/data-structure/main/Vector.cpp)
  
+ #### [One linked list](https://github.com/toth3m00n/data-structure/main/OneLinkedList.cpp) + [One linked list](https://github.com/toth3m00n/data-structure/main/linked_list.py) (Python)
  
I've done it to figure out how the structure works from the inside

---

+ #### [LRU-cache](https://github.com/toth3m00n/data-structure/main/lru.py) (Python)

Simple realization using One linked list, mentioned earlier

---

---

+ #### [Suffix Tree (Ukkonen's algorithm)](https://github.com/toth3m00n/data-structure/main/suffix_tree.cpp)
  
Complex data structure for search entry in text. Tree build based on text. You can read more about it [here](https://www.geeksforgeeks.org/pattern-searching-using-suffix-tree/)

---

+ #### [Red black tree](https://github.com/toth3m00n/data-structure/main/RBTFix.cpp)

Balanced BST. It wrote for string value, so you can modified for you needs

+ #### [Persistent red back tree](https://github.com/toth3m00n/data-structure/main/RBTFix.cpp)

Data structure that let you store versions of yours Red black tree. Here i implement only part persistent. 

I do it for that task:

```
You are given a set of horizontal segments, and a set of points. For each point, determine how many segments lie strictly above it.
Your solution should work online, that is, it should process requests one at a time after building the necessary data structure based on the input data. Reading input data and queries together and building a common structure based on them is prohibited.

Input format
In the first line, you are given two numbers n and m (1 ≤ n, m ≤ 10^5 )
— the number of segments and the number of points , respectively. In the next n
lines, you are given segments, in the form of triples of the numbersчисел l, r и h( −10^9 ≤ l < r ≤ 10^9, −10^9 ≤ h ≤ 10^9) —are the x coordinates of the left and right boundaries of the segment and
the y coordinate
of the segment, respectively. In the next m lines, you are given pairs of numbers x, y ( −10^9 ≤ x, y ≤ 10^9) — coordinates of the points.
Output format
For each point, print the number of segments above it.
```

The version is created by copying the nodes that we interact with, and after copying the paths to the root, and forwarding links to other children in this path from the node.

---

+ [KMP](https://github.com/toth3m00n/data-structure/main/RBTFix.cpp)
searching for a substring in a string
