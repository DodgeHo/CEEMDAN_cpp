#include "CEEMDAN.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#define FAILMESSAGE "Fail to Open File"
using namespace std;

/*
<CEEMDAN_CPP: C++ implementation of Variational Mode Decomposition using Eigen.>
Copyright (C) <2022>  <Lang He: asdsay@gmail.com>
Mozilla Public License v. 2.0.
*/

int main() {

	vector<double> testSignal;
	ifstream inFile("Test_Signal.csv", ios::in);
	if (!inFile){
		cout << FAILMESSAGE << endl;
		exit(1);
	}
	int i = 0;
	string line;
	string field;
	while (getline(inFile, line)) {
		//Read data from CSV file by line
		string field;
		istringstream sin(line); //transfer line into stringstrem sin
		getline(sin, field); //sin will be seperated by comma
		double num = double(atof(field.c_str()));//turn the string to be float
		testSignal.emplace_back(num);
		i++;
	}
	inFile.close();
	cout << "Totally written:" << i << "lines" << endl;
	cout << "Finish reading" << endl;

	vector<vector<double>> CEEMDAN_RESULT = CEEMDAN(testSignal, 500, 0.1, 2);
	int M = int(CEEMDAN_RESULT.size());
	int N = int(CEEMDAN_RESULT[0].size());
	ofstream outFile("test_result_1.csv", ios::out);

	if (!outFile){
		cout << FAILMESSAGE << endl;
		exit(1);
	}

	//write data
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			outFile << CEEMDAN_RESULT[i][j] << ",";
		}
		outFile << endl;
	}
	outFile.close();
	return 0;
};