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
    
    
	
    return 0;
}