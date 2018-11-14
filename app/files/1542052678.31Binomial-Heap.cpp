#include "Binomial-Heap.h"
//print BTree
void printTree(Tree *root){ 
    while(root!=NULL){ 
        cout<<root->value<<"->"<<root->data->node_id<< " "; 
        printTree(root->child); 
        root=root->sibling; 
    } 
} 


//print Bheap
void printHeap(list<Tree*> heap) 
{ 
	cout<<"-------------------------\n";
    list<Tree*> ::iterator it; 
    it = heap.begin(); 
    while (it != heap.end()){ 
        printTree(*it); 
        cout<<endl;
        it++; 
    } 
    cout<<"-------------------------\n";
} 

//create a bew BTree with 0 degree
Tree * newNode(node * data,int value){
	Tree * newNode=(Tree *)malloc(sizeof(Tree));
	newNode->value=value;
	newNode->degree=0;
	newNode->parent=NULL;
	newNode->child=NULL;
	newNode->sibling=NULL;
	newNode->data=data;

	return newNode;
}

//merge 2 BTrees having same degree
Tree * merge2BTrees(Tree * first, Tree * second){
	if(first->value > second->value )
		swap(first, second);
	//change parent of second
	second->parent=first;
	//change sibling of second
	second->sibling=first->child;
	//change child of parent
	first->child=second;
	// increment the degree of parent
	first->degree+=1;
	return first;

}

//merge 2 heaps into a new heap with nondecending order of degrees of BTrees
list<Tree*> merge(list<Tree*> heap1, list<Tree*> heap2){
	list<Tree*> newHeap;
	list<Tree*> :: iterator it1=heap1.begin();
	list<Tree*> :: iterator it2=heap2.begin();
	while(it1!=heap1.end()&&it2!=heap2.end()){
		if((*it1)->degree<=(*it2)->degree){
			newHeap.push_back(*it1);
			it1++;
		}
		else{
			newHeap.push_back(*it2);
			it2++;
		}
	}
	while(it1!=heap1.end()){
		newHeap.push_back(*it1);
		it1++;
	}
	while(it2!=heap2.end()){
		newHeap.push_back(*it2);
		it2++;
	}
	// cout<<"-----------------------------\n";
	// printHeap(newHeap);
	// cout<<"-----------------------------\n";
	return newHeap;
}

//make adjustments to assure one BTree corresponding to one degree(not more)
//cases
	//case1: if degree of adjacent nodes are different -> move forward
	
	//case 2: if degree of adjacent nodes are same
		//2-a
			// if degree of node next to next of current node is same as current->move forward
		//2-b else
			//merge BTrees corresponding to current and next
list<Tree*> adjust(list<Tree*> heap){
	list<Tree *>:: iterator it1, it2, it3;
	it1=heap.begin();
	it2=it1;
	it3=it1;
	list<Tree *> newHeap;
	if(heap.size()<=1)
		return heap;
	if(heap.size()==2){
		it2++;
		it3=heap.end();
	}
	else{
		it2++;
		it3=it2;
		it3++;
	}
	while(it1!=heap.end()){
		//case1
		if(it2==heap.end())
			it1++;
		else if((*it1)->degree!=(*it2)->degree){
			it1++;
			it2++;
			if(it3!=heap.end())
				it3++;
		}
		else{ //case 2-a
			if(it3!=heap.end()&&(*it1)->degree==(*it3)->degree){
				it1++;
				it2++;
				it3++;
			}
			else{	//case 2-b
	            *it1=merge2BTrees(*it1,*it2); 
	            it2=heap.erase(it2); 
	            if(it3!=heap.end()) 
	                it3++; 
			}
		}
	}
	return heap;
}

//insert a new node(BTree with 0 degree) in BHeap
list<Tree *> insert(list<Tree*> heap,node *data,int value){
	Tree * nNode=newNode(data,value);
	list<Tree *> newHeap;
	newHeap.push_back(nNode);
	//merge return new list with Btrees in nondecending order of degree
	//adjust take care to avoid 2 BTrees with same degree
	return adjust(merge(heap, newHeap));
}


// Function to search for an element 

Tree *findNode(Tree* tree, int val, node* n) 
{ 
    if (tree == NULL) 
      return NULL; 
    if (tree->value==val && n->node_id==tree->data->node_id)
        return tree; 
    // Recur for child 
    Tree *res=findNode(tree->child, val,n); 
    // cout<<"Node Id "<<res->data->node_id<<" " <<n->node_id<<endl;
    if (res != NULL )
       	return res; 
    return findNode(tree->sibling, val,n); 
} 

Tree* find(list<Tree*> heap,int val, node* n){
	list<Tree*> ::iterator it; 
	Tree* temp;
    it = heap.begin(); 
    while (it != heap.end()){ 
        temp=findNode(*it,val,n);
        if(temp!=NULL)
        	return temp;
        it++; 
    }
    return NULL;
}

// Function to decrease the value of old_val 
// to new_val 
list<Tree*> decreaseKeyBHeap(list<Tree*> heap,node* n, int oldV, int newV) 
{ 
    // First check element present or not 
    // cout<<"in dc "<<(n->node_id)<<endl;
    Tree* _node;
	_node=find(heap, oldV,n); 
	// cout<<"found "<<(_node->data->node_id)<<endl;

    // return if Node is not present 
    if (_node == NULL){
    	// cout<<"Test\n";
        return heap; 
    }
    // Reduce the value to the minimum 
    _node->value=newV; 
    Tree *parent=_node->parent; 
    // Update the heap according to reduced value 
    while(parent!=NULL&&_node->value<parent->value) 
    { 
        swap(_node->value, parent->value); 
        swap(_node->data,parent->data);
        _node=parent; 
        parent=parent->parent; 
    } 
    return heap;
} 

//return root node of BTree with minimum value :O(logn)
Tree * findMin(list<Tree*> heap){
	Tree * heapMin=newNode(NULL,INT_MAX);//have to set largest (more than INT_MAX-1)
	for (list<Tree*> :: iterator i = heap.begin(); i != heap.end(); ++i)
		if((*i)->value<heapMin->value)
			heapMin=*i;
	return heapMin;
}	

//extract the minimum node
list<Tree*> extractMin(list<Tree*> heap){
	// cout<<"In extractMin\n";
	Tree *heapMin=findMin(heap);
	list<Tree*> newHeap;//heap containing all trees other than tree with minnode
	list<Tree*> :: iterator it=heap.begin();
	//create heap of all nodes other than tree with minnode
	while(it!=heap.end()){
		if(*it!=heapMin)
			newHeap.push_back(*it);
		it++;
	}
	//now create a heap with child of btree having minimun as its root
	Tree *child=heapMin->child;
	Tree *temp;
	list<Tree*> heap2;//heap containing all subtrees of tree with minnode
	while(child!=NULL){
		temp=child;
		child=child->sibling;
		temp->sibling=NULL;
		heap2.push_front(temp);//to insert lower degree first
	}
	return adjust(merge(newHeap, heap2));
}


node *get_data1(int &order) {

	int n;
	scanf("%d",&n);
	order = n;
	node *node_list = new node[order];
	for(int i = 0; i < order; i++) {
		node_list[i].node_id = i;
		node_list[i].current_dist = INT_MAX-1;
	}
	scanf("%d",&n);
	while(n--) {
		int a,b,c;
		scanf("%d%d%d",&a,&b,&c);
		insert_edge(&node_list[a-1],&node_list[b-1],c);
	}
	return node_list;
}


