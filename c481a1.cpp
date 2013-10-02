#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <cmath>
#include <fstream>
#include <sstream>

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
double getAvg(int array[], int size);
int runCalculation(int n, int num_threads);

//global matrices
int **matrixA;
int **matrixB;
int **matrixC;	// result matrix

int main(void) {
	//timer for entire experiment
	int startTime = time(0);

	int sizes[] = {1024, 1200, 1400, 1600, 1800, 2048, 2200};			//array of matrix sizes to perform calculations on
	
	//for each size
	for(int a = 0; a < 7; a++){
		int n = sizes[a];
		
		cout << "-Size: " << n << endl;
		
		//compile filename
		ostringstream convert;
		convert << n;
		string filename = convert.str();
		filename += ".txt";
		
		//new file to write to
		ofstream file;
		file.open(filename.c_str());
		
		// seed the RNG and regenerate matrices for every size 
		srandom(time(0));		
		generateMatrices(sizes[a]);
		
		//perform 5 trials per thread count per matrix size
		int trials[5];
		
		//perform matrix mult using 5 different numbers of threads
		int threads[] = {1,2,3,4,8};		
		for(int t = 0; t < 5; t++){
			int num_threads = threads[t];
			cout << "--Threads: " << num_threads << endl;
			for(int b = 0; b < 5; b++){
				cout << "---trial: " << b << "| result...";
				int result = runCalculation(n, num_threads);
				cout << result << endl;
				trials[b] = result;
			}		
			double avg = getAvg(trials,5);
			cout << "---avg: " << avg << endl;
			file << num_threads << "," << avg << endl;
		}

		file.close();		
		
		//cleanup
		delete[] matrixA;
		delete[] matrixB;
		delete[] matrixC;
	}
	
	int endTime = time(0);
	cout << endl << "Total time: " << (endTime - startTime) << endl;
	return EXIT_SUCCESS;
}

//generate 2 random dense matrices with values from 0 to 999
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
}

//get average of ints in array
double getAvg(int array[], int size){
	double sum = 0.0;
	for(int i = 0 ; i < size; i++){
		sum += array[i];
	}
	
	return (double) sum/size;
}

//perform matrix multiplication of size n x n using num_threads threads
int runCalculation(int n, int num_threads){
	//get input for number of threads
	/*num_threads = getIntegerInput("Enter the number of threads:");
	n = getIntegerInput("Enter size of the matrix:");*/
	int startTime = 0;
	
	//if more than 1 thread, perform parallel mat mult
	if (num_threads > 1){
	
		//initialize thread pointer array
		pthread_t ThreadId[num_threads];
		
		//split up the matrix indices (where index i is row floor(i/n), column n - floor(i/n) )
		int calcs = n*n;							//number of calcs that need to be performed
		int countMost = floor((calcs)/num_threads);	//number of calcs to be performed by num_threads - 1 threads
		int countLast = calcs - countMost*(num_threads - 1);	//number of calcs to be performed by the last thread

		//START TIMER
		startTime = time(0);

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
		
	//else if only 1 thread, perform linear matrix mult
	} else {
		startTime = time(0);
		for (int i = 0; i < n; ++i) { 
			for (int j = 0; j < n; ++j) { 
				for (int k = 0; k < n; ++k) {
					matrixC[i][j] += matrixC[i][k] * matrixC[k][j]; 
				}
			} 
		}
	}

	//END TEST TIMER
	int endTime = time(0);

	return (endTime - startTime);
}

//test thread function
void *threadMain(void* arg){
	threadParams* args = (threadParams*) arg;

	for (int i = args->start; i < args->start + args->count; i++){
		int row = floor(i/args->n);
		int col = i - (args->n * row);
		//int col = (i % args->n);

		int val = 0;

		for(int j = 0; j < args->n; j++){
			val += matrixA[row][j] * matrixB[j][col];
		}

		matrixC[row][col] = val;
	}
}

