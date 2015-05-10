#ifndef DIRECTED_ACYCLIC_GRAPH_H
#define DIRECTED_ACYCLIC_GRAPH_H

/*****************************************
 * UW User ID:  ymibrahi
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Fall) 2013
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#include "ece250.h"
#include "Exception.h"
#include <queue>
#include <limits>
#include <iostream>

class Directed_acyclic_graph {
	private:
		bool **double_array;
		int *inDegree;
		int *outDegree;
		int edges;
		double *prirty;
		int array_size;

	public:
		Directed_acyclic_graph( int = 10 );
		Directed_acyclic_graph( Directed_acyclic_graph const & );
		~Directed_acyclic_graph();

		void swap( Directed_acyclic_graph & );
		Directed_acyclic_graph &operator = ( Directed_acyclic_graph );

		int in_degree( int ) const;
		int out_degree( int ) const;
		int edge_count() const;
		bool adjacent( int, int ) const;
		bool connected( int, int ) const;
		void topological_sort() const;

		bool set_priority( int, double );
		bool insert_edge( int, int );
		void clear_edges();
		void reset_priorities();

	friend std::ostream &operator << ( std::ostream &, Directed_acyclic_graph const & );
};

//Constructor - Initiates all values within program 
Directed_acyclic_graph::Directed_acyclic_graph( int size )
{	
	double_array = new bool* [size];
	inDegree = new int [size];
	outDegree = new int [size];
	edges = 0;
	prirty = new double [size];
	array_size = size;

	//Loops within the double_array to create a 2D array
	//to store the bool value if something is connected or not
	for (int i = 0; i<size;i++){
		double_array[i]=new bool[size];
		inDegree[i] = 0;
		outDegree[i] = 0;
		prirty[i] = i;
		for (int j = 0; j < size; ++j)
		{
			double_array[i][j] = false;
		}
	}

}

//Copy Constructor - Creates a copy of the graph 
Directed_acyclic_graph::Directed_acyclic_graph( Directed_acyclic_graph const &obj ){
	
	double_array = new bool* [obj.array_size];
	inDegree = new int [obj.array_size];
	outDegree = new int [obj.array_size];
	edges = obj.edges;
	prirty = new double [obj.array_size];
	array_size = obj.array_size;

	for (int i = 0; i<obj.array_size;i++){
		double_array[i]=new bool[array_size];
		inDegree[i] = obj.inDegree[i];
		outDegree[i] = obj.outDegree[i];
		prirty[i] = obj.prirty[i];
		for (int j = 0; j < obj.array_size; ++j)
		{
			double_array[i][j] = obj.double_array[i][j];
		}
	}
}

//Destructor - Deletes all arrays to deallocate memory
Directed_acyclic_graph::~Directed_acyclic_graph(){

	delete [] inDegree;
	delete [] outDegree;
	delete [] prirty;
	
	for (int i = 0; i<array_size;i++){
		delete [] double_array[i];
	}	
		delete [] double_array;
}

//in_degree Method - checks and returns the in degree
//of a specific vertex within the graph 
int Directed_acyclic_graph::in_degree( int n ) const{
	if(n < 0){
        throw illegal_argument(); 
    }

	return inDegree[n];
}

//out_degree Method - checks and returns the out degree
//of a specific vertex within the graph 
int Directed_acyclic_graph::out_degree( int n ) const{
	if (n<0){
		throw illegal_argument();
	}
	return outDegree[n];
}

//Edge Count Method - Returns the total number of
//edges within the graph 
int Directed_acyclic_graph::edge_count() const{
	return edges;
}

//Adjacent Method - Returns true if and only if a is adjacent to b
bool Directed_acyclic_graph::adjacent( int a, int b ) const{
	if(a==b)
		return false;
	return double_array[a][b];
}

//Connected Method - Returns true if and only if vertex 'a' has a path
//to vertex b
bool Directed_acyclic_graph::connected( int a, int b ) const{
	if(a<0 || b<0 || a>=array_size || b>=array_size){
		throw illegal_argument();
	}

	//Uses a queue to push all the vertices
	//that follow the path from a to b
	std::queue<int> vertices;
	vertices.push(a);
	int tmp = vertices.front();

	//use a while loop to continue to fill the
	//queue  with vertices that are connected
	//searching for vertix 'b'
	while(vertices.empty() != true) {
		tmp = vertices.front();

		vertices.pop();

		if(tmp == b)
			return true;
		
		
		for(int j = 0; j<array_size;j++){
			if(double_array[tmp][j])
				vertices.push(j);
		}
	}
	return false;
}

//Topological Sort Method - This method sorts the graph
//and prints out vertices depending on priority and 
//in degree. Starting with the lowest in degree and increasing
//accordingly through the graph
void Directed_acyclic_graph::topological_sort() const{

	int *copy = new int [array_size];
	double tmp1 = static_cast<double>(ULLONG_MAX);
	int index = 0;

	for(int i=0;i<array_size;i++){
		copy[i] = inDegree[i];
	}

	//Searches the whole in_degree and prirty
	//arrays for the lowest verticy and stores it
	for (int a = 0; a < array_size; a++) {
		for(int i = 0; i<array_size;i++){
			if(copy[i]==0 && prirty[i]<tmp1){
				tmp1 = prirty[i];
				index = i;
			}
		}
		//Reached the end of the sorting and shouldn't
		//print with dash at the end
		if (a == (array_size - 1)) {
			std::cout << index;
		}
		else{
			std::cout << index << '-';
		}

		//Takes the specific vertex that has now
		//been printed and changes its copy in_degree
		//to -1, so it doesn't get printed again
		copy[index] = -1;
		for(int j = 0; j < array_size; j++){
			if(double_array[index][j] == true){
				copy[j] -=1;
			}

		}
		tmp1 = static_cast<double>(ULLONG_MAX);
	}

	//deallocates used memory space
	delete [] copy;
}

//Set Priority Method - Accessess the priority array and inputs a given
//value at a given index, returns true if it has done so, false otherwise
//if that same priority value has been given to a different index
bool Directed_acyclic_graph::set_priority( int a, double priority ){
	
	if(a>=array_size||a<0)
		throw illegal_argument();

	for(int i = 0;i<array_size;i++){
		if(prirty[i]==priority)
			return false;
	}

	prirty[a]=priority;
	return true;
}

//Insert Edge Method - Inserts an edge between two specific verticies 
//only if the two vertices aren't already adjacent and not connected,
//or if they are the same vertex
bool Directed_acyclic_graph::insert_edge( int a, int b ){
	if(a>=array_size || b>=array_size || a<0 || b<0 ){
		throw illegal_argument();
	}

	if(a == b){
		return false;
	}

	if (adjacent(a,b) == true) {
		return false;
	}
	
	if(connected(b,a) == false){
		double_array[a][b] = true;
		edges++;
		outDegree[a]++;
		inDegree[b]++;
		return true;
	}
	return false;
}

//Clear Edges Method - Deletes all edges by 
//clearing the arrays, and deleting all connections
//between verticies 
void Directed_acyclic_graph::clear_edges(){
	edges = 0;
	for (int i =0; i<array_size;i++){
		for (int j=0;j<array_size;j++){
			double_array[i][j] = false;
		}
		inDegree[i] = 0;
		outDegree[i]=0;
	}
	return;
}

//Reset Priorities Method - Replaces all values in the priority
//array by the value of the index
void Directed_acyclic_graph::reset_priorities(){
	for(int i = 0; i<array_size;i++){
		prirty[i]=i;
	}
	return;
}



void Directed_acyclic_graph::swap( Directed_acyclic_graph &dag ) {
	std::swap(double_array,dag.double_array);
	std::swap(prirty, dag.prirty);
	std::swap(inDegree, dag.inDegree);
	std::swap(outDegree, dag.outDegree);
	std::swap(edges, dag.edges);
	std::swap(array_size, dag.array_size);
}

Directed_acyclic_graph &Directed_acyclic_graph::operator = ( Directed_acyclic_graph rhs ) {
	swap( rhs );

	return *this;
}

// whatever you want...

// You can modify this function however you want:  it will not be tested

std::ostream &operator << ( std::ostream &out, Directed_acyclic_graph const &list ) {
	out << "#";

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
