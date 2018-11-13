//#include <bits/stdc++.h>
#include <list>
#include "node.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <climits>
#include <cstdlib>

using namespace std;
typedef long l;
typedef struct Tree Tree;
struct Tree
{
	l value;
	l degree;
	struct Tree *parent;
	struct Tree *child;
	struct Tree *sibling;
	node *data;
};

//print BTree
void printTree(Tree *root);
//print Bheap
void printHeap(list<Tree*> heap) ;
//create a bew BTree with 0 degree
Tree * newNode(node * data,int value);
//merge 2 BTrees having same degree
Tree * merge2BTrees(Tree * first, Tree * second);
//merge 2 heaps into a new heap with nondecending order of degrees of BTrees
list<Tree*> merge(list<Tree*> heap1, list<Tree*> heap2);
//make adjustments to assure one BTree corresponding to one degree(not more)
//cases
	//case1: if degree of adjacent nodes are different -> move forward
	
	//case 2: if degree of adjacent nodes are same
		//2-a
			// if degree of node next to next of current node is same as current->move forward
		//2-b else
			//merge BTrees corresponding to current and next
list<Tree*> adjust(list<Tree*> heap);
//insert a new node(BTree with 0 degree) in BHeap
list<Tree *> insert(list<Tree*> heap,node *data,int value);
// Function to search for an element 
Tree *findNode(Tree* tree, int val, node* n);
Tree* find(list<Tree*> heap,int val, node* n);
// Function to decrease the value of old_val 
// to new_val 
list<Tree*> decreaseKeyBHeap(list<Tree*> heap,node* n, int oldV, int newV);
//return root node of BTree with minimum value :O(logn)
Tree * findMin(list<Tree*> heap);
//extract the minimum node
list<Tree*> extractMin(list<Tree*> heap);
node *get_data1(int &order);
int *dijkstra_bino(node *node_list,int total_nodes,int source);
