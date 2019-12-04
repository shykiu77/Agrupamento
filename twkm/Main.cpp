#include <iostream>	
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "KMeans.h"
#include "Utils.h"

using namespace std;

double **matrizes;

void writeNormalizedMatrixInFile(int p, int n) {
	
		string fileName = "fawa.txt";
		ofstream file1(fileName);

		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				file1 << matrizes[j][k] << " ";
			}
		}

		file1.close();
	
}


void initMatrizes(unsigned int n){
    
	matrizes = (double **) malloc (sizeof(double)*n);
	
	for(unsigned int j=0; j<n; j++)
		matrizes[j] = (double *) malloc (sizeof(double)*n);
	
	for(unsigned int j=0; j<n; j++)
		for(unsigned int k=0; k<n; k++)
			matrizes[j][k] = (double) 0.0;
}

void writeResultsInFile(string alg,ofstream *file1, unsigned int nbObj, unsigned int k, double bestbfobj, double *medias, double *desvios, double **lambda,unsigned int p, unsigned int lim, unsigned int **g,unsigned int q,unsigned int n, unsigned int *apriori, unsigned int *finalclustering, double eta, double lambda2) {
	Utils utils = Utils();
	*file1 << "\n\n";
	*file1 << "ETA: " << eta << "	Lambda: " << lambda2 << "\n";
	*file1 << "\nOERC: " << utils.oerc(apriori, finalclustering, nbObj, k) << "	" << medias[0] << "	" << desvios[0] << "\n";
	*file1 << "ARI: " << utils.cr_index(apriori, finalclustering, nbObj) << "	" << medias[1] << "	" << desvios[1] << "\n";
	*file1 << "F-Measure: " << utils.fmeasuret(apriori, finalclustering, nbObj, k, k) << "	" << medias[2] << "	" << desvios[2] << "\n";
	*file1 << "Jaccard: " << utils.jaccard_index(apriori, finalclustering, nbObj) << "	" << medias[3] << "	" << desvios[3] << "\n";
	*file1 << "NMI: " << utils.nmi2(apriori, finalclustering, nbObj, k) << "	" << medias[4] << "	" << desvios[4] << "\n";
	*file1 << "Purity: " << utils.purity(apriori, finalclustering, nbObj,k ) << "	" << medias[5] << "	" << desvios[5] << "\n";
    *file1 << "Best obj: " << bestbfobj << "	" << medias[6] << "	" << desvios[6] << "\n";
	*file1 << "Tempo seg: " << medias[7] << "	" << desvios[7] << "\n";
	*file1 << "Tempo min: " << medias[8] << "	" << desvios[8] << "\n";
	
	if (g != NULL) {
		*file1 << "\n\nBest representatives: ";
		for (unsigned int i = 0; i < k; i++) {
			*file1 << "\nCluster: " << i << "\n";
			for (unsigned int j = 0; j < q; j++)
				*file1 << g[i][j] << " ";
		}	
	}

	/*
	for (unsigned int i = 0; i < k; i++) {
		*file1 << "\nCluster: " << i << "\n";

		for (unsigned int j = 0; j < n; j++)
			if (finalclustering[j] == i)
				*file1 << j << " ";

		*file1 << "\n";
	}
	
	*file1 << "\n\n";*/
}

void computeMeansAndDeviation(double *medias, double *desvios, unsigned int nbRep, double **indices) {

	for (unsigned int z = 0; z < 9; z++) {
		medias[z] = 0;
		desvios[z] = 0;
		for (unsigned int i = 0; i < nbRep; i++)
			medias[z] += indices[z][i];

		medias[z] = medias[z] / nbRep;

		for (unsigned int i = 0; i < nbRep; i++)
			desvios[z] += pow(indices[z][i] - medias[z], 2);

		desvios[z] = desvios[z] / (nbRep - 1);
		desvios[z] = sqrt(desvios[z]);
	}
}

int main(int argc, char *argv[]) {

	unsigned int nbRep = atoi(argv[1]);
	unsigned int base = atoi(argv[2]);

	//unsigned int nbRep = 50;
	//unsigned int base = 4;
	string algs[] = { "TWKM"};
	Utils utils = Utils();
	Data *data = NULL;	
	srand(time(NULL));
	unsigned int nbObj = 0;
	unsigned int k = 3,p=1;
	double **indices = (double **)malloc(sizeof(double) * 9);
	bool normalized = false;
	string dataSets[] = {"Image", "Mfeat" , "Water","Phoneme","Internet","3sources","Flowers","AWA1","AWA2" ,"SubCorel1", "SubCorel2","FAWA","Wine","Breast","Thyroid","Australian","Pima","Bupa","Mammography","Heart","Abalone","Satellite"};
	string externos[] = { "OERC","ARI", "F-Measure","Jaccard","NMI","Purity","FOBJ" };
	double eta[] = {40,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	double lambda[] = {70,40,60,80,100};
	int i = 4;
	int j = 5;

	//eta[j] = 7;
	//lambda[i] = 20;

	double *desvios = (double *)malloc(sizeof(double) * 9);
	double *medias = (double *)malloc(sizeof(double) * 9);
	double bfobj, bestbfobj;

	for (unsigned int i = 0; i < 9; i++)
		indices[i] = (double *)malloc(sizeof(double)*nbRep);
	
	data = utils.chooseDataset(base, &p, &k, &nbObj, normalized);
	unsigned int *sol = (unsigned int *)malloc(sizeof(unsigned int)*nbObj);
	unsigned int *bsol = (unsigned int *)malloc(sizeof(unsigned int)*nbObj);
	unsigned int *apriori = (unsigned int *)malloc(sizeof(unsigned int)*nbObj);
	unsigned int *clustering = (unsigned int *)malloc(sizeof(unsigned int)*nbObj);
	
	for (unsigned int i = 0; i < nbObj; i++)
		apriori[i] = data[i].cluster;

	string name = "_" + algs[0] + "_" + to_string(nbRep) + "_" + to_string(eta[j]) + "_" + to_string(lambda[i]);
	string fileName1 = dataSets[base] + name + ".data";
	ofstream file1(fileName1);


	for(int i=0; i<5; i++){
		for (int j = 0; j < 6; j++) {
			printf("\nEta: %f, lambda: %f", eta[j], lambda[i]);
			utils.run(medias, desvios, indices, sol, &bfobj, bsol, &bestbfobj, nbRep, nbObj, data, k, p, matrizes, NULL, &file1, apriori, clustering,eta[j],lambda[i]);
			//writeResultsInFile("TWKM", &file1, nbObj, k, bfobj, medias, desvios, NULL, p, k, NULL, 0, nbObj, apriori, clustering,eta[j],lambda[i]);
		}
	}

	file1.close();
	return 1;
}
