#include "Utils.h"
#include "KMeans.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

Utils::Utils(){

}

Utils::~Utils(){

}

void Utils::run(double *medias, double *desvios, double **indices, unsigned int *sol, double *bfobj, unsigned int *bsol, double *bestbfobj,unsigned int nbRep, unsigned int nbObj, Data *data, unsigned int k, unsigned int p, double **matrizes, unsigned int **g, ofstream *file1, unsigned int *apriori, unsigned int *finalclustering, double eta, double lambda) {
	clock_t t1, t2;
    	string externos[] = { "OERC","ARI", "F-Measure","Jaccard","NMI","Purity","FOBJ"};    
    	double objf;
        
	for (unsigned int z = 0; z<nbRep; z++) {
		printf("\nRep: %u",z);
		KMeans s = KMeans(0,0);
		
		t1 = clock();
		s.run(data,nbObj,k,data[0].nbAtt,p,eta,lambda);
		t2 = clock();
		
		for (unsigned int j = 0; j<nbObj; j++)
			sol[j] = data[j].cCluster;

		printf(" obj: %f",  s.fobj);
        	objf = s.fobj;
                
		if (z == 0) {
				for (unsigned int j = 0; j<nbObj; j++)
					bsol[j] = data[j].cCluster;

				*bfobj = objf;

		}
		else
			    if((s.fobj < *bfobj)){
					for (unsigned int j = 0; j<nbObj; j++)
						bsol[j] = data[j].cCluster;

					*bfobj = s.fobj;
                }
                
		indices[0][z] = oerc(apriori, sol, nbObj, k);
		indices[1][z] = cr_index(apriori, sol, nbObj);
		indices[2][z] = fmeasuret(apriori, sol, nbObj, k, k);
        	indices[3][z] = jaccard_index(apriori, sol, nbObj);
		indices[4][z] = nmi2(apriori, sol, nbObj, k);
        	indices[5][z] = purity(apriori, sol, nbObj, k);
		indices[6][z] = objf;
		indices[7][z] = ((double)t2 - (double)t1) / CLOCKS_PER_SEC;
		indices[8][z] = indices[7][z] / 60;
	}

	/*for (unsigned int i = 0; i < 7; i++) {
		*file1 << "\n" << externos[i] << "\n";
		for (unsigned int j = 0; j < nbRep; j++)
			*file1 << indices[i][j] << "\n";
	}*/

	/*file1 << "\nBest representatives: " << "\n";
	for (unsigned int i = 0; i < k; i++) {
		for (unsigned int j = 0; j < q; j++)
			*file1 << g[i][j] << " ";
		*file1 << "\n";
	}*/

	

	
	for (unsigned int z = 0; z<9; z++) {
		medias[z] = 0;
		desvios[z] = 0;
		for (unsigned int i = 0; i<nbRep; i++)
			medias[z] += indices[z][i];

		medias[z] = medias[z] / nbRep;

		for (unsigned int i = 0; i<nbRep; i++)
			desvios[z] += pow(indices[z][i] - medias[z], 2);

		desvios[z] = desvios[z] / (nbRep - 1);
		desvios[z] = sqrt(desvios[z]);
	}

	for (unsigned int j = 0; j<nbObj; j++) 
		finalclustering[j] = bsol[j];

	//*file1 << "\nBest partition: " << "\n";
	for (unsigned int j = 0; j < nbObj; j++)
	  *file1 << finalclustering[j] << " ";
	*file1 << "\n";

	/**file1 << "\n\nConfusion matrix: " << "\n";
	unsigned int nij;
	for(int i=0; i<k; i++) {
			for(int j=0; j<k; j++) {
				nij=0;
				for(int t=0; t<nbObj; t++)
					if(apriori[t] == i && finalclustering[t] == j)
						nij++;
				*file1 << nij << " " ;
			}
		*file1 << "\n";
	}*/
}



Data* Utils::chooseDataset(unsigned int dataset, unsigned int *p, unsigned int *k, unsigned int *nbObj, bool normalized) {
	
	switch (dataset) {
		
		case 0: {
			*k = 7;
			*p = 2;
			*nbObj = 2310;
			return readImageSegmentation(normalized);
		}break;
		
	}
}



