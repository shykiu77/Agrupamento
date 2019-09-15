#include "header.h"

#define Nelements 2310

double RandIndex(int *clustersVerdadeiros,int *clustersAlgoritmo){
	double a = 0.0, b = 0.0, c = 0.0, d = 0.0, p, cr;
	for(unsigned int i = 0; i < Nelements-1; i++)
		for(unsigned int j = (i+1); j < Nelements; j++){
			if((clustersAlgoritmo[i] == clustersAlgoritmo[j]) && (clustersVerdadeiros[i] == clustersVerdadeiros[j]))
		        	a += 1.0;
			if((clustersAlgoritmo[i] != clustersAlgoritmo[j]) && (clustersVerdadeiros[i] == clustersVerdadeiros[j]))
				b += 1.0;
			if((clustersAlgoritmo[i] == clustersAlgoritmo[j]) && (clustersVerdadeiros[i]!= clustersVerdadeiros[j]))
				c += 1.0;
			if((clustersAlgoritmo[i] != clustersAlgoritmo[j]) && (clustersVerdadeiros[i]!= clustersVerdadeiros[j]))
				d += 1.0;
		}	
    p = a + b + c + d;
    cr = (a + d) /p;
    return cr;
}

int main(){
	int clustersVerdadeiros[Nelements];
	int clustersAlgoritmo[Nelements];
	
	for(int i=0;i<Nelements;i++)
		scanf("%d",&clustersVerdadeiros[i]);
	for(int i=0;i<Nelements;i++)
		scanf("%d",&clustersAlgoritmo[i]);
		
	
	printf("%lf",RandIndex(clustersVerdadeiros,clustersAlgoritmo));
	
}
