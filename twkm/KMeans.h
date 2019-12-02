#pragma once
#include <stdlib.h>
#include <string>
#include "Data.h"

class KMeans{
public:
	int k;
	double **centroides;
	double fobj;
	double *featureWeight;
	double *groupWeight;		
public:
	KMeans(int , int);
	~KMeans(void);
	void run(Data *,int, int, int, int, double eta, double lambda);
	void updateFeatureWeights(int * nr, int * nc, int * k, double * eta, int * numGroups, Data * data);
	void updateViewWeights(int * nr, int * nc, int * k, double * lambda, int * numGroups, Data * data);
	void f(Data *, int n, int k, int p, double eta, double lambda);
	
};