double Utils::purity(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k) {
	unsigned int nij = 0;
        double purity = 0;
        double max=0;


	for (unsigned int i = 0; i < k; i++) {
            max=0;
		for (unsigned int j = 0; j < k; j++){
                        nij=0;
			for (unsigned int z = 0; z < n; z++)
					if (apriori[z] == i && clustering[z] == j)
						nij++;
                        if(nij>max)
                            max = nij;
                }
		
                
            purity+=max;
	}

	purity = (double) purity / n;

	return purity;
}

double Utils::precision(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int i, unsigned int j){
        
        unsigned int nij=0;
	unsigned int nj=0;

	for(unsigned int k=0; k<n; k++)
		if(apriori[k] == i && clustering[k] == j)
			nij++;
	
	for(unsigned int k=0; k<n; k++)
			if(clustering[k]==j)
				nj++;

	if(nj == 0 )
		return 0;	

	return (double) nij/nj;
}	

double Utils::recall(unsigned int *apriori, unsigned int *clustering,unsigned int n, unsigned int i, unsigned int j){
    
	unsigned int nij=0;
	unsigned int ni=0;

	for(unsigned int k=0; k<n; k++)
		if(apriori[k] == i && clustering[k] == j)
			nij++;
	
	for(unsigned int k=0; k<n; k++)
			if(apriori[k]==i)
				ni++;

	if(ni == 0 )
		return 0;

	return (double) nij/ni;
}

double Utils::fmeasure(unsigned int *apriori, unsigned int *clustering,unsigned int n,unsigned int i, unsigned int j){
	
	double p=precision(apriori, clustering, n, i,j);
	double r=recall(apriori, clustering, n, i ,j);
	
	if(p==0 || r==0)
		return 0;

	return (double) 2*(p*r)/(p+r);
}

double Utils::fmeasuret(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int m, unsigned int k){

	double f=0;
	
	unsigned int *count= (unsigned int *) malloc(sizeof(unsigned int)*k);
	double *value = (double *) malloc (sizeof(double)*k);

	for(unsigned int i=0; i<m; i++){
                
		for(unsigned int j=0; j<k; j++)
			value[j] = (double) fmeasure(apriori, clustering, n, i, j);
		
		double max = value[0];
		
		for(unsigned int j=1; j<k; j++)
			if(value[j] > max)
				max = value[j];

		for(unsigned int j=0; j<k; j++)
			count[j]=0;
	
		for(unsigned int j=0; j<n; j++){
			count[apriori[j]]++;
		}
		
		f+= (double) count[i]*max;		
	}
        free(value);
        free(count);

	return (double) f/n;
}

double Utils::nmi(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k){

	double nmi=0;
	double d1=0;
	double d2=0;
	double e1 = 0;
	double e2 = 0;

	for(unsigned int i=0; i<k; i++){
		unsigned int count=0;
		for(unsigned int j=0; j<n; j++)
			if(clustering[j]==i)
				count++;
		
		d1+=((double) count/n)*log2((double )count/n);
		//e1 += (count/n) * d1[i];
	}

	for(unsigned int i=0; i<k; i++){
		unsigned int count=0;
		for(unsigned int j=0; j<n; j++)
			if(apriori[j]==i)
				count++;
		d2+=((double) count/n)*log2((double )count/n);
		//e2 += (count/n) * d1[i];
	}

	//printf("\n%f",e1);
	//printf("\n%f",e2);

	double value=0;

	for(unsigned int i=0; i<k; i++){

		unsigned int count=0;
		for(unsigned int l=0; l<n; l++)
			if(apriori[l]==i)
				count++;

		for(unsigned int j=0; j<k; j++){
			unsigned int nij=0;
			for(unsigned int l=0; l<n; l++)
				if(clustering[l] == i && apriori[l]==j)
					nij++;

			unsigned int count2=0;
			for(unsigned int l=0; l<n; l++)
				if(clustering[l]==j)
					count2++;

			double temp = n* nij;
			//printf("\ntemp:%f",temp);
			double temp2 = (double) count*count2;
			//printf("\ntemp2:%f", temp2);
			double temp3;
			if(temp2==0)
				temp3=0;
			else
				temp3 = log2((double)temp/temp2);
			//printf("\ntemp3: %f", temp3);
			if(temp3>0)
				value+=((double) nij/n)*temp3;
			//printf("\nvalue:%f", value);
		}
	}

	return (double) -(value)/((d1+d2)/2);
}		

