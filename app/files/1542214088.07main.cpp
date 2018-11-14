#include<iostream>
#include<list>
#include"fibonacci.h"
#include"van_emde.h"
#include"Binomial-Heap.h"
#include<cstring>
#include<chrono>
using namespace std::chrono;
//#include<iostream>
#define dbg(A) printf(A);
#define dl printf("\n\n");
using namespace std;

//all functions return a distance vector with minimum distace from source which is assumed to be the 0 indexed node
//function for calculating dijkstra using fibonacci heaps 
int *dijkstra_fibo(node *node_list,int total_nodes,int source) {

	fibo_heap heap;
	fibo_node node_vec[total_nodes];
	int *dist_vec = new int[total_nodes];

	/*Initialize the fibonacci heap nodes and insert into heap
	key determines the distance from source 
	Each node in the graph has a corresponding node in the fibonacci heap indexed in the node_vac using the node_id */
	for(int i = 0; i < total_nodes; i++) {
		if(i == source) continue;
		node_vec[i].key  = INT_MAX;
		node_vec[i].data = &node_list[i];
		node_vec[i].child = NULL;
		heap.insert(&node_vec[i]);
	}
	//Initialize the first node as the source and insert into heap
	node_vec[source].key = 0;
	node_vec[source].data = &node_list[source];
	node_vec[source].child = NULL;
	heap.insert(&node_vec[source]);
	int count = 0;


	while(!heap.is_empty()) {
		//extract minimum distance node and insert the distance in resultant vector;
		fibo_node *head = heap.extract_min();
		node *cur = head -> data;
		dist_vec[cur -> node_id] = head -> key;
		cur->current_dist = 1;
		//decrease (if required) the distance from the source node
		for(auto i = cur->edge_list.begin(); i != cur->edge_list.end(); i++) {
			if(i -> first -> current_dist ) continue;
			int adj_node = i -> first -> node_id;
			//check if current distance from source is greater than if we take a path through the current selected node
			if(node_vec[adj_node].key > (node_vec[cur -> node_id].key + i -> second)) {
				heap.decrease_key(&node_vec[adj_node],node_vec[cur -> node_id].key + i -> second);
			}
		}
		//printf("Final\n");
		//heap.show_tree();
		//printf("\n");
		count++;
	}

	return dist_vec;
}	

//function to calculation dijkstra using binomial heaps
//int *dijkstra_bino(node *node_list,int total_nodes,int source) {



//}

//function to calculation dijkstra using van emde boas trees
int *dijkstra_vEBT(veb_tree *tree,node *node_list,int total_nodes,int source) {

	int cur = 0;
	int *dist_vec = new int[total_nodes];
	for(int i=0; i < total_nodes; i++) {
		dist_vec[i] = -1;
		if(i == source) 
			node_list[i].current_dist = 0;
		else node_list[i].current_dist = tree->universe - 1;
		tree -> insert(&node_list[i]);
	}

	while(cur < total_nodes) {
		//tree -> show_tree(); dl
		node *min_node = tree->get_min();
		dist_vec[min_node -> node_id] = min_node -> current_dist;
		tree -> delnode(min_node);

		for(auto i = min_node->edge_list.begin(); i != min_node->edge_list.end(); i++) {
			node *cur = i -> first;
			if(dist_vec[cur->node_id] != -1) continue;
			if(cur -> current_dist > (i -> second + min_node -> current_dist)) {
				tree -> delnode(cur);
				cur -> current_dist = i -> second + min_node -> current_dist;
				tree -> insert(cur);
			}
		}
		cur++;
	}
	return dist_vec;
}

int *dijkstra_bino(node *node_list,int total_nodes,int source) {
	list<Tree *> bHeap;
	int *dist_vec=new int[total_nodes];
	for(int i = 0; i < total_nodes; i++) {
		if(i == source){
			node_list[i].current_dist=0;
			bHeap=insert(bHeap,&node_list[i],0);
			// cout<<"hello\n";
			// printHeap(bHeap);
			continue;
		}
		bHeap=insert(bHeap,&node_list[i],INT_MAX-1);//in case graph is disconnected
		// printHeap(bHeap);
	}
	// printHeap(bHeap);
	while(!bHeap.empty()){	
		// cout<<"size "<<bHeap.size()<<endl;
		// printHeap(bHeap);
		Tree *head=findMin(bHeap);
		node *cur=head->data;
		// cout<<(cur==NULL)<<endl;
		// cout<<head->value<<" "<<cur->node_id<<endl;
		bHeap=extractMin(bHeap);
		dist_vec[cur->node_id]=head->value;
		for(vector<std::pair<node *,int> > :: iterator i = cur->edge_list.begin(); i != cur->edge_list.end(); i++) {
			// printHeap(bHeap);
			// if(i->first->current_dist ) continue;
			int adj_node = i->first->node_id;
			// cout<<"adj node "<<adj_node<<" "<<node_list[adj_node].current_dist<<" "<<(cur->current_dist + i -> second)<<endl;
			//check if current distance from source is greater than if we take a path through the current selected node
			if(node_list[adj_node].current_dist > (cur->current_dist + i -> second)) {
				int oldV=node_list[adj_node].current_dist;
				node_list[adj_node].current_dist=(cur->current_dist + i -> second);
				bHeap=decreaseKeyBHeap(bHeap,&node_list[adj_node],oldV,(cur->current_dist + i -> second));
				// printHeap(bHeap);
				// heap.decrease_key(&node_vec[adj_node],node_vec[cur -> node_id].key + i -> second);
			}
		}
		

	}

	return dist_vec;
}


