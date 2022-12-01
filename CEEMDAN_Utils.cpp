#include "CEEMDAN.h"
#include "Spline.h"
using namespace std;


vector<vector<double>> CEEMDAN(vector<double> data, double I, double sd, int max_extr) {
	double epsilon = 0.08;
	int L = int(data.size());
	vector<vector<double>> imfs;
	vector<vector<double>> imfs1;
	vector<double> reside = data;
	const int Imfs_max = 50;

	for (int i = 0; i < I; i++) {
		vector<double> imf1 = imf_n(randomList(reside, epsilon), 1);
		imfs1.emplace_back(imf1);
	}
	imfs.emplace_back(vec2DMeanCol(imfs1));
	cout << "imf 0 is ready" << endl;

	int num = 1;
	bool last_imf = false;
	for (int k = 0; k < Imfs_max; k++) {
		vector<double> imf_prev = imfs[num - 1];
		reside = vecDoubleMinus(reside, imf_prev);
		vector<vector<double>> imfs_num;
		vector<double> imf_num;
		for (int i = 0; i < I; i++) {
			vector<double> imf_num = imf_n(randomList(reside, epsilon), num);
			if (imf_num.empty()) {
				last_imf = true;
				break;
			}
			imfs_num.emplace_back(imf_num);
		}

		if (!last_imf) {
			imf_num = vec2DMeanCol(imfs_num);
		}

		vector<double> maxima_idx, minima_idx;
		vector<double> maxSeq, minSeq;
		vector<double> maxInterSeq(L, 0), minInterSeq(L, 0);

		argrelMaxminList(imf_num, maxima_idx, minima_idx, maxSeq, minSeq);
		if (int(maxima_idx.size()) + int(minima_idx.size()) <= (max_extr)) {
			last_imf = true;
		}

		/* finish criterion by difference in amps */
		if (sdCal(imf_num, imf_prev) < sd)
			last_imf = true;

		if (last_imf) {
			imfs.emplace_back(reside);
			cout << "last imf is ready" << endl;
			break;
		}
		imfs.emplace_back(imf_num);
		cout << "imf" << dec << num << "is ready" << endl;
		num += 1;
	}


	return imfs;
}

vector<double> imf_n(vector<double> data, int num) {
	vector<vector<double>> imfs = EMD(data);
	int len = int(data.size());
	if (int(imfs.size()) > num)
		return imfs[num];
	vector<double> emptylist(0);
	return emptylist;
}

vector<vector<double>> EMD(vector<double> data, double sd) {

	int L = int(data.size());
	vector<double> timeSeq(L, 0);
	for (int i = 1; i < L; i++)
		timeSeq[i] = double(i);
	vector<vector<double>>imfs;
	bool last_imf = false;
	vector<double> residue = data;
	double leftBound = 0, RightBound = 0;	//±ß½çµ¼Êý

	for (int i = 0; i < 20; ++i) {
		vector<double> h_prev = residue;
		vector<double> iterSignal;
		for (int j = 0; j < 50; ++j) {
			vector<double> maxima_idx, minima_idx;
			vector<double> maxSeq, minSeq;
			vector<double> maxInterSeq(L, 0), minInterSeq(L, 0);

			argrelMaxminList(h_prev, maxima_idx, minima_idx, maxSeq, minSeq);
			if (int(maxima_idx.size()) + int(minima_idx.size()) <= 6) {
				last_imf = true;
				break;
			}

			SplineSpace::Spline spmin(
				(double*)minima_idx.data(), &minSeq[0],
				int(minima_idx.size()), SplineSpace::GivenSecondOrder,
				leftBound, RightBound);
			spmin.MultiPointInterp(&timeSeq[0], L, &minInterSeq[0]);

			SplineSpace::Spline spmax(
				(double*)maxima_idx.data(), &maxSeq[0],
				int(maxima_idx.size()), SplineSpace::GivenSecondOrder,
				leftBound, RightBound);
			spmax.MultiPointInterp(&timeSeq[0], L, &maxInterSeq[0]);

			iterSignal = meanNewSeq(h_prev, maxInterSeq, minInterSeq);
			if (sdCal(iterSignal, h_prev) < sd)
				break;

			h_prev = iterSignal;
		}

		if (last_imf)
			break;

		imfs.emplace_back(iterSignal);
		residue = vecDoubleMinus(residue, iterSignal);

	}
	imfs.emplace_back(vecDoubleMinus(data, vec2DMeanCol(imfs)));
	return imfs;
}


