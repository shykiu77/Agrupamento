#include "ensemble.h"

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

double distancia(int Natributos,double *p1,double *p2){
    double dist = 0;
    for(int i=0;i<Natributos;i++){
        dist += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sqrt(dist);
}



double CenterDistance(unsigned int Nelements, unsigned int Natributos,double **dataset, unsigned int *clustering, unsigned int NclustersClustering){
    double **centroids = MALLOC(double*,NclustersClustering+1);
    for(int i=1;i<=NclustersClustering;i++)
        centroids[i] = CALLOC(double,Natributos);
     for(int i=1;i<=NclustersClustering;i++)
        for(int j=0;j<Natributos;j++)
            centroids[i][j] = 0;
    
    int *NelementsOnCluster = CALLOC(int,NclustersClustering+1);
    for(int i=0;i<Nelements;i++){
        for(int j=0;j<Natributos;j++)
            centroids[clustering[i]][j] += dataset[i][j];
        NelementsOnCluster[clustering[i]]++;
    }
    for(int i=1;i<=NclustersClustering;i++){
        for(int j=0;j<Natributos;j++){
            centroids[i][j] /= NelementsOnCluster[i];
        }
    }
    double dist[NclustersClustering+1];
    for(int i=1;i<=NclustersClustering;i++)
        dist[i]=0;
    
    for(int i=0;i<Nelements;i++){
        dist[clustering[i]] += distancia(Natributos,centroids[clustering[i]],dataset[i]);
    }
    double retorno = 0;
    for(int i=1;i<=NclustersClustering;i++)
        retorno += dist[i];
    return retorno;

}


double Sillouet(unsigned int Nelements, unsigned int Natributos,double **dataset, unsigned int *clustering, unsigned int NclustersClustering){
    unsigned int *NelementsOnCluster = CALLOC(unsigned int,NclustersClustering +1);
    for(int i=0;i<Nelements;i++)
        NelementsOnCluster[clustering[i]]++;
    
    double *a = CALLOC(double,Nelements);
    for(int i=0;i<Nelements;i++){
        for(int j=0;j<Nelements;j++){
            if(clustering[i] == clustering[j] && i != j){
                a[i] += distancia(Natributos,dataset[i],dataset[j]);
            }
        }
    }

    for(int i=0;i<Nelements;i++){
        if(NelementsOnCluster[clustering[i]] - 1 != 0)
            a[i] /= NelementsOnCluster[clustering[i]] - 1;
        else
            a[i] = 0;
        
    }
    
    double *b = CALLOC(double,Nelements);

    for(int i=0;i<Nelements;i++){
        double *potentialB = CALLOC(double,NclustersClustering+1);
        for(int j=0;j<Nelements;j++){
            if(clustering[i] != clustering[j]){
                potentialB[clustering[j]] += distancia(Natributos,dataset[i],dataset[j]);
            }
        }
        
        b[i] = INFINITY;
        for(int j=1; j<=NclustersClustering;j++){
            if(j != clustering[i])
                b[i] = MIN(b[i], potentialB[j]/NelementsOnCluster[j]);
        }    
    }
    
    double *s = CALLOC(double,Nelements);

    for(int i=0;i<Nelements;i++){
        if(NelementsOnCluster[clustering[i]] > 1){
            s[i] = ((b[i] - a[i])/(MAX(a[i],b[i])));
        }
        else{
            s[i] = 0;
        }
    }
    double soma = 0;
    for(int i=0;i<Nelements;i++)
        soma += s[i];

    return soma/Nelements;
}

//Argumentos: número de elementos, numero de atributos, dataset e clustering.
int main(int argc, char **argv){
    if(argc != 5){
        printf("Número de argumentos incorreto.\n");
        return 1;
    }
    FILE *file1,*file2;
    file1 = fopen(argv[3],"r");
    file2 = fopen(argv[4],"r");
    if(!(file1 && file2)){
        printf("Não foi possível abrir um dos arquivos\n");
        return 1;
    }
    unsigned int Nelements = atoi(argv[1]);
    unsigned int Natributos = atoi(argv[2]);
    
	unsigned int *clustering = MALLOC(unsigned int,Nelements);
    double **dataset = MALLOC(double*,Nelements);
    for(int i=0;i<Nelements;i++)
        dataset[i] = MALLOC(double,Natributos);


    for(unsigned int i=0;i<Nelements;i++){
        fscanf(file2,"%d",&clustering[i]);
        for(unsigned int j=0;j<Natributos;j++)
            fscanf(file1,"%lf",&dataset[i][j]);
    }

    fclose(file1);
    fclose(file2);

	clustering = normalize_clusters(clustering,Nelements);

	

	
    unsigned int NclustersClustering=0;
    for(unsigned int i=0;i<Nelements;i++)
        NclustersClustering = MAX(NclustersClustering,clustering[i]);
    
    double s = Sillouet( Nelements, Natributos,dataset, clustering, NclustersClustering);
    double d = CenterDistance(Nelements, Natributos,dataset, clustering, NclustersClustering);
	printf("Sillouet: %lf\n",s);
    printf("Distancias ao centro: %lf\n",d);
    return 0;
}