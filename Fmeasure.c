#include "header.h"

#define Nelements 2310

int max(int a,int b){
	if(a>b)
		return a;
	return b;
}

double recall(int *clustersVerdadeiros,int *clustersAlgoritmo,int clusteri,int clusterj){
	unsigned int nij=0;
	unsigned int ni=0;

	for(unsigned int k=0; k<Nelements; k++)
		if(clustersVerdadeiros[k] == clusteri && clustersAlgoritmo[k] == clusterj)
			nij++;
	
	for(unsigned int k=0; k<Nelements; k++)
			if(clustersVerdadeiros[k]==clusteri)
				ni++;

	if(ni == 0 )
		return 0;

	return (double) nij/ni;
}

double precision(int *clustersVerdadeiros,int *clustersAlgoritmo,int clusteri,int clusterj){
	unsigned int nij=0;
	unsigned int nj=0;

	for(unsigned int k=0; k<Nelements; k++)
		if(clustersVerdadeiros[k] == clusteri && clustersAlgoritmo[k] == clusterj)
			nij++;
	
	for(unsigned int k=0; k<Nelements; k++)
			if(clustersAlgoritmo[k]==clusterj)
				nj++;

	if(nj == 0 )
		return 0;	

	return (double) nij/nj;
}

double Fmeasure(int *clustersVerdadeiros,int *clustersAlgoritmo,int clusteri,int clusterj){
	double p = precision(clustersVerdadeiros,clustersAlgoritmo,clusteri,clusterj);
	double r = recall(clustersVerdadeiros,clustersAlgoritmo,clusteri,clusterj);
	
	if(p==0 || r==0)
		return 0;

	return (double) 2*(p*r)/(p+r);
}

double FmeasureTotal(int *clustersVerdadeiros,int *clustersAlgoritmo,int NclustersVerdadeiros,int NclustersAlgoritmo){
	double f=0;
	
	unsigned int *count= (unsigned int *) CALLOC(unsigned int,NclustersVerdadeiros);
	double *value;

	for(unsigned int j=0; j<Nelements; j++){
		count[clustersVerdadeiros[j]]++;
	}

	for(unsigned int i=0; i<NclustersVerdadeiros; i++){    
		value = MALLOC(double,NclustersAlgoritmo);
		for(unsigned int j=0; j<NclustersAlgoritmo; j++)
			value[j] = (double) Fmeasure(clustersVerdadeiros, clustersAlgoritmo,i, j);
		
		double max = value[0];
		
		for(unsigned int j=1; j<NclustersAlgoritmo; j++)
			if(value[j] > max)
				max = value[j];

		f+= (double) count[i]*max;
		free(value);		
	}
        free(count);

	return (double) f/Nelements;
}

int main(){
	int clustersVerdadeiros[Nelements];
	int clustersAlgoritmo[Nelements];
	
	for(int i=0;i<Nelements;i++)
		scanf("%d",&clustersVerdadeiros[i]);
	for(int i=0;i<Nelements;i++)
		scanf("%d",&clustersAlgoritmo[i]);
		
	int NclustersVerdadeiros=clustersVerdadeiros[0],NclustersAlgoritmo=clustersAlgoritmo[0];
	for(int i=1;i<Nelements;i++){
		NclustersVerdadeiros = max(NclustersVerdadeiros,clustersVerdadeiros[i]);
		NclustersAlgoritmo = max(NclustersAlgoritmo,clustersAlgoritmo[i]);
	} 
	printf("%lf",FmeasureTotal(clustersVerdadeiros,clustersAlgoritmo,++NclustersVerdadeiros,++NclustersAlgoritmo));
	
}
