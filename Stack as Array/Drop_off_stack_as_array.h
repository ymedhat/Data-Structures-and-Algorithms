#ifndef DROP_OFF_STACK_AS_ARRAY_H
#define DROP_OFF_STACK_AS_ARRAY_H

/*****************************************
 * UW User ID:  ymibrahi
 * Submitted for ECE 250
 * Semester of Submission:  (Fall) 2013
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#include "Exception.h"

template <typename Type>
class Drop_off_stack_as_array {
	private:
		int itop;
		int ibottom;
		int entry_count;
		int array_capacity;
		Type *array;

	public:
		Drop_off_stack_as_array( int = 10 );
		Drop_off_stack_as_array( Drop_off_stack_as_array const & );
		~Drop_off_stack_as_array();

		int size() const;
		bool empty() const;
		Type top() const;

		void swap( Drop_off_stack_as_array & );
		Drop_off_stack_as_array &operator = ( Drop_off_stack_as_array );
		void push( Type const & );
		Type pop();
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator << ( std::ostream &, Drop_off_stack_as_array<T> const & );
};

template <typename Type>
Drop_off_stack_as_array<Type>::Drop_off_stack_as_array( int n ):
//Constructs all values initially
//with 0 and compares capacity with 
//zero to allow for positive values always
itop(0),
ibottom(0),
entry_count(0),
array_capacity( std::max(0, n) ),
array(new Type[array_capacity])
{
	//Empty Constructor
}

template <typename Type>
Drop_off_stack_as_array<Type>::Drop_off_stack_as_array( Drop_off_stack_as_array<Type> const &stack ):
itop( stack.itop ),
ibottom( stack.ibottom ),
entry_count( stack.entry_count ),
array_capacity( array_capacity ),
array( new Type[array_capacity] ) {
	// The above initializations copy the values of the appropriate
	// member variables and allocate memory for the data structure; 
	// however, you must still copy the stored objects.

//iterates through the stack array
//and copies all values into new array given 
//arguements 
	int i=0;
	while (i<=entry_count){
		array[i]=stack.array[i];
		i++;
	}
}

template <typename Type>
Drop_off_stack_as_array<Type>::~Drop_off_stack_as_array() {
//deletes all contents of array
	delete [] array;
}

template <typename Type>
int Drop_off_stack_as_array<Type>::size() const {
//Returns the integer value of entry count
//to show how many values are in array
	return entry_count;
}

template <typename Type>
bool Drop_off_stack_as_array<Type>::empty() const {
//Returns true if array is empty
//false if has values in list (entry count>0)
	if (entry_count==0){
		return true;
	}
	else
		return false;
}

template <typename  Type>
Type Drop_off_stack_as_array<Type>::top() const {
//Checks if list is empty and throws underflow
	if (empty())
		throw underflow();
//Returns value of the top of list -1 due to indexing
//of array starting at zero
	else
		return array[entry_count - 1];
}

template <typename Type>
void Drop_off_stack_as_array<Type>::swap( Drop_off_stack_as_array<Type> &stack ) {
	std::swap( itop, stack.itop );
	std::swap( ibottom, stack.ibottom );
	std::swap( entry_count, stack.entry_count );
	std::swap( array_capacity, stack.array_capacity );
	std::swap( array, stack.array );
}

template <typename Type>
Drop_off_stack_as_array<Type> &Drop_off_stack_as_array<Type>::operator = ( Drop_off_stack_as_array<Type> rhs ) {
	swap( rhs );
	
	return *this;
} 

template <typename Type>
void Drop_off_stack_as_array<Type>::push( Type const &obj ) {
//Case 1 - checks if empty and stores value at bottom of stack
	if (empty()){
		itop = 0;
		ibottom = 0;
		array[ibottom]=obj;
		++entry_count;
	}
//Case 2 - when stack is full, modular is used to 
//find the next value of ibottom and itop
//and stores value into the new itop (oldest in array)	
	else if ( entry_count == array_capacity ) {
	    ibottom=(ibottom+1)%array_capacity;
	    itop=(itop+1)%array_capacity;
		array[itop] = obj;
	}
//Case 3 - normal case where itop has to iterate its value
//through the array and stores value in new itop index in the array
	else{
	itop=(itop+1)%array_capacity;
	array[itop] = obj;
	++entry_count;
	}
}

template <typename Type>
Type Drop_off_stack_as_array<Type>::pop() {
//Case 1 - If empty throws overflow
	if ( empty() ) {
		throw underflow();
	}
//Case 2 - Contains elements within the array	
	else{
		//stores initial value to be deleted
		Type obj = array [itop];
		//Checks if itop is at bottom of stack
		//and restores to top of stack
		if((itop-1)<0){
			itop=array_capacity-1;
		}
		//Normal case where it shifts itop
		//index through stack 
		else{
		itop=(itop-1)%array_capacity;
		}
		//Reduces number of elements in array
		--entry_count;

		//Returns deleted item previously stored
		return obj;
	}
}

template <typename Type>
void Drop_off_stack_as_array<Type>::clear() {
//Resets all values and initializes a new array 
	itop = 0;
	ibottom = 0;
	entry_count = 0;
	delete [] array;
	array = new Type [array_capacity];
}

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator << ( std::ostream &out, Drop_off_stack_as_array<T> const &stack ) {
	// I don't know how you are implementing your stack so I cannot print it.
	// If you want, you can print whatever you want here and then call cout
	// in the driver.

	// Remember to redirect to out, e.g.,
	//      out << "Hello!";

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
