#ifndef PRACTIVE_H
#define PRACTIVE_H

#include <iostream>
#include <QString>
#include <QDebug>
#include <QStack>
#include <vector>
#include <stack>
#include <list>
using namespace std;

#include "LinkList.h"
#include "GTree.h"
using namespace DTLib;

template < typename T >
struct Node
{
    T value;
    Node* next;
};

void linklist_test();

void stl_vector_test();
void stl_deque_test();
void stl_stack_test();
void stl_list_test();

bool parenthesis_matching(string& ss);

Node<int>* create_list(int len, int stv=0);
void delete_list(Node<int>* list);
void print_list(Node<int>* list);
Node<int>* flip_list(Node<int>* list);
void reverse_list_test();
Node<int>* merge_list(Node<int>* list1, Node<int>* list2);
void merge_list_test();

void choiceSort(vector<int>& arr, int len);
void quickSort(int left, int right, vector<int>& arr);

int sum(int n);
int str_len(const char* s);
void hanoiTower(int n,char a,char b,char c);

void tree_test();


#endif // PRACTIVE_H
