#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <stack>
#include <deque>
#include <list>
#include <set>
#include <map>
using namespace std;

#include "BTree.h"
using namespace DTLib;
int main()
{
    string str;
    BTree<int> tree;
    BTreeNode<int> root;
    BTreeNode<int>* pnode=nullptr;
//    tree.insert(1);
    tree.insert(1,&root,LEFT);
    pnode = tree.find(1);
    tree.insert(2,pnode,LEFT);


    return 0;
}




