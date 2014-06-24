/*
 * readIn.h
 *
 *  Created on: Jan 22, 2014
 *      Author: joeyazo
 */

#include "MEMM_class.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>
#include <time.h>
using namespace std;
#ifndef READIN_H_
#define READIN_H_

vector<MEMM> readInBinFile(string fileName, int test = 0){
	clock_t time = clock();
	vector<MEMM> struct_array;



	MEMM memmObj;
	ifstream binaryFile(fileName, ios::binary|ios::in);
	binaryFile.seekg(0, ios::beg);
	int threshold = 1000;
	if (binaryFile.is_open()){
		while (1){
			binaryFile.read((char*)&memmObj, sizeof(MEMM));
			if (binaryFile.eof()){
				break;
			}
			struct_array.push_back(memmObj);
		}
		binaryFile.close();
	}
	else{
		cout<<"Unable to Open Binary File: "<<fileName<<endl;
		return struct_array;
	}
	return struct_array;
}





vector<MEMM> readInFile(string fileName, string strand, bool test = 0)
{
	cout<<"Reading In: "<<fileName<<endl;
	vector<string> lineInfo;
	vector<MEMM> struct_array;
	string line;
	string coverages;
	string chrom;
	string token;
	string comma = ",";
	string delimiter = "\t";
	int pos, start, stop, prev, begin;
	ifstream myfile(fileName);
	double sum;
	int N;
	int length;
	bool header = 1;
	MEMM MEMM_struct;
	bool GENE_BOOL;
	int ct = 0;
	int test_threshold = 1000;
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			if (ct > test_threshold){
				break;
			}
			if (not header){
				pos=0;
				GENE_BOOL = 0;
				while ((pos = line.find(delimiter) ) != string::npos){
					token = line.substr(0, pos);
					line.erase(0, pos+delimiter.length());
					lineInfo.push_back(token);
				}
				lineInfo.push_back(line);
				start = atoi(lineInfo[1].c_str());
				stop  = atoi(lineInfo[2].c_str());
				length = atof(lineInfo[3].c_str());
				coverages = lineInfo[5];
				chrom = lineInfo[0];
				sum = 0;
				pos = 0;
				N = 0;
				while ((pos = coverages.find(comma)) != string::npos){
					sum = sum+atof(coverages.substr(0, pos).c_str());
					coverages.erase(0, pos+comma.length());
					N++;
				}
				N++;
				sum = sum + atoi(coverages.c_str());
				if (lineInfo[4] != "N_A"){
					GENE_BOOL = 1;
				}

				MEMM_struct.set_values(start, stop, length,sum / double(N), GENE_BOOL, lineInfo[4], strand, chrom);
				struct_array.push_back(MEMM_struct);
				lineInfo.clear();

				ct++;
			}
			else{
				header = 0;
			}




		}
		myfile.close();
	}
	else{
		cout << "Unable to open file"<<endl;
	}
	return struct_array;
}


#endif /* READIN_H_ */
