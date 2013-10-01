#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <pthread.h>

using namespace std;

//return true if string is integer
bool isInteger(const string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

//get integer input from user
int getIntegerInput(string prompt){
	string input;
	cout << prompt << endl;
	getline(cin, input);

	//repeat until integer is inputted
	while (!isInteger(input)){
		cout << "Please enter an integer:" << endl;
		getline(cin, input);
	}

	return atoi(input.c_str());
}

//test thread function
void *testFunction(void* ptr){
    int x = 2;

    for(int i = 0; i < 1000000000; i++){
    	x = x * 3;
    }

    cout << x << endl;
}

int main(void) {
	int num_threads = 0;		//number of threads to generate
	int n = 0;					//size of square matrix (n x n)

	//get input for number of threads
	num_threads = getIntegerInput("Enter the number of threads:");
	n = getIntegerInput("Enter size of the matrix:");

	//intialize thread pointer array
	pthread_t ThreadId[num_threads];

	//START TEST TIEMR
	int startTime = time(0);

	for(int i = 0; i < num_threads; i++ ){
		pthread_create( &( ThreadId[ i ] ), NULL, testFunction, NULL );
		//pthread_join(ThreadId[i], NULL);
	}

	for(int i = 0; i < num_threads; i++ ){
		pthread_join(ThreadId[i], NULL);
	}

	int endTime = time(0);

	cout << "total time " << (endTime - startTime) << endl;

	return EXIT_SUCCESS;
}
