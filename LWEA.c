#include "ensemble.h"

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
    int Ncluster = 0;
    for(int i=0;i<Nclusters;i++){
        int NhyperEdges = 0;
        for(int j=0;j<Nelementos;j++){
            NhyperEdges = MAX(NhyperEdges,ensemble[i][j]);
        }
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

     for(int i=1;i<=Ncluster;i++)   
        printf("%.2lf\n",ECI[i]);

    return 0;
}