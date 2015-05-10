#ifndef LAZY_TREE_H
#define LAZY_TREE_H

#include <queue>
#include "Lazy_deletion_node.h"

/*****************************************
 * UW User ID:  ymibrahi
 * Submitted for ECE 250
 * Semester of Submission:  (Fall) 2013
 *
 * By  submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#define nullptr 0

template <typename Type>
class Lazy_deletion_tree {
	private:
		Lazy_deletion_node<Type> *root_node;
		int count;

	public:
		Lazy_deletion_tree();
		~Lazy_deletion_tree();

		bool empty() const;
		int size() const;
		int height() const;
		Type front() const;
		Type back() const;
		bool member( Type const & ) const;
		void breadth_first_traversal() const;

		bool insert( Type const & );
		bool erase( Type const & );
		void clean();
		void clear();
};

template <typename Type>
Lazy_deletion_tree<Type>::Lazy_deletion_tree():
root_node( nullptr ),
count( 0 ) {
	// Only initializes member variables
}

//Destructor - Clears everything within the tree
template <typename Type>
Lazy_deletion_tree<Type>::~Lazy_deletion_tree(){
	clear();
}

//Empty Method - Resets number of nodes in tree 
//to zero
template <typename Type>
bool Lazy_deletion_tree<Type>::empty() const {
	return (count == 0);
}

//Size Method - Returns the number of nodes in tree, stored in 
//variable count
template <typename Type>
int Lazy_deletion_tree<Type>::size() const {
	return count;
}

//Height Method - Calls the height method from the 
//node function height 	
template <typename Type>
int Lazy_deletion_tree<Type>::height() const{
	return root_node->height();
}

//Front Method - Calls the front method from the node
//function front, but also throws an exception if tree
//is empty
template <typename Type>
Type Lazy_deletion_tree<Type>::front() const{
	if (root_node->front().second == 0)
		throw underflow();
	else
	return root_node->front().first;
}

//Back Method - Calls the back method from the node
//function back, but also throws an exception if tree
//is empty
template <typename Type>
Type Lazy_deletion_tree<Type>::back() const{
	if(root_node->back().second == 0)
		throw underflow();
	return (root_node->back()).first;
}

//Member Method - Calls the member method from the node
//function member, sending in the arguement obj to return a 
//boolean value 
template <typename Type>
bool Lazy_deletion_tree<Type>::member( Type const &obj ) const{
	return root_node->member(obj);
}


//Breadth-First-Traversal Method - Traverses through the tree using the 
//Breadth First method and printing out the result including nodes that
//are tagged as erased 
template <typename Type>
void Lazy_deletion_tree<Type>::breadth_first_traversal() const {
	//Initially checks if list is empty - nothing to traverse
	if ( root_node == nullptr ) {
		return;
	}

	//Calls from the standard library for queue to initialize
	//variable 'queue' to hold data for traversing
	std::queue< Lazy_deletion_node<Type> *> queue;

	//Starts the traversal at the root node
	queue.push( root_node );

	//Using a while loop to iterate through the list until
	//it queue has gone through every node
	while ( !queue.empty() ) {

		//Initiates pointer to help iterate through tree
		//starting at the front value of the queue
		Lazy_deletion_node<Type> *ptr = queue.front();

		//Pops the oldest element in the queue, currently 
		//being pointed to by 'ptr'
		queue.pop();

		//Checks if the element currently is erased and prints
		//its value with an x in-front of it to recognize it
		//is flagged as erased within the tree 
		if(ptr->erased){
			std::cout<<ptr->retrieve()<<"x ";		
		}
		//If not erased, then it gets printed without an x
		else{
			std::cout<<ptr->retrieve()<<" ";
		}

		//Starting from the left of the tree
		//the traversal pushes the value of the node
		//into the queue 
		if(ptr->left() != nullptr){
			queue.push(ptr->left_tree);
		}
		//Following, the traversal goes to the node to the
		//right of the stored node, storing the value 
		//of the new node
		if(ptr->right() != nullptr){
			queue.push(ptr->right_tree);
		}	
	}
}

//Insert Method - Mostly uses the node method of insert but implements
//the counter to update size as well as considers the case tree is empty
template <typename Type>
bool Lazy_deletion_tree<Type>::insert( Type const &obj ){
	//Case 1 - Tree is empty
	if(root_node == nullptr){
		root_node = new Lazy_deletion_node<Type>(obj);
		count=1;
		return true;
	}

	//Case 2 - Tree has nodes but does not contain 
	//the value attempting to insert
	if(root_node->insert(obj) == true){
		count++;
		return true;
	}

	//Case 3 - Tree has nodes and already contains value being
	//inserted
	else{
		return false;
	}
}


//Erase Method - Calls the erase method in the node to check 
//what to do 
template <typename Type>
bool Lazy_deletion_tree<Type>::erase( Type const &obj ){
	//Case 1 - Tree is empty 
	if (count == 0){
		return false;
	}

	//Case 2 - Node is in tree and has been erased
	if (root_node->erase(obj) == true){
		count--;
		return true;
	}
	//Case 3 - Node was not in tree or has already been erased
	else{
		return false;
	}
}

//Clear Method - Clears all elements in the tree, and resets all 
//variables that get updated with tree 
template <typename Type>
void Lazy_deletion_tree<Type>::clear(){
	//Calls the method from the node class to clear all 
	//elements 
	root_node->clear();

	//Resets the root_node to null and renews count of nodes to 
	//zero
	root_node = nullptr;
	count = 0;
}

//Clean Method - Calls the clean method in node to 
//erase the flagged nodes
template <typename Type>
void Lazy_deletion_tree<Type>::clean(){
	root_node->clean(root_node);
}

#endif
