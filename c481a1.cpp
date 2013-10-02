#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <cmath>

using namespace std;

struct threadParams {
	int start;	//index at which to start calc
	int count;	//how many indeces to calc
	int n;		//size of matrix
	int i; 		//index of thread (starting at 0)
};

bool isInteger(const string & s);
int getIntegerInput(string prompt);
void generateMatrices(int n);
void *threadMain(void *arg);

//global matrices
int **matrixA;
int **matrixB;
int **matrixC;	// result matrix

int main(void) {
	srandom(2);
	int num_threads = 0;		//number of threads to generate
	int n = 0;					//size of square matrix (n x n)

	//get input for number of threads
	num_threads = getIntegerInput("Enter the number of threads:");
	n = getIntegerInput("Enter size of the matrix:");

	//initialize thread pointer array
	pthread_t ThreadId[num_threads];

	generateMatrices(n);

	/*n = 3;

	matrixA[0][0] = 1;
	matrixA[0][1] = 2;
	matrixA[0][2] = 3;
	matrixA[1][0] = 4;
	matrixA[1][1] = 5;
	matrixA[1][2] = 6;
	matrixA[2][0] = 7;
	matrixA[2][1] = 8;
	matrixA[2][2] = 9;

	matrixB[0][0] = 1;
	matrixB[0][1] = 2;
	matrixB[0][2] = 3;
	matrixB[1][0] = 4;
	matrixB[1][1] = 5;
	matrixB[1][2] = 6;
	matrixB[2][0] = 7;
	matrixB[2][1] = 8;
	matrixB[2][2] = 9;*/

	//split up the matrix indices (where index i is row floor(i/n), column n - floor(i/n) )
	int calcs = n*n;							//number of calcs that need to be performed
	int countMost = floor((calcs)/num_threads);	//number of calcs to be performed by num_threads - 1 threads
	int countLast = calcs - countMost*(num_threads - 1);	//number of calcs to be performed by the last thread

	cout << "countMost: " << countMost << "| countLast: " << countLast << endl;

	//START TEST TIMER
	int startTime = time(0);

	threadParams tps[num_threads];	//array of thread params for each thread

	int currI = 0;	// keep track of which index we're assigning to the thread to start on
	for(int i = 0; i < num_threads; i++ ){
		tps[i].start = currI;
		tps[i].count = (i < num_threads - 1) ? countMost : countLast;
		tps[i].n = n;
		tps[i].i = i;

		//if not on last thread, increment start by countMost
		if (i < num_threads - 1){
			currI += countMost;
		}

		pthread_create( &( ThreadId[ i ] ), NULL, threadMain, &tps[i]);
	}

	for(int i = 0; i < num_threads; i++ ){
		pthread_join(ThreadId[i], NULL);
	}

	//END TEST TIMER
	int endTime = time(0);

	/*cout << endl << "result matrix C:" << endl;
		for(int i = 0; i < n; i++){
			cout << "[";
			for(int j = 0; j < n; j++){
				int val = matrixC[i][j];
				string space;
				//space = (val < 1000) ? " " : ""; cout << space;
				space = (val < 100) ? " " : ""; cout << space;
				space = (val < 10) ? " " : ""; cout << space;
				cout << " " << val;
			}
			cout << " ]" << endl;
		}
	*/
	cout << "total time " << (endTime - startTime) << endl;

	//cleanup
	delete[] matrixA;
	delete[] matrixB;
	delete[] matrixC;

	return EXIT_SUCCESS;
}


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

void generateMatrices(int n){
	matrixA = new int*[n];
	matrixB = new int*[n];
	matrixC = new int*[n];

	for(int i = 0; i < n; i++){
		matrixA[i] = new int[n];
		matrixB[i] = new int[n];
		matrixC[i] = new int[n];

		for(int j = 0; j < n; j++){
			matrixA[i][j] = random() % 1000;
			matrixB[i][j] = random() % 1000;
		}
	}

	//print matrices
	/*cout << "multiplying matrix A:" << endl;
	for(int i = 0; i < n; i++){
		cout << "[";
		for(int j = 0; j < n; j++){
			int val = matrixA[i][j];
			string space;
			//space = (val < 1000) ? " " : ""; cout << space;
			space = (val < 100) ? " " : ""; cout << space;
			space = (val < 10) ? " " : ""; cout << space;
			cout << " " << val;
		}
		cout << " ]" << endl;
	}

	cout << endl << "by matrix B:" << endl;
	for(int i = 0; i < n; i++){
		cout << "[";
		for(int j = 0; j < n; j++){
			int val = matrixB[i][j];
			string space;
			//space = (val < 1000) ? " " : ""; cout << space;
			space = (val < 100) ? " " : ""; cout << space;
			space = (val < 10) ? " " : ""; cout << space;
			cout << " " << val;
		}
		cout << " ]" << endl;
	}*/
}

//test thread function
void *threadMain(void* arg){
	threadParams* args = (threadParams*) arg;

	for (int i = args->start; i < args->start+args->count; i++){
		int row = (int)floor((double)(i/args->n));
		int col = (i % args->n);

		int val = 0;

		for(int j = 0; j < args->n; j++){
			val += matrixA[row][j] * matrixB[j][col];
		}

		matrixC[row][col] = val;
	}

}
