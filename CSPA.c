#include "ensemble.h"


int main(){
    int Nelements,Nclusters;
	scanf("%d%d",&Nelements,&Nclusters);
    int **ensemble = MALLOC(int*,Nclusters);
	for(int i=0;i<Nelements;i++)
		ensemble[i] = MALLOC(int,Nelements);
	
	for(int i=0;i<Nclusters;i++)
		for(int j=0;j<Nelements;j++)
			scanf("%d",&ensemble[i][j]);
    
    int Nvertices,Narestas;
    Nvertices = Nelements;
    Narestas = (Nvertices*Nvertices)/2;
    int peso[Nvertices][Nvertices];
    int NarestasZero = 0;
    for(int i=0;i<Nelements;i++){
        for(int j=0;j<Nelements;j++){
            peso[i][j] = 0;
            for(int k=0;k<Nclusters;k++)
                if(ensemble[k][i] == ensemble[k][j])
                    peso[i][j]++;
            if(i == j)
                peso[i][j] = 0;
            if(peso[i][j] == 0)
                NarestasZero++;
        }
    }
    Narestas -= NarestasZero/2;

    printf("%d %d 001\n",Nvertices,Narestas);
    for(int i=0;i<Nvertices;i++){
        for(int j=0;j<Nvertices;j++)
            if(i!=j && peso[i][j]!=0 )
                printf("%d %d ",j+1,peso[i][j]);
        
        printf("\n");
    }
}