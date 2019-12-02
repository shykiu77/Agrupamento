#include "KMeans.h"
#include <string>
#include <time.h>
#include <math.h>
#include <stdio.h>

using namespace std;

KMeans::KMeans(int k, int c){
	
}

KMeans::~KMeans(void){
}	

void KMeans::run(Data *data, int n, int k, int c, int p, double eta, double lambda) {
	fobj = 10000;
	featureWeight = (double *)malloc(sizeof(double)*data[0].nbAtt);

	int j, *nums;
	nums = (int *)calloc(p, sizeof(int));
	for (j = 0; j < c; ++j) {
		nums[data[0].groupInfo[j]]++;
	}
	for (j = 0; j < c; ++j)
		featureWeight[j] = 1.0 / nums[data[0].groupInfo[j]];

	groupWeight = (double *)malloc(sizeof(double)*p);

	for (int i = 0; i < p; i++)
		groupWeight[i] = 1.0 / p;

	centroides = (double **) malloc(sizeof(double)*k);
	for(int i=0; i<k; i++)
		centroides[i] = (double *) malloc(sizeof(double)*c);

	int *centroids = (int *) malloc (sizeof(int)*k);
	bool flag = false;

	for(int i=0; i<k; i++){
		do{
			flag = false;
			centroids[i] = rand()%n;
			for(int j=0; j<i; j++){
				if(centroids[i] == centroids[j])
					flag=true;
			}
		}while(flag);
	}

	for(int i=0; i<k; i++)
		for(int j=0; j<c; j++)
			centroides[i][j] = data[centroids[i]].a[j];	
	
	double **sum = NULL;
	sum = (double **) malloc(sizeof(double)*k);
	int *count=NULL;
	
	count = (int *) malloc(sizeof(int)*k);
	double **dist=NULL;
	
	for(int i=0; i<k; i++)
		sum[i] = (double *) malloc(sizeof(double)*c);
	
	dist = (double **) malloc(sizeof(double)*n);
	
	for(int i=0; i<n; i++)
		dist[i] = (double *) malloc(sizeof(double)*c);
	
	for(int i=0; i<n; i++)
		for(int j=0; j<c; j++)
			dist[i][j]=0;

	int ite=0;
	
	double old;

	do{
		for(int i=0; i<n; i++)
			for(int j=0; j<c; j++)
				dist[i][j]=0;

		double temp;
		double d1;
		for(int i=0; i<n; i++){
			for(int j=0; j<k; j++)
				for (int z = 0; z < c; z++) {
					d1 = data[i].euclidianDistance3(data[i].a[z], centroides[j][z]);
					temp = groupWeight[data[0].groupInfo[z]] * featureWeight[z] * d1;
					dist[i][j] += temp;
				}
			
			double min = dist[i][0];
			int m = 0;

			for(int z=1; z<k; z++)
				if(dist[i][z] <= min){
					m=z;
					min = dist[i][z];
				}
				
				data[i].cCluster = m;		
		}
		
		for(int i=0; i<k; i++){
				count[i]=0;
			for(int j=0; j<c; j++)
				sum[i][j]=0;
		}

		for(int i=0; i<n; i++){
			for(int j=0; j<c; j++)
				sum[data[i].cCluster][j]+=data[i].a[j];
			
			count[data[i].cCluster]++;
		}

		for(int i=0; i<k; i++)
			for(int j=0; j<c; j++)
				centroides[i][j] = (double) sum[i][j]/count[i];

		updateFeatureWeights(&n, &c, &k, &eta, &p, data);
		updateViewWeights(&n, &c, &k, &lambda, &p, data);
		
		ite++;
		old = fobj;
		f(data, n, k, p, eta, lambda);
	}while(fobj!=old );

	for (int i = 0; i < n; i++)
		free(dist[i]);

	free(dist);

	for (int i = 0; i < k; i++)
		free(sum[i]);

	free(centroids);
	free(sum);
	free(count);
}

