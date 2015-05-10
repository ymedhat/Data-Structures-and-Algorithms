#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

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
#include "ece250.h"

enum state { EMPTY, OCCUPIED, DELETED };

template<typename Type>
class Double_hash_table {
	private:
		int count;
		int power;
		int array_size;
		int mask;
		Type *array;
		state *occupied;

		int h1( Type const & ) const;
		int h2( Type const & ) const;

	public:
		Double_hash_table( int = 5 );
		~Double_hash_table();
		int size() const;
		int capacity() const;
		double load_factor() const;
		bool empty() const;
		bool member( Type const & ) const;
		Type bin( int ) const;

		void insert( Type const & );
		bool remove( Type const & );
		void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator << ( std::ostream &, Double_hash_table<T> const & );
};

//Constructor
template<typename Type>
Double_hash_table<Type>::Double_hash_table( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
mask( array_size - 1 ),
array( new Type[array_size] ),
occupied( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = EMPTY;
	}
}
//De-constructor
template<typename Type>
Double_hash_table<Type>::~Double_hash_table(){
		delete [] array;
        delete [] occupied;
        count = 0;
}

//h1 Method - Finds the initial hash value of where 
//an object is supposed to go 
template<typename Type >
int Double_hash_table<Type>::h1( Type const &obj ) const 
{
	int hash = static_cast<int>(obj)%array_size;
	if (hash < 0){
		return hash+array_size;
	}
	else
		return hash;
}

//h2 Method - Finds the value you should jump by within the array
template<typename Type >
int Double_hash_table<Type>::h2( Type const &obj ) const 
{
	int bin = static_cast<int>(obj);
	bin = (bin/array_size)%(array_size);

	if (bin < 0)
		 bin = bin+array_size;

	if (bin % 2 == 0){
		bin++;
	}
	return bin;
}

//Size Method - Returns the number of elements 
//within the array
template<typename Type>
int Double_hash_table<Type>::size() const{
	return count;
}

//Capacity Method - Returns the total number 
//of possible elements in the array
template<typename Type>
int Double_hash_table<Type>::capacity() const{
	return array_size;
}

//Load Factor Method - Returns a ratio of how filled
//the array is
template<typename Type>
double Double_hash_table<Type>::load_factor() const{
	return static_cast<double>(count)/ array_size;
}

//Empty Method - Checks if the array is empty
//returns true if empty, otherwise false
template<typename Type>
bool Double_hash_table<Type>::empty() const{
	if(count == 0)
		return true;
	else 
		return false;
}

//Member Method - Checks if a specific integer is within an 
//index of the array using the initial hash index and then the 
//value of the jump 
template<typename Type>
bool Double_hash_table<Type>::member( Type const &obj ) const{
	int initial = h1(obj);
	int secondary = h2(obj);
	int index = (initial+secondary)%array_size;

	//Case 1 - If located in the initial index
	//without any jumps within the hash table
	if(array[initial]==obj){
		if(occupied[initial]==OCCUPIED)
			return true;
	}

	//Case 2 - If located within an index that is a
	//multiple of the value of the jump. Treats the array
	//as a cyclical array using mods
	else{
		for(int i = 0; i<array_size;i++){
			if(array[index]==obj){
				if(occupied[index]==OCCUPIED)
					return true;
				else
					return false;
			}
			index = (index + secondary)%array_size;
		}
	}
	return false;
}

//Bin Method - Checks what the value at a specific bin is
template<typename Type>
Type Double_hash_table<Type>::bin( int obj ) const{
	return array[obj];
}

//Insert Method - Searches the array to see if the value of the obj is 
//already located within the array, if not then it searches for the most
//appropriate location within the array
template<typename Type>
void Double_hash_table<Type>::insert( Type const &obj ){
	//Case 1 - If it is in the array
	if(member(obj)==true)
		return;

	//Case 2 - Array is filled and objects cannot be inserted
	if (count== capacity()){
		throw overflow();
	}

	//Case 3 - There is a spot where when searched can be found to 
	//allow an object to be inserted in 
	else{
		int initial = h1(obj);
		int secondary = h2(obj);
		int index = (initial+secondary)%array_size;

		//Treats the array cyclically 
		for(int i = 0; i<array_size;i++){
			//Case 4 - Initial spot where an object is supposed to go
			//only if it is empty
			if(occupied[initial]== EMPTY || occupied[initial]==DELETED){
				array[initial]=obj;
				occupied[initial]=OCCUPIED;
				count++;
				return;
			}

			//Case 5 - Uses the jump value to jump through the list
			//till it finds an empty or deleted spot allowing to insert
			//within the array
			if(occupied[index]==EMPTY || occupied[index]==DELETED){
				array[index]=obj;
				occupied[index]=OCCUPIED;
				count++;
				return;
			}
			//Calculates the next initial value by the jump value
			else{
				index = (index + secondary)%array_size;
			}
		}
	}
}

//Remove Method - Deletes a specific object within the array 
//but only changes its status rather than genuinely deleting it
template<typename Type>
bool Double_hash_table<Type>::remove( Type const &obj ){

	//Case 1 - If not in list
	if (member(obj)==false)
		return false;

	//Case 2 - In list
	else{
		int initial = h1(obj);
		int secondary = h2(obj);
		int index = (initial+secondary)%array_size;


		for(int i = 0; i<array_size;i++){
			//Case 3 - Located at the initial hash expected index
			if(array[initial]==obj){
					occupied[initial]=DELETED;
					count--;
					return true;
			}

			//Case 4 - Located at another index depending on the
			//jump value 
			if(array[index]==obj){
						occupied[index]=DELETED;
						count--;
						return true;
			}
			//Calculates the index by the jump value using a cyclical array
			else{
				index = (index + secondary)%array_size;
			}
		}
	}
}

//Clear Method - Deletes all data stored within the array
template<typename Type>
void Double_hash_table<Type>::clear(){
	delete [] array;
	delete [] occupied;
	count = 0;

	//Re-initiates all the arrays
	Type *array = new Type [array_size]; 
	Type *occupied = new Type [array_size]; 
	//Sets all status to empty within the array
	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = EMPTY;
	}
}


template <typename T>
std::ostream &operator << ( std::ostream &out, Double_hash_table<T> const &hash ) {
	for ( int i = 0; i < hash.capacity(); ++i ) {
		if ( hash.occupied[i] == EMPTY ) {
			out << "- ";
		} else if ( hash.occupied[i] == DELETED ) {
			out << "x ";
		} else {
			out << hash.array[i] << ' ';
		}
	}

	return out;
}

#endif