vector<double> vec2DMeanCol(const vector<vector<double>> data) {
	int W = int(data.size());
	int M = int(data[0].size());

	vector<double> resCol(M, 0);

	for (int i = 0; i < W; i++) {
		for (int j = 0; j < M; j++) {
			resCol[j] += data[i][j];
		}
	}
	for (int j = 0; j < M; j++) {
		resCol[j] /= double(W);
	}

	return resCol;
}

vector<double> randomList(vector<double> data, double epsilon) {
	std::default_random_engine gen;
	std::normal_distribution<double> dis(0, 1);
	vector<double> newData = data;
	int L = int(newData.size());
	for (int i = 0; i < L; i++) {
		newData[i] += epsilon * dis(gen);
	}
	return newData;
}

double vec2DMean(vector<vector<double>> data) {
	int M = int(data.size());
	int N = int(data[0].size());
	double sum = 0.0;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			sum += data[i][j];
		}
	}
	return (sum / (M * N));
}

double sdCal(vector<double> signal, vector<double> signal_prev) {
	double sd = 0, sqsum = 0, diffsqsum = 0;
	int L = int(signal_prev.size());
	if (signal.empty())
		signal.resize(L, 0);
	for (int i = 0; i < L; i++) {

		sqsum += signal_prev[i] * signal_prev[i];
		diffsqsum += (signal[i] - signal_prev[i]) * (signal[i] - signal_prev[i]);
	}
	//      # sifting criterion
	sd = diffsqsum / sqsum;
	return sd;
}

vector<double> vecDoubleMinus(vector<double> subtractee, vector<double> subtracter) {
	int L = int(subtractee.size());
	vector<double> diff(L, 0);
	for (int i = 0; i < L; i++) {
		diff[i] = subtractee[i] - subtracter[i];
	}
	return diff;
}

vector<double> vecDoubleMinus(vector<double> subtractee, double subtracter) {
	int L = int(subtractee.size());
	vector<double> diff(L, 0);
	for (int i = 0; i < L; i++) {
		diff[i] = subtractee[i] - subtracter;
	}
	return diff;
}

vector<double> meanNewSeq
(vector<double> signal_prev,
	vector<double> &maxima_vals, vector<double> &minima_vals) {

	/*
	mean = 0.5*(maxima_vals + minima_vals)
	h = h_prev - mean
	*/

	int L = int(signal_prev.size());
	vector<double> h(L, 0);
	for (int i = 0; i < L; i++) {
		h[i] = signal_prev[i] - (maxima_vals[i] + minima_vals[i]) * 0.5;
	}
	return h;
}

static void argrelMaxminList(
	vector<double> signal,
	vector<double>& maxlist, vector<double>& minlist,
	vector<double> &maxSiglist, vector<double>& minSiglist
	) {
	maxlist.resize(1.0, 0);
	minlist.resize(1.0, 0);
	int L = int(signal.size());
	if (L <= 2)
		return;

	maxSiglist.resize(1, signal[0]);
	minSiglist.resize(1, signal[0]);

	for (int i = 1; i <= L-2; i++){
		if (signal[i] > signal[i - 1]  && signal[i] > signal[i + 1]){
			maxlist.emplace_back(double(i));
			maxSiglist.emplace_back(signal[i]);
		}
		else if (signal[i] < signal[i - 1] && signal[i] < signal[i + 1]){
			minlist.emplace_back(double(i));
			minSiglist.emplace_back(signal[i]);
		}
	}

	maxlist.emplace_back(double(L-1));
	minlist.emplace_back(double(L - 1));
	maxSiglist.emplace_back(signal[L - 1]);
	minSiglist.emplace_back(signal[L - 1]);
	return;
}






