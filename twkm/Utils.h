#pragma once

#include <string>
#include "Data.h"

#define FPOINT_OFFSET 1e-9

using namespace std;

class Utils{
public:
	Utils(void);
	~Utils(void);
	double precision(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int i, unsigned int j);
	double purity(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k);
	double recall(unsigned int *apriori, unsigned int *clustering, unsigned int n,unsigned int i, unsigned int j);
	double fmeasure(unsigned int *apriori, unsigned int *clustering, unsigned int n,unsigned int i, unsigned int j);
	double fmeasuret(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int m, unsigned int k);
	double nmi(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k);
	double cr_index(unsigned int *apriori, unsigned int *clustering, unsigned int n);
	double rand_index(unsigned int *apriori, unsigned int *clustering, unsigned int n);
	double jaccard_index(unsigned int *apriori, unsigned int *clustering, unsigned int n);
	double entropyC_index(unsigned int *clustering, unsigned int n, unsigned int k);
	double entropyP_index(unsigned int *apriori, unsigned int n, unsigned int k);
	double mutual_information_index(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k);
	double nmi2(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k);
	double oerc(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k);
	double euclidianDistance(Data rd1, Data rd2);
	double euclidianDistance(Data rd1, Data rd2, unsigned int p);
	void run( double *medias, double *desvios, double **indices, unsigned int *sol, double *bfobj, unsigned int *bsol, double *bestbfobj, unsigned int nbRep,  unsigned int nbObj, Data *data, unsigned int k, unsigned int p, double **matrizes, unsigned int **g, ofstream *file1, unsigned int *apriori, unsigned int *finalclustering, double eta, double lambda);
	double euclideanDistance(Data rd1, Data rd2);
	double euclideanDistance(Data rd1, Data rd2, unsigned int p);
	
	Data* chooseDataset(unsigned int dataset, unsigned int *p, unsigned int *k,  unsigned int *nbObj, bool normalized);
	
	Data* readImageSegmentation(bool normalized);
};