double Utils::cr_index(unsigned int *apriori, unsigned int *clustering, unsigned int n){
	double a = 0.0, b = 0.0, c = 0.0, d = 0.0, p, cr;
	for(unsigned int i = 0; i < n-1; i++)
        for(unsigned int j = (i+1); j < n; j++){
			if ((clustering[i] == clustering[j]) && (apriori[i] == apriori[j]))
				a += 1.0;
			if ((clustering[i] != clustering[j]) && (apriori[i] == apriori[j]))
				b += 1.0;
			if ((clustering[i] == clustering[j]) && (apriori[i] != apriori[j]))
				c += 1.0;
			if ((clustering[i] != clustering[j]) && (apriori[i] != apriori[j]))
				d += 1.0;
        }
    p = a + b + c + d;
    cr = ((a + d) - ((a + b) * (a + c) + (d + b) * (d + c)) * (1 / p)) / (p - ((a + b) * (a + c) + (d + b) * (d + c)) * (1 / p));
    return cr;
}

double Utils::rand_index(unsigned int *apriori, unsigned int *clustering, unsigned int n){
	double a = 0.0, b = 0.0, c = 0.0, d = 0.0, p, cr;
	for(unsigned int i = 0; i < n-1; i++)
        for(unsigned int j = (i+1); j < n; j++){
			if ((clustering[i] == clustering[j]) && (apriori[i] == apriori[j]))
				a += 1.0;
			if ((clustering[i] != clustering[j]) && (apriori[i] == apriori[j]))
				b += 1.0;
			if ((clustering[i] == clustering[j]) && (apriori[i] != apriori[j]))
				c += 1.0;
			if ((clustering[i] != clustering[j]) && (apriori[i] != apriori[j]))
				d += 1.0;
        }
    p = a + b + c + d;
    cr = (a + d) /p;
    return cr;
}

double Utils::jaccard_index(unsigned int *apriori, unsigned int *clustering, unsigned int n){
	double a = 0.0, b = 0.0, c = 0.0, d = 0.0, p, j;
	 for(unsigned int i = 0; i < n-1; i++)
        for(unsigned int j = (i+1); j < n; j++){
			if((clustering[i] == clustering[j]) && (apriori[i] == apriori[j]))
                a += 1.0;
            if((clustering[i] != clustering[j]) && (apriori[i] == apriori[j]))
                b += 1.0;
            if((clustering[i] == clustering[j]) && (apriori[i]!= apriori[j]))
                c += 1.0;
            if((clustering[i] != clustering[j]) && (apriori[i]!= apriori[j]))
                d += 1.0;
        }	
    p = a + b + c;
    j = a  /p;
    return j;
}

double Utils::oerc(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k) {
	unsigned int *nij = (unsigned int *)malloc(sizeof(unsigned int)*k);
	unsigned int sum = 0;

	for (unsigned int i = 0; i<k; i++)
		nij[i] = 0;

	for (unsigned int i = 0; i<k; i++) {
		for (unsigned int j = 0; j<k; j++) {
			nij[j] = 0;
			for (unsigned int l = 0; l<n; l++)
				if (clustering[l] == i && apriori[l] == j)
					nij[j]++;
		}

		unsigned int max = nij[0];

		for (unsigned int i = 1; i<k; i++)
			if (nij[i]>max)
				max = nij[i];

		sum += max;
	}

	free(nij);

	return 1 - ((double)sum / n);
}



double Utils::entropyC_index(unsigned int *clustering, unsigned int n, unsigned int k){

	double *entropy = (double *) malloc(sizeof(double)*k);
	double e = 0;
	for(unsigned int j=0; j<k; j++)
		entropy[j] = 0;

	for(unsigned int i=0; i<k; i++){
		double ni=0;
		for(unsigned int l=0; l<n; l++)
			if(clustering[l]==i)
				ni++;

		entropy[i]= ((double) ni / n) * log2((double) ni/n);
		e += entropy[i];
	}

        free(entropy);
	return  -e;
}	

