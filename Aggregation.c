#include "ensemble.h"

typedef struct Cluster{
	int elemento;
	struct Cluster *proximo;
	struct Cluster *ultimo;
}Cluster;

Cluster **CreateSingletonClusters(int Nelements){
	Cluster **singletonClusters = MALLOC(Cluster*,Nelements);
	for(int i=0;i<Nelements;i++){
		singletonClusters[i] = MALLOC(Cluster,1);
		singletonClusters[i]->elemento = i;
		singletonClusters[i]->proximo = NULL;	
		singletonClusters[i]->ultimo = singletonClusters[i];			
	}
	return singletonClusters;
}

void mergeClusters(Cluster **cluster,int c1,int c2){
	int inserido;
	int removido;
	Cluster *iterar;
	if(cluster[c1]!=NULL){
		inserido = c1;
		removido = c2;
		iterar = cluster[c2];
	}
	else {
		if(cluster[c2]!=NULL){
			inserido = c2;
			removido = c1;
			iterar = cluster[c1];
		}
		else
			return;
	}
	while(iterar != NULL){
		cluster[inserido]->ultimo->proximo = iterar;
		cluster[inserido]->ultimo = iterar;
		iterar = iterar->proximo;
	}
	cluster[removido]=NULL;
}

int ContagemDeElementos(Cluster *cluster){
	int cont = 0;
	Cluster *iterate = cluster;
	while(iterate != NULL){
		cont++;
		iterate = iterate->proximo;
	}
	return cont;
}

double AvgDistance(Cluster *cluster1,Cluster *cluster2,double **distancias){
	if(cluster1 == NULL || cluster2 == NULL)
		return 2;
		
	double distancia = 0;
	Cluster *iterateC1 = cluster1;
	while(iterateC1 != NULL){
		Cluster *iterateC2 = cluster2;
		while(iterateC2 != NULL){
			distancia += distancias[iterateC1->elemento][iterateC2->elemento];
			iterateC2 = iterateC2->proximo;
		}
		iterateC1 = iterateC1->proximo;
	}
	distancia =distancia /  (ContagemDeElementos(cluster1) * ContagemDeElementos(cluster2));
	return distancia;
}

//algoritmo super ineficiente, melhorar essa merda.
double GetMinAverageWeight(Cluster **cluster,double **pesos,int *c1,int *c2,int Nelements){
	double minDist = 2;	
	for(int i=0;i<Nelements;i++)
		for(int j=0;j<Nelements;j++)
			if(i!=j && cluster[i]!=NULL && cluster[j]!=NULL)
				if(minDist > pesos[i][j]){
					minDist = pesos[i][j];
					*c1 = i;
					*c2 = j;
				}	
	return minDist;
}


void AtualizarPesos(Cluster **clusters,double **pesos,double **distancias,int c1,int c2,int Nelements){
	Cluster *novo=clusters[c1];
	int indiceNovo = c1;
	int indiceRemovido = c2;
	if(novo == NULL){
		novo = clusters[c2];
		indiceNovo = c2;
		indiceRemovido = c1;
	}
	for(int i=0;i<Nelements;i++)
		if(i != indiceNovo)
			if(clusters[i]!=NULL){
				double avgDist = AvgDistance(novo,clusters[i],distancias);
				pesos[indiceNovo][i]=avgDist;
				pesos[i][indiceNovo]=avgDist;
				pesos[i][indiceRemovido]=2;
				pesos[indiceRemovido][i]=2;
			}
	
}

int *AgglomerativeClustering(Cluster **clusters,double **distancias,int Nelements){
	int ite=0;
	int c1,c2;
	double **pesos=MALLOC(double*,Nelements);
	for(int i=0;i<Nelements;i++){
		pesos[i]=MALLOC(double,Nelements);
		for(int j=0;j<Nelements;j++)
			pesos[i][j]=distancias[i][j];
	}
	double minDist = GetMinAverageWeight(clusters,pesos,&c1,&c2,Nelements);
	while(minDist < 0.5){
		ite++;
		mergeClusters(clusters, c1, c2);
		AtualizarPesos(clusters,pesos,distancias,c1,c2,Nelements);
		minDist = GetMinAverageWeight(clusters,pesos,&c1,&c2,Nelements);
	}
	int *finalClustering = MALLOC(int,Nelements);
	int cont=0;
	for(int i=0;i<Nelements;i++){
		if(clusters[i]!=NULL){
			Cluster *iterate = clusters[i];
			while(iterate!=NULL){
				finalClustering[iterate->elemento] = cont;
				iterate = iterate->proximo; 
			}
			cont++;
		}
	}
	return finalClustering;
}


void CalcularDistancias(int **ensemble,double **distancias,int Nelements,int Nclusters){	
	for(int i=0;i<Nelements;i++)
		for(int j=0;j<Nelements;j++){
			double diferencas = 0;
			for(int k=0;k<Nclusters;k++){
				if(ensemble[k][i]!=ensemble[k][j])
					diferencas = diferencas + 1;
			}
			distancias[i][j] = diferencas / (double)(Nclusters);
		}
}	


int main(int argc, char **argv){
	int Nelements,Nclusters;
	scanf("%d%d",&Nelements,&Nclusters);
	int **ensemble = MALLOC(int*,Nclusters);
	for(int i=0;i<Nelements;i++)
		ensemble[i] = MALLOC(int,Nelements);
	
	for(int i=0;i<Nclusters;i++)
		for(int j=0;j<Nelements;j++)
			scanf("%d",&ensemble[i][j]);
		
	
	double ** distancias = MALLOC(double*,Nelements);
	for(int i=0;i<Nelements;i++)
		distancias[i] = MALLOC(double,Nelements);
		
	CalcularDistancias(ensemble,distancias,Nelements,Nclusters);		
	
		
	Cluster **clusterings = CreateSingletonClusters(Nelements);
	
	int *results = AgglomerativeClustering(clusterings,distancias,Nelements);

	char output[200];
	strcpy(output,argv[2]);
	strcat(output,"_AGGREGATION");
	FILE *filePont = fopen(output,"w");
	if(filePont){
		for(int i=0;i<Nelements;i++)
			fprintf(filePont,"%d ",results[i]);
		fprintf(filePont,"\n");
		fclose(filePont);
	}
	else{
		printf("Não foi possível criar o arquivo de saída.\n");
	}
	return 0;
}










