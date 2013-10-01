//============================================================================
// Name        : c481a1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>

using namespace std;

//return true if string is integer
bool isInteger(const string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

//test thread function
void *testFunction(void*){
    cout << "Threading\n";
}

int main(void) {
	int num_threads = 0;
	string input = "";

	//get input for nubmer of threads
	cout << "Enter the number of threads:" << endl;
	getline(cin, input);

	//repeat until integer is inputted
	while (!isInteger(input)){
		cout << "Please enter an integer:" << endl;
		getline(cin, input);
	}

	// set number of threads
	num_threads = atoi(input.c_str());
	pthread_t ThreadId[num_threads];

	for(int i = 1; i < num_threads; i++ ){
		pthread_create( &( ThreadId[ i ] ), NULL, testFunction, (void*)&( TCB[ i ] ) );
	}


	return EXIT_SUCCESS;
}