double Utils::entropyP_index(unsigned int *apriori, unsigned int n, unsigned int k){

	double *entropy = (double *) malloc(sizeof(double)*k);
	double e = 0;
	for(unsigned int j=0; j<k; j++)
		entropy[j] = 0;

	for(unsigned int i=0; i<k; i++){
		double ni=0;
		for(unsigned int l=0; l<n; l++)
			if(apriori[l] ==i)
				ni++;

		entropy[i]= ((double) ni / n) * log2((double) ni/n);
		e += entropy[i];
	}

           free(entropy);
	//printf("\n%f",entropy);
	return  -e;
}	

double Utils::mutual_information_index(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k){
	double value=0;

	for(unsigned int i=0; i<k; i++){

		unsigned int count=0;
		for(unsigned int l=0; l<n; l++)
			if(apriori[l]==i)
				count++;

		for(unsigned int j=0; j<k; j++){
			unsigned int nij=0;
			for(unsigned int l=0; l<n; l++)
				if(apriori[l] == i && clustering[l]==j)
					nij++;

			unsigned int count2=0;
			for(unsigned int l=0; l<n; l++)
				if(clustering[l]==j)
					count2++;

			double temp = n*nij;
			double temp2 = (double) count*count2;
			double temp3;

			if(temp2==0 || temp==0)
				temp3=0;
			else
				temp3 = log2((double)temp/temp2);
			
			//if(temp3>0)
			value+=((double) nij/n)*temp3;
		}
	}

	return value;

}	

double Utils::nmi2(unsigned int *apriori, unsigned int *clustering, unsigned int n, unsigned int k){
	return 2*mutual_information_index(apriori, clustering, n,k)/(entropyC_index(clustering,n,k)+entropyP_index(apriori, n,k));
}

Data* Utils::readImageSegmentation(bool normalized){
	ifstream file("prog/segmentation.data");
	
	string content;
	unsigned int cont=0;
	unsigned int i=0;
	unsigned int cluster = -1;
	Data *rd=NULL;
	rd = (Data *) malloc(sizeof(Data)*2310);

	for(unsigned int i=0 ; i<2310; i++){
		rd[i].nbAtt = 19;
		rd[i].a = (double *) malloc(sizeof(double)*19);
		if (i == 0) {
			rd[0].groupInfo = (int *)malloc(sizeof(int) * 19);

			for (int j = 0; j < 9; j++)
				rd[0].groupInfo[j] = 0;
			for (int j = 9; j<19; j++)
				rd[0].groupInfo[j] = 1;

		}
	}
	
	while(file >> content) {
		if(cont==0){
			rd[i].cluster = stod(content);
			cont++;
		}
		else{
			rd[i].a[cont-1] = stod(content);
			cont++;
		}
		
		if(cont==20){
			cont=0;
			i++;
		}
	}

	file.close();

	if (normalized) {
		double *max = (double *)malloc(sizeof(double) * 19);
		double *min = (double *)malloc(sizeof(double) * 19);

		for (int i = 0; i<19; i++) {
			max[i] = rd[0].a[i];
			min[i] = rd[0].a[i];
		}

		for (int i = 1; i<2310; i++)
			for (int j = 0; j<19; j++) {
				if (rd[i].a[j] < min[j])
					min[j] = rd[i].a[j];
				if (rd[i].a[j] > max[j])
					max[j] = rd[i].a[j];
			}

		for (int i = 0; i<2310; i++)
			for (int j = 0; j<19; j++)
				rd[i].a[j] = (rd[i].a[j] - min[j]) / (max[j] - min[j]);

		free(max);
		free(min);
	}

	return rd;
}


double Utils::euclideanDistance(Data rd1, Data rd2){

	double dist = 0.0;
	
	for(unsigned int i=0; i<rd1.nbAtt; i++)
		dist += pow(rd1.a[i] - rd2.a[i], 2);

	dist = sqrt(dist);
	return dist;
}

double Utils::euclideanDistance(Data rd1, Data rd2, unsigned int p){

	double dist = 0.0;

	//for(unsigned int i=0; i<rd1.nbAtt; i++)
	dist += pow(rd1.a[p] - rd2.a[p], 2);

	dist = sqrt(dist);

	return dist;
}