void KMeans::updateFeatureWeights( int *nr, int *nc, int *k, double *eta, int *numGroups, Data *data) {
	int i, j, t;

	for (j = 0; j < *nc; ++j) 
		featureWeight[j] = 0;
	
	for (j = 0; j < *nc; ++j) 
		for (i = 0; i < *nr; ++i)
			featureWeight[j] += groupWeight[data[0].groupInfo[j]]*data[i].euclidianDistance3(data[i].a[j],centroides[data[i].cCluster][j]);
	
	double *sum, *sum2, *max;
	sum = (double*)malloc(*numGroups * sizeof(double));
	sum2 = (double*)malloc(*numGroups * sizeof(double));
	max = (double*)malloc(*numGroups * sizeof(double));

	for (t = 0; t < *numGroups; t++) {
		sum[t] = 0;
		sum2[t] = 0;
		max[t] = -1.79769e+308;
	}

	//find maximum
	for (j = 0; j < *nc; ++j) {
		featureWeight[j] = -featureWeight[j] / (*eta);
		if (max[data[0].groupInfo[j]] < featureWeight[j]) {
			max[data[0].groupInfo[j]] = featureWeight[j];
		}
	}
	//compute exp()
	for (j = 0; j < *nc; ++j) {
		featureWeight[j] = exp(featureWeight[j] - max[data[0].groupInfo[j]]);
		sum[data[0].groupInfo[j]] += featureWeight[j];
	}
	//normalize
	double minWeight = 0.00001 / (*nc);
	for (j = 0; j < *nc; ++j) {
		featureWeight[j] /= sum[data[0].groupInfo[j]];
		if (featureWeight[j] < minWeight) {
			featureWeight[j] = minWeight;
		}
		sum2[data[0].groupInfo[j]] += featureWeight[j];
	}

	for (j = 0; j < *nc; ++j) {
		featureWeight[j] /= sum2[data[0].groupInfo[j]];
	}


	free(sum);
	free(sum2);
	free(max);
}

void KMeans::updateViewWeights(int *nr, int *nc, int *k, double *lambda, int *numGroups, Data *data) {
	int i, j, t;

	for (t = 0; t < *numGroups; ++t)
		groupWeight[t] = 0;

	for (i = 0; i < *nr; ++i)
		for (j = 0; j < *nc; ++j)
			groupWeight[data[0].groupInfo[j]] += featureWeight[j]* data[i].euclidianDistance3(centroides[data[i].cCluster][j],data[i].a[j]);

	for (t = 0; t < *numGroups; ++t)
		groupWeight[t] = -groupWeight[t] / (*lambda);

	double sum = 0.0, sum2 = 0.0, max = 0.0;

	// implement expNormalize()
	max = groupWeight[0]; // initially assign gw[0] to max
	for (t = 1; t < *numGroups; ++t) {
		if (groupWeight[t] >= max)
			max = groupWeight[t];
	}

	for (t = 0; t < *numGroups; ++t) {
		groupWeight[t] = exp(groupWeight[t] - max);
		sum += groupWeight[t];
	}

	double minWeight = 0.00001 / (*numGroups);
	for (t = 0; t < *numGroups; ++t) {
		groupWeight[t] /= sum;
		if (groupWeight[t] < minWeight) {
			groupWeight[t] = minWeight;
		}
		sum2 += groupWeight[t];
	}

	if (sum2 != 1) {
		for (t = 0; t < *numGroups; ++t) {
			groupWeight[t] /= sum2;
		}
	}

}

void KMeans::f(Data *data, int n, int k, int p, double eta, double lambda){

	double *obj = (double *)malloc(sizeof(double)*k);

	fobj=0;

	for(int i=0; i<k; i++){
		obj[i]=0;
	}
	
	for(int i=0; i<k; i++)
		for(int j=0; j<n; j++)
			if (data[j].cCluster == i)
				for(int l=0; l<data[0].nbAtt; l++)
					obj[i]+=groupWeight[data[0].groupInfo[l]]*featureWeight[l]*data[j].euclidianDistance3(data[j].a[l],centroides[i][l]);
	
	for(int i=0; i<k; i++)
		fobj += obj[i];
	

	double term2 = 0;
	double term3 = 0;

	for (int i = 0; i < p; i++)
		term3 += groupWeight[i] * log(groupWeight[i]);

	for (int i = 0; i < data[0].nbAtt; i++)
		term2 += featureWeight[i] * log(featureWeight[i]);


	fobj += (eta*term2 + lambda*term3);

}
