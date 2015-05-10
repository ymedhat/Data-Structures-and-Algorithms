#ifndef SINGLE_LIST_H
#define SINGLE_LIST_H

/*****************************************
 * UW User ID:  ymibrahi
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Fall) 2013
 *
 * By submitting this file,  I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#include "ece250.h"
#include "Single_node.h"
#include "Exception.h"
#include <iostream>

#define NULLVARIATE_CONSTRUCTOR

template <typename Type>
class Single_list {
	private:
		Single_node<Type> *list_head;
		Single_node<Type> *list_tail;
		int node_count;

	public:
		Single_list();
		Single_list( Single_list const & );
		~Single_list();

		// Accessors

		int size() const;
		bool empty() const;

		Type front() const;
		Type back() const;

		Single_node<Type> *head() const;
		Single_node<Type> *tail() const;

		int count( Type const & ) const;

		// Mutators

		void swap( Single_list & );
		Single_list &operator = ( Single_list const & );

		void push_front( Type const & );
		void push_back( Type const & );

		Type pop_front();

		int erase( Type const & );

	// Friends

	template <typename T>
	friend std::ostream &operator << ( std::ostream &, Single_list<T> const & );
};

template <typename Type>
Single_list<Type>::Single_list():
list_head( 0 ),
list_tail( 0 ),
node_count( 0 ) {
	// empty constructor
}

template <typename Type>
Single_list<Type>::Single_list( Single_list<Type> const &list ):
list_head( 0 ),
list_tail( 0 ),
node_count( 0 ) {
if(!list.empty()){
	for ( Single_node<Type> *ptr = list.head(); ptr != 0; ptr = ptr->next() ) {
	    	std::cout<<ptr->retrieve()<<std::endl;
	    	push_back(ptr->retrieve());
	    }
	}
}

template <typename Type>
Single_list<Type>::~Single_list() {
	while ( !empty() ) 
         pop_front();
    node_count=0;
 }

template <typename Type>
int Single_list<Type>::size() const {
	return node_count;
}

template <typename Type>
bool Single_list<Type>::empty() const {
	if(node_count == 0)
		return true;
	else 
		return false;
}

template <typename Type>
Type Single_list<Type>::front() const {
	if(empty())
		throw underflow();
	else
		return head()->retrieve();
}

template <typename Type>
Type Single_list<Type>::back() const {
	if(empty())
		throw underflow();
	else 
		return tail()->retrieve();
}

template <typename Type>
Single_node<Type> *Single_list<Type>::head() const {
	return list_head;
}

template <typename Type>
Single_node<Type> *Single_list<Type>::tail() const {
	return list_tail;
}

template <typename Type>
int Single_list<Type>::count( Type const &obj ) const {
	int count = 0;
	for ( Single_node<Type> *ptr = list_head; ptr != 0; ptr = ptr->next() ) {
		if(ptr->retrieve()==obj){
			count++;
		}
	}
	return count;
}

template <typename Type>
void Single_list<Type>::swap( Single_list<Type> &list ) {
	std::swap( list_head, list.list_head );
	std::swap( list_tail, list.list_tail );
	std::swap( node_count, list.node_count );
}

template <typename Type>
Single_list<Type> &Single_list<Type>::operator = ( Single_list<Type> const &rhs ) {
	Single_list<Type> copy( rhs );

	swap( copy );
	
	return *this;
} 

template <typename Type>
void Single_list<Type>::push_front( Type const &obj ) {
	list_head = new Single_node<Type>( obj, list_head );
	node_count++;
	if(node_count==1){
		list_tail=list_head;
	}
}

template <typename Type>
void Single_list<Type>::push_back( Type const &obj ) {
	if(empty()){
		list_head = new Single_node<Type>( obj, 0 );
		list_tail = list_head;
	}
	else{
		list_tail->next_node = new Single_node<Type>( obj, 0 );
		list_tail = list_tail->next();
	}
	node_count++;
}

template <typename Type>
Type Single_list<Type>::pop_front() {
	
	Type front_value = front();
	if(node_count == 0) {
		throw underflow();
	}
	if (node_count==1)
    {
    	delete list_head;
    	list_tail=0;
    	list_head=0;
    	node_count--;
    	return front_value;
    }
    Single_node<Type> *ptr = list_head;
    list_head = list_head->next();
    delete ptr;
    node_count--;
    return front_value;
	
}

template <typename Type>
int Single_list<Type>::erase( Type const &obj ) {
	int count =0;
	
	 if (empty()) {
	 	return 0;
	 }

	else{
		Single_node<Type> *prev = list_head;
		Single_node<Type> *curr = list_head->next();

		while(list_head->retrieve()==obj){
				pop_front();
				count++;
				if(!empty()){
					prev = list_head;
					curr = list_head->next();
				}
				else
					break;
		}
		while (curr!=0){
			if(curr->retrieve()==obj){
					Single_node<Type> *temp = curr;
					prev->next_node=curr->next();
					curr=curr->next();
					delete temp;
					count++;
					node_count--;
			}
			else{
				prev = curr;
				curr = curr->next();
			}
		}
		if(node_count >= 1)
			list_tail = prev;
	}
	return count;
}
// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator << ( std::ostream &out, Single_list<T> const &list ) {
	for ( Single_node<T> *ptr = list.head(); ptr != 0; ptr = ptr->next() ) {
		out << "->" << ptr->retrieve();
	}

	out << "->0";

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
