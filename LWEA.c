#include "ensemble.h"


//Código das estruturas.
typedef struct Node{
    struct Node *proximo;
    int vertice;
}Node;

typedef struct Queue{
    struct Node *head;
    struct Node *tail;
}Queue;

Queue *MakeQueue(){
    Queue *queue = MALLOC(Queue,1);
    queue->head = queue->tail = NULL;
    return queue;
}

Node *MakeNode(int vertice){
    Node *node = MALLOC(Node,1);
    node->proximo = NULL;
    node->vertice = vertice;
    return node;
}

Queue *InsertNode(int vertice, Queue *queue){
    if(queue->head == NULL){
        queue->head = MakeNode(vertice);
        queue->tail = queue->head;
        return queue;
    }
    queue->tail->proximo = MakeNode(vertice);
    queue->tail = queue->tail->proximo;
    return queue;
}

void PrintQueue(Queue *queue,FILE *pontFile){
    Node *ite = queue->head;
    while(ite != NULL){
        fprintf(pontFile,"%d ",ite->vertice);
        ite = ite->proximo;
    }
    fprintf(pontFile,"\n");
}


//Código do LWEA

int *normalize_clusters(int *cluster,int Nelements){
	int *taken = CALLOC(int,NCLUSTERSMAX);
	int *newCluster = MALLOC(int,Nelements);

	int Nclusters = 0;

	for(int i=0;i<Nelements;i++){
		if( taken[cluster[i]] == 0 ){
			newCluster[i] = taken[cluster[i]] = ++Nclusters;
		}
		else{
			newCluster[i] = taken[cluster[i]];
		}
	}

	return newCluster;
}

double p(Queue **cluster,int clusterI,int clusterJ,int Nelementos){
    int NclusterI=0,NclusterIntersec=0;
    int *intersecta = CALLOC(int,Nelementos);

    Node *ite = cluster[clusterI]->head;
    while(ite != NULL){
        NclusterI++;
        intersecta[ite->vertice] = 1;
        ite = ite->proximo;
    }
    ite = cluster[clusterJ]->head;
    while(ite != NULL){
        if(intersecta[ite->vertice] == 1)
            NclusterIntersec++;
        ite = ite->proximo;
    }
    return (double)((double)(NclusterIntersec)/(double)(NclusterI));
}

double uncertainty(Queue **cluster,int clusterAlvo,int Ncluster,int Nelementos){
    double HclusterAlvo=0;
    for(int i=1;i<=Ncluster;i++){
        double pValue = p(cluster,clusterAlvo,i,Nelementos);  
        if(pValue != 0)
            HclusterAlvo += pValue*log2(pValue);
    }
    return -HclusterAlvo;
}


//Código do aggregation
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

//algoritmo super ineficiente, melhorar essa coisa.
double GetMaxAverageWeight(Cluster **cluster,double **pesos,int *c1,int *c2,int Nelements){
	double maxDist = 0;	
	for(int i=0;i<Nelements;i++)
		for(int j=0;j<Nelements;j++)
			if(i!=j && cluster[i]!=NULL && cluster[j]!=NULL)
				if(maxDist < pesos[i][j]){
					maxDist = pesos[i][j];
					*c1 = i;
					*c2 = j;
				}	
	return maxDist;
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

int *AgglomerativeClustering(Cluster **clusters,double **distancias,int Nelements,int NclustersFinal){
	int ite=0;
	int c1,c2;
	double **pesos=MALLOC(double*,Nelements);
	for(int i=0;i<Nelements;i++){
		pesos[i]=MALLOC(double,Nelements);
		for(int j=0;j<Nelements;j++)
			pesos[i][j]=distancias[i][j];
	}
	GetMaxAverageWeight(clusters,pesos,&c1,&c2,Nelements);
	while(Nelements-ite > NclustersFinal){
		ite++;
		mergeClusters(clusters, c1, c2);
		AtualizarPesos(clusters,pesos,distancias,c1,c2,Nelements);
		GetMaxAverageWeight(clusters,pesos,&c1,&c2,Nelements);
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

void CalcularDistancias(int **ensemble,double **distancias,int Nelements,int Nclusters, int *NclusteringsOnCluster,double *ECI,int Ncluster){	
	for(int i=0;i<Nelements;i++)
		for(int j=0;j<Nelements;j++){
			double diferencas = 0;
            int clusterAtual = 0;
			for(int k=0;k<Nclusters;k++){
				if(ensemble[k][i]==ensemble[k][j])
					diferencas = diferencas + ECI[clusterAtual + ensemble[k][i]];
                clusterAtual += NclusteringsOnCluster[k];
			}
			distancias[i][j] = diferencas / (double)(Nclusters);
		}
}	

int main(int argc,char **argv){
    int Nelementos,Nclusters;
	scanf("%d%d",&Nelementos,&Nclusters);
    int **ensemble = MALLOC(int*,Nclusters);
	for(int i=0;i<Nclusters;i++)
		ensemble[i] = MALLOC(int,Nelementos);
	
    

	for(int i=0;i<Nclusters;i++)
		for(int j=0;j<Nelementos;j++)
			scanf("%d",&ensemble[i][j]);
    
    for(int i=0;i<Nclusters;i++)
        ensemble[i] = normalize_clusters(ensemble[i],Nelementos);
    

    Queue **cluster = MALLOC(Queue*,NCLUSTERSMAX);
    int NclusteringsOnCluster[NCLUSTERSMAX];
    int Ncluster = 0;
    for(int i=0;i<Nclusters;i++){
        int NhyperEdges = 0;
        for(int j=0;j<Nelementos;j++){
            NhyperEdges = MAX(NhyperEdges,ensemble[i][j]);
        }
        NclusteringsOnCluster[i] = NhyperEdges;
        for(int j=1; j<=NhyperEdges;j++){
            cluster[Ncluster+j] = MakeQueue();
            for(int k=0;k<Nelementos;k++)
                if(ensemble[i][k] == j)
                    cluster[Ncluster+j] = InsertNode(k,cluster[Ncluster+j]);
        }
        Ncluster += NhyperEdges;
    }

    //Ncluster: numero de agrupamentos no ensemble(conjuntos).
    //Nclusters: numero de agrupamentos gerados por algoritmos.

    double ECI[Ncluster+1];
    double tetha=0.556832765;


    for(int i=1;i<=Ncluster;i++)
       ECI[i] = exp(-uncertainty(cluster,i,Ncluster,Nelementos)/(tetha*Ncluster));

    
    double ** distancias = MALLOC(double*,Nelementos);
	for(int i=0;i<Nelementos;i++)
		distancias[i] = MALLOC(double,Nelementos);
		
	CalcularDistancias(ensemble,distancias,Nelementos,Nclusters,NclusteringsOnCluster,ECI,Ncluster);
    Cluster **clusterings = CreateSingletonClusters(Nelementos);
    int *results = AgglomerativeClustering(clusterings,distancias,Nelementos,atoi(argv[1]));
    
    results = normalize_clusters(results,Nelementos);

	char output[60];
	strcpy(output,argv[2]);
	strcat(output,"_LWEA");

	FILE *fileOutput = fopen(output,"w");
	if(fileOutput){
		for(int i=0;i<Nelementos;i++)
			fprintf(fileOutput,"%d ",results[i]);
		fprintf(fileOutput,"\n");
	}
    else{
		printf("não foi possível salvar o restulado.\n");	
	}
	

    return 0;
}