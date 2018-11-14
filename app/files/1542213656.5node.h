#include<cstdio>
#include<vector>
#include<climits>

#ifndef NODE_STRUCT
#define NODE_STRUCT
struct node {
	int node_id;
	int current_dist = 0;
	std::vector<std::pair<node *,int>> edge_list;
	node *next = NULL;
	node *prev = NULL;

};


node *create_node(int node_id);
void insert_edge(node *start_node,node *end_node,int weight);

#endif
