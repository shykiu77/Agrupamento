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
    
    for(int i=0;i<Nelementos;i++)
        for(int j=0;j<Nelementos;j++)
            distancias[i][j] *= 100;

    int Narestas = Nelementos*Nelementos/2;
    int NarestasZero = 0;

    for(int i=0;i<Nelementos;i++){
        for(int j=0;j<Nelementos;j++){
            if(i == j )
                distancias[i][j] = 0;
            if((int) (distancias[i][j]) == 0)
                NarestasZero++;
        }
    }
    Narestas -= NarestasZero/2;

    char fileOutput[40];

    strcpy(fileOutput,argv[2]);
    strcat(fileOutput,"_LWEA+CSPA");
    FILE *pontFile = fopen(fileOutput,"w");
    if(pontFile){
        fprintf(pontFile,"%d %d 001\n",Nelementos,Narestas);
        for(int i=0;i<Nelementos;i++){
            for(int j=0;j<Nelementos;j++)
                if(i!=j && (int) (distancias[i][j])!=0 )
                    fprintf(pontFile,"%d %d ",j+1,(int)(distancias[i][j]));
            
            fprintf(pontFile,"\n");
        }
        fclose(pontFile);
    }
    else{
        printf("error opening file\n");
        return 1;
    }

    
    printf("Se você esta vendo essa mensagem então provavelmente não tem o metis instalado\n");
    execl("/usr/bin/gpmetis","gpmetis","-ptype=rb",fileOutput,argv[1],NULL);
    
	
	

    return 0;
}