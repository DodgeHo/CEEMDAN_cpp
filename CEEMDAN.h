#pragma once
#include <vector>
#include "Spline.h"
#include <random>
#include <iostream>
using namespace std;


vector<vector<double>> CEEMDAN(vector<double> data, double I, double sd, int max_extr);

vector<double> imf_n(vector<double> data, int num);

vector<vector<double>> EMD(vector<double> data, double sd = 0.1);


vector<double> vec2DMeanCol(const vector<vector<double>> data);

vector<double> randomList(vector<double> data, double epsilon);

double vec2DMean(vector<vector<double>> data);

vector<double> vecDoubleMinus(vector<double> subtractee, double subtracter);

vector<double> vecDoubleMinus(vector<double> subtractee, vector<double> subtracter);

double sdCal(vector<double> signal, vector<double> signal_prev);

vector<double> meanNewSeq(vector<double> signal_prev,
	vector<double>& maxima_vals, vector<double>& minima_vals);

static void argrelMaxminList(vector<double> signal,
	vector<double>& maxlist, vector<double>& minlist,
	vector<double>& maxSiglist, vector<double>& minSiglist);


