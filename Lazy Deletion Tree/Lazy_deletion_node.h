#ifndef LAZY_DELETION_NODE_H
#define LAZY_DELETION_NODE_H

/*****************************************
 * UW User ID:  ymibrahi
 * Submitted for ECE 250
 * Semester of Submission:  (Fall) 2013
 *
 * By  submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#include "ece250.h"
#include "Exception.h"
#include <utility>

#define nullptr 0

template <typename Type>
class Lazy_deletion_tree;

template <typename Type>
class Lazy_deletion_node {
	private:
		Type element;
		Lazy_deletion_node<Type> *left_tree;
		Lazy_deletion_node<Type> *right_tree;
		bool erased;

	public:
		Lazy_deletion_node( Type const & = Type() );

		Lazy_deletion_node<Type> *left() const;
		Lazy_deletion_node<Type> *right() const;
		Type retrieve() const;
		int height() const;
		std::pair<Type, bool> front() const;
		std::pair<Type, bool> back() const;
		bool member( Type const & ) const;

		bool insert( Type const & );
		bool erase( Type const & );
		void clear();
		void clean( Lazy_deletion_node *& );

	friend class Lazy_deletion_tree<Type>;
};

template <typename Type>
Lazy_deletion_node<Type>::Lazy_deletion_node( Type const &obj ):
element( obj ),
left_tree( nullptr ),
right_tree( nullptr ),
erased( false ) {
	// does nothing
}

//Left Method - Returns the type pointer left tree, allowing it 
//to show the element to the left of the node
template <typename Type>
Lazy_deletion_node<Type> *Lazy_deletion_node<Type>::left() const {
        return left_tree;
}

//Right Method - Returns the type pointer right tree, allowing it 
//to show the element to the right of the node
template <typename Type>
Lazy_deletion_node<Type> *Lazy_deletion_node<Type>::right() const {
        return right_tree;
}

//Retrieve Method - Returns the element located within a specific node
template <typename Type>
Type Lazy_deletion_node<Type>::retrieve() const {
	return element;
}

//Height Method - Checks the current height of all nodes within tree
template <typename Type>
int Lazy_deletion_node<Type>::height() const {
	//Case 1 - Tree is empty, returns a value of -1
	if (this == nullptr){return -1;}

	//Case 2 - Tree contains elements and iterates the method
	//through the tree and adds one for the root node
	else
		return (1 + std::max(left_tree->height(),right_tree->height()));
}


//Front Method - Checks flags of the front node and returns the value of 
//the left most node if existant 
template <typename Type>
std::pair<Type, bool> Lazy_deletion_node<Type>::front() const {
	//Case 1 - Tree is empty
	if ( this == nullptr ) {
		return std::pair<Type, bool>( Type(), false );
	}

	//Case 2 - Checks left branch of tree to see if it contains
	//any elements
	std::pair<Type, bool> result = left()->front();

	if ( result.second ) {
		return result;
	}

	if ( !erased ) {
		return std::pair<Type, bool>( retrieve(), true );
	}

	//Case 3 - Checks the right branch of the tree to see if it
	//contains any elements 
	result = right()->front();

	if ( result.second ) {
		return result;
	}

	return std::pair<Type, bool>( Type(), false );
}

//Back Method - Implemented as the front method, except checks the right
//most branch before considering left tree branches
template <typename Type>
std::pair<Type, bool> Lazy_deletion_node<Type>::back() const {
	if ( this == nullptr ) {
		return std::pair<Type, bool>( Type(), false );
	}

	std::pair<Type, bool> result = right()->back();

	if ( result.second ) {
		return result;
	}

	if ( !erased ) {
		return std::pair<Type, bool>( retrieve(), true );
	}

	result = left()->back();

	if ( result.second ) {
		return result;
	}

	return std::pair<Type, bool>( Type(), false );
}

//Member Method - Checks the current status of a node and returns a boolean
//depending on the status 
template <typename Type>
bool Lazy_deletion_node<Type>::member( Type const &obj ) const{
	//Case 1 - Empty tree, returns false
	if (this == nullptr){return false;}

	//Case 2 - Tree has nodes, but element being checked is not erased
	//and returns true
	else if(obj==element && !erased){
		return true;
	}

	//Case 3 - Tree has nodes, but currenet elements is a smaller
	//value than object being checked, hence we check the right of
	//the tree
	else if(element < obj){
		return right_tree->member(obj);
	}

	//Case 4 - Object is found in tree and has been flagged as erased
	//returns false
	else if(obj==element && erased){
		return false;
	}

	//Case 5 - Tree has nodes, but currenet elements is a smaller
	//value than object being checked, hence we check the right of
	//the tree
	else{
		return left_tree->member(obj);
	}
	
}

//Insert Method - Checks if value is in list, and if not
//adds value to the expected location within the tree
template <typename Type>
bool Lazy_deletion_node<Type>::insert( Type const &obj ){
	//Case 1 - If current node contains the object
	if(element==obj){
		//Checks if erased and changes its flag to not erased
		if(erased){
			erased = false;
			return true;
		}
		//It has not been erased and hence returns false because
		//it is already in the tree
		else{return false;}
	}

	//Case 2 - Needs to search for node in tree with same
	//element, and goes to the right because the object being searched
	//for is larger than current value
	if(element < obj && right_tree!=nullptr){
		return right_tree->insert(obj);
	}

	//Case 3 - Needs to search for node in tree with same element, and goes
	//to the left of the tree because the object being searched for is smaller
	//than current value
	if(element > obj && left_tree!=nullptr){
		return left_tree->insert(obj);
	}

	//Case 4 - When the object being searched for was not found
	else{
		//After reaching the end of the list, a new node is added
		//to the left if current element is greater than object being
		//searched for
		if (element > obj && this->left()==nullptr){
			this->left_tree = new Lazy_deletion_node<Type>(obj);
			return true;
		}
		//New node added to right if current element is smaller than
		//object being searched for
		else if(element < obj && this->right() == nullptr){
			this->right_tree = new Lazy_deletion_node<Type>(obj);
			return true;
		}
		return false;
	}
}

//Erase Method - Flags a specific node as erased but does not 
//delete it from tree
template <typename Type>
bool Lazy_deletion_node<Type>::erase( Type const &obj){
	//Case 1 - Element is the same as object being searched for
	if(element==obj){
		//If already erased, return false
		if(erased){
			return false;
		}
		//Other wise, changes flag of node to erased and returns
		//true
		else{
			erased = true;
			return true;
		}
	}

	//Case 2 - Element is less than object being searched for 
	//and must iterate through the right of the tree by calling 
	//the same erase method
	else if(element < obj && right_tree!=nullptr){
		return right_tree->erase(obj);
	}

	//Case 3 - Element is greater than object being searched for 
	//and must iterate through the left of the tree by calling 
	//the same erase method
	else if(element > obj && left_tree!=nullptr){
		return left_tree->erase(obj);
	}

	//Case 4 - Element was not found in list and cannot be erased
	//returns false
	else{
		return false;
	}
}

//Clear Method - Deletes all nodes within the tree
template <typename Type>
void Lazy_deletion_node<Type>::clear(){
	//Case 1 - Tree is empty, nothing to be deleted
	if(this == nullptr){
		return;
	}

	//Case 2 - Tree is not empty, iterates through tree deleting
	//all elements within the tree
	right_tree->clear();
	left_tree->clear();
	delete this;
}

//Clean Method - Deletes all nodes that have been flagged as erased
//within the tree
template <typename Type>
void Lazy_deletion_node<Type>::clean( Lazy_deletion_node *&ptr_to_this ){
	//Case 1 - Tree is empty, nothing to be cleaned
	if(this == nullptr){
		return;
	}

	//Case 2 - Tree is not empty, checks the 
	//flag of current node and replaces it if necessary
	//depending on if right and left nodes have also been erased
	//or not
	if(erased == true){
		if(right()->front().second == 1){
			element = right()->front().first;
			erased = false;
			right()->erase(right()->front().first);
		}
		else if(left()->back().second == 1){
			element = left()->back().first;
			erased = false;
			left()->erase(left()->back().first);
		}
	}
	//Continues to iterate through the tree
	//in both directions of right and left
	right()->clean(right_tree);
	left()->clean(left_tree);

	//Second check if last node has also been erased
	//it then removes the last node and deletes the link
	//replacing it with null
	if (erased == true){
		ptr_to_this = nullptr;
		delete this;	
	}
}

#endif