node *get_data(int &order) {

	int n;
	scanf("%d",&n);
	order = n;
	node *node_list = new node[order];
	for(int i = 0; i < order; i++) {
		node_list[i].node_id = i;
		node_list[i].current_dist = INT_MAX;
	}
	scanf("%d",&n);
	while(n--) {
		int a,b,c;
		scanf("%d%d%d",&a,&b,&c);
		insert_edge(&node_list[a-1],&node_list[b-1],c);
	}
	return node_list;
}


void print_dist(int *dist_vec,int size) {

	for(int i = 0; i < size; i++) printf("%d ", dist_vec[i]);
	printf("\n");
}

void veb_test() {

	node node_list[6];
	veb_tree nt;
	nt.init(8,new node);
	int arr[] = { 0,7, 7, 7, 7, 7};
	for(int i = 0; i < 6; i++) {
		node_list[i].current_dist = arr[i];
		nt.insert(&node_list[i]);
	}
	nt.show_tree();
	printf("\n");
	for(int i = 0; i < 6; i++) {
		nt.delnode(&node_list[i]);
		nt.show_tree();
		printf("\n\n");
	}



}

int main(int args,char *argc[]) {

	//veb_test();

	bool bino = false,fibo = false ,veb = false;

	int list_size;
	node *node_list = get_data(list_size);
	int source;
	scanf("%d",&source);
	int avgRuns = 10;
	if(args > 1) {
		sscanf(argc[1],"%d",&avgRuns);
	}
	if(args > 2) {
		if(strcmp(argc[2],"bino") == 0) bino = true;
		if(strcmp(argc[2],"fibo") == 0) fibo = true;
		if(strcmp(argc[2],"veb") == 0) veb = true;
	}
	else bino = fibo = veb = true;

	int veb_size;
	scanf("%d",&veb_size);
	veb_tree newTree;
	newTree.init(veb_size,new node);

	int total_duration = 0;
	int *djk;
	auto start = high_resolution_clock::now(),end = high_resolution_clock::now();	
	auto duration = duration_cast<microseconds>(end - start);

	//perform an average of runs as defined in the avgRuns parameter
	if(args == 1) cout << "Total nodes " << list_size << " " << "Veb tree universe size " << veb_size << " \n";
	else cout << list_size << " " << veb_size << " ";
	if(veb) {
		for(int count = 0; count < avgRuns; count++) {
			
			for(int i=0; i < list_size; i++) {
				node_list[i].current_dist = 0;
			}
			start = high_resolution_clock::now();
			djk = dijkstra_vEBT(&newTree,node_list,list_size,source-1);
			end = high_resolution_clock::now();
			duration = duration_cast<microseconds>(end - start);
			total_duration += duration.count();
		}
		if(args == 1) {
			cout << "vEB - Tree distance vector\n";
			print_dist(djk,list_size);
			printf("\n Duration ");
		}
		cout << (float)total_duration/avgRuns << " ";
		if(args == 1) cout << endl;
	}

	if(fibo) {
		total_duration = 0;
		for(int count = 0; count < avgRuns; count++) {
			
			for(int i=0; i < list_size; i++) {
				node_list[i].current_dist = 0;
			}
			start = high_resolution_clock::now();
			djk = dijkstra_fibo(node_list,list_size,source-1);
			end = high_resolution_clock::now();
			duration = duration_cast<microseconds>(end - start);
			total_duration += duration.count();
		}
		if(args == 1) {
			cout << "Fibonacci - Tree Distance vector\n";
			print_dist(djk,list_size);
			printf("\n Duration ");
		}
		cout << (float)total_duration/avgRuns << " ";
		if(args == 1) cout << endl;

	}

	if(bino) {
		total_duration = 0;
		for(int count = 0; count < avgRuns; count++) {
			
			for(int i=0; i < list_size; i++) {
				node_list[i].current_dist = INT_MAX-1;
			}
			start = high_resolution_clock::now();
			djk = dijkstra_bino(node_list,list_size,source-1);
			end = high_resolution_clock::now();
			duration = duration_cast<microseconds>(end - start);
			total_duration += duration.count();
		}
		if(args == 1) {
			cout << "Binomial - Tree Distance vector\n";
			print_dist(djk,list_size);
			printf("\n Duration ");
		}
		cout << (float)total_duration/avgRuns;
		if(args == 1) cout << endl;
	}
	cout << endl;
	
	return 0;
}
