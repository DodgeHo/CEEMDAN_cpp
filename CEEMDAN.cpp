#include "CEEMDAN.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;
/*
<CEEMDAN_CPP: C++ implementation of Variational Mode Decomposition using Eigen.>
Copyright (C) <2022>  <Lang He: asdsay@gmail.com>
Mozilla Public License v. 2.0.
*/

int main() {

    vector<double> testSignal;
    ifstream inFile("Test_Signal.csv", ios::in);
    if (!inFile)
    {
        cout << "打开文件失败！" << endl;
        exit(1);
    }
    int i = 0;
    string line;
    string field;
    while (getline(inFile, line))//getline(inFile, line)表示按行读取CSV文件中的数据
    {
        string field;
        istringstream sin(line); //将整行字符串line读入到字符串流sin中
        getline(sin, field); //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符 
        double num = double(atof(field.c_str()));//将刚刚读取的字符串转换成float
        testSignal.emplace_back(num);
        i++;
    }
    inFile.close();
    cout << "共读取了：" << i << "行" << endl;
    cout << "读取数据完成" << endl;

    vector<vector<double>> CEEMDAN_RESULT = CEEMDAN(testSignal, 500, 0.1, 2);
    int M = int(CEEMDAN_RESULT.size());
    int N = int(CEEMDAN_RESULT[0].size());
    ofstream outFile("test_result_1.csv", ios::out);

    if (!outFile)
    {
        cout << "打开文件失败！" << endl;
        exit(1);
    }
    //写入3行数据
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            outFile << CEEMDAN_RESULT[i][j] << ",";
        }
        outFile << endl;
    }
    outFile.close();
	return 0;
}; 