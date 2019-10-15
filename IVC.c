#include "ensemble.h"

//limitações: Esse algoritmo só funciona bem quando todos os ensembles tem o mesmo número de agrupamentos, que é igual ao número de agrupamentos alvo.

unsigned int *normalize_clusters(unsigned int *cluster,unsigned int Nelements){
	unsigned int *taken = CALLOC(unsigned int,NCLUSTERSMAX);
	unsigned int *newCluster = MALLOC(unsigned int,Nelements);

	unsigned int Nclusters = 0;

	for(unsigned int i=0;i<Nelements;i++){
		if( taken[cluster[i]] == 0 ){
			newCluster[i] = taken[cluster[i]] = ++Nclusters;
		}
		else{
			newCluster[i] = taken[cluster[i]];
		}
	}

	return newCluster;
}

void Copy(unsigned int *clusterCenter,unsigned int *clusterPoint,int Nclusters){
    for(int i=0;i<Nclusters;i++)
        clusterCenter[i] = clusterPoint[i];
}

int Hamming(unsigned int *clusterCenter,unsigned int *clusterPoint,int Nclusters){
    int dist = 0;
    for(int i=0;i<Nclusters;i++)
        if(clusterCenter[i] != clusterPoint[i])
            dist++;
    return dist;
}

int Reassign(unsigned int **clusterCenters,unsigned int **clusterPoints,unsigned int *finalClustering,int Nelements,int Nclusters,int NclustersFinal){
    int hammingSum = 0;
    for(int i=0;i<Nelements;i++){
        int newCluster = 0;
        int minDist = Hamming(clusterCenters[0],clusterPoints[i],Nclusters);
        for(int j=1;j<NclustersFinal;j++){
            if(Hamming(clusterCenters[j],clusterPoints[i],Nclusters) < minDist){
                minDist = Hamming(clusterCenters[j],clusterPoints[i],Nclusters);
                newCluster = j;
            }
        }
        finalClustering[i] = newCluster;
        hammingSum += minDist;
    }
    return hammingSum;
}

void RecalculateCenters(unsigned int **clusterCenters,unsigned int **clusterPoints,unsigned int *finalClustering,int Nelements,int Nclusters,int NclustersFinal){
    int frequency[NclustersFinal][Nclusters][NCLUSTERSMAX];
    for(int i=0;i<NclustersFinal;i++)
        for(int j=0;j<Nclusters;j++)
            for(int k=0;k<NCLUSTERSMAX;k++)
                frequency[i][j][k]=0;
            
    for(int i=0;i<Nelements;i++){
        for(int j=0;j<Nclusters;j++){
            frequency[finalClustering[i]][j][clusterPoints[i][j]]++;
        }
    }
    for(int i=0;i<NclustersFinal;i++)
        for(int j=0;j<Nclusters;j++){
            int freq = 0;
            for(int k=0;k<NCLUSTERSMAX;k++)
                if(freq < frequency[i][j][k]){
                    freq = frequency[i][j][k];
                    clusterCenters[i][j] = k;
                }
        }       
}

unsigned int *IVC(unsigned int **ensemble,int Nclusters,int Nelements,int NclustersFinal){
    unsigned int **clusterCenters = MALLOC(unsigned int*,NclustersFinal);
    for(int i=0;i<NclustersFinal;i++)
        clusterCenters[i] = MALLOC(unsigned int,Nclusters);
    
    unsigned int **clusterPoints = MALLOC(unsigned int*,Nelements);
    for(int i=0;i<Nelements;i++)
        clusterPoints[i] = MALLOC(unsigned int,Nclusters);


    
    for(int i=0;i<Nelements;i++)
        for(int j=0;j<Nclusters;j++)
            clusterPoints[i][j] = ensemble[j][i];
    
    int initialCentroids[NclustersFinal];
    int Nselected = 0;
    int *selected = CALLOC(int,Nelements);
    while(Nselected < NclustersFinal){
        int candidate = rand()%Nelements;
        if(!selected[candidate]){
            selected[candidate] = 1;
            initialCentroids[Nselected++] = candidate;
        }
    }
    for(int i=0;i<NclustersFinal;i++)
        Copy(clusterCenters[i],clusterPoints[initialCentroids[i]],Nclusters);
    
    unsigned int *finalClustering = MALLOC(unsigned int,Nelements);

    int hammingSum = Reassign(clusterCenters,clusterPoints,finalClustering,Nelements,Nclusters,NclustersFinal);
    int previousHammingSum = hammingSum + 1;
    while(hammingSum < previousHammingSum){
        previousHammingSum = hammingSum;
        RecalculateCenters(clusterCenters,clusterPoints,finalClustering,Nelements,Nclusters,NclustersFinal);
        hammingSum = Reassign(clusterCenters,clusterPoints,finalClustering,Nelements,Nclusters,NclustersFinal);
    }
    return finalClustering;
}


int main(int agrc,char **argv){
    srand(time(NULL));
    int Nelements,Nclusters;
	scanf("%d%d",&Nelements,&Nclusters);
    unsigned int **ensemble = MALLOC(unsigned int*,Nclusters);
	for(int i=0;i<Nclusters;i++)
		ensemble[i] = MALLOC(unsigned int,Nelements);
	
	for(int i=0;i<Nclusters;i++)
		for(int j=0;j<Nelements;j++)
			scanf("%d",&ensemble[i][j]);

    for(int i=0;i<Nclusters;i++)
        ensemble[i] = normalize_clusters(ensemble[i],Nelements);

    
    int NclustersFinal = atoi(argv[1]);

    unsigned int *finalClustering = IVC(ensemble,Nclusters,Nelements,NclustersFinal);

    finalClustering = normalize_clusters(finalClustering,Nelements);

    char output[60];
	strcpy(output,argv[2]);
	strcat(output,"_AGGREGATION");

	FILE *fileOutput = fopen(output,"w");

    if(fileOutput){
        for(int i=0;i<Nelements;i++)
            fprintf(fileOutput,"%d ",finalClustering[i]);
        fprintf(fileOutput,"\n");
    }
    else{
        printf("nao foi possivel salver o resultado do IVC\n");
    }
    
    return 0;
}