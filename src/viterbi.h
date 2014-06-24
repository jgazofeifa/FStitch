/*
 * viterbi.h
 *
 *  Created on: Jan 22, 2014
 *      Author: joeyazo
 */

//Libraries
#include "MEMM_class.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>
#include "functions.h"
using namespace std;

#ifndef VITERBI_H_
#define VITERBI_H_
/*
 * vector<MEMM> is all the data you want to encode
 * parameters: you need a transition matrix, so double A
 * you need to set up a vector of vectors that store the logistic regression
 * 0 index for off and 1 for On
 * for logistic regression, you need weights and slopes
 */

double B(vector<double> xs, vector<logistic_equation> emission_eqs, int i){
	return emission_eqs[i].evaluate(xs);
}

double max(vector<double> array){
	double curr_max = 0;
	for (int i =0; i< array.size(); i++){
		if (array[i] > curr_max){
			curr_max = array[i];
		}
	}
	return curr_max;
}

int argmax(vector<float> array){
	float  * pos = NULL;
	int index;
	for (int i =0; i< array.size(); i++){
		if (pos == NULL){
			pos = &array[i];
			index = i;
		}
		else if (*pos < array[i] ){
			pos = &array[i];
			index = i;
		}
	}
	return index;
}



vector<MEMM> viterbi(vector<MEMM> data_array, vector<vector<double> > A, vector<logistic_equation> emission_eqs, vector<int> feature_dimension, bool SHOW){
	clock_t time;
	int f;
	time = clock();



	//	Initialize Some Program Parameters
	vector<double> xs;
	vector<double> pi(2);
	pi[0]=0.5;
	pi[1]=0.5;

	double MAX;
	vector<float> temp;
	double emit_prob;
	double SUM = 0;
	int k;
	int N = data_array.size();
	vector<int> path;

	//	Initialize Trellis
	vector< vector<float> > trellis(N);
	for (int t = 0; t < N; t++){
		trellis[t] = vector<float>(2);
	}

	//	Initialize Pointers
	vector< vector<int> > pointers(N);
	for (int t = 0; t < N; t++){
		pointers[t] = vector<int>(2);
	}
	double prev_percent = 0;
	vector<double> past_xs;
	vector<double> future_xs;
	int t;
	bool EXIT = 0;
	for (t = 0; t < N; t++){
		xs = data_array[t].getFeatures(feature_dimension);

		if (t == 0){
			for (int i =0; i <2; i++){
				emit_prob = B(xs, emission_eqs, i);
				trellis[t][i] = pi[i] *emit_prob;
				SUM = SUM + trellis[t][i];
			}
		}
		else{
			for (int i =0; i <2; i++){
				emit_prob = B(xs, emission_eqs, i);
				if (emit_prob < 0){
					EXIT 	= 1;
					break;
				}
				for (int j =0; j<2; j++){
					temp.push_back(trellis[t-1][j] *A[j][i]);
				}
				k = argmax(temp);
				MAX = temp[k];

				trellis[t][i] = MAX * emit_prob;
				pointers[t][i] = k;
				SUM = SUM + trellis[t][i];
				temp.clear();

			}
		}
		if (EXIT){
			cout<<"Exiting..."<<endl;
			break;
		}

		if (prev_percent + 0.1 < ((double)t / N)){
			prev_percent = (double)t / (double)N;
		}

		for (int i =0; i < 2; i ++){

			trellis[t][i] = trellis[t][i] / SUM;
		}


		SUM = 0;

	}

	int i;
	t = N-1;
	vector<double> scores;
	while (t > 0){
		if (t == N -1){
			i = argmax(trellis[t]);
			scores.push_back(trellis[t][i]);
			path.push_back(i);
		}
		else {
			i = pointers[t][i];
			scores.push_back(trellis[t][i]);
			path.push_back(i);

		}
		t--;
	}
	int j = 0;
	for (int i = N-1; i >= 0; i--){
		data_array[j].SegCall 	= path[i];
		data_array[j].score 	= scores[i];
		j++;
	}
	if (SHOW){
		cout<<"...viterbi:\t\t\t"<<((float)clock()-time)/CLOCKS_PER_SEC<<" seconds..."<<endl;
	}
	return data_array;
}


#endif /* VITERBI_H_ */
