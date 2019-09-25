#include "ensemble.h"

#define NHYPEREDGESMAX 1000


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


double Similaridade(Queue *conjunto1,Queue *conjunto2,int NelementosTotais){
    int *repetidas = CALLOC(int,NelementosTotais);
    int NelementosIntersecao = 0;
    int NelementosUniao = 0;

    Node *ite = conjunto1->head;
    while(ite!=NULL){
        if(repetidas[ite->vertice] == 0){
            repetidas[ite->vertice] = 1;
            NelementosUniao++;
        }
        else{
            NelementosIntersecao++;
        }
        ite = ite->proximo;
    }
    ite = conjunto2->head;
    while(ite!=NULL){
        if(repetidas[ite->vertice] == 0){
            repetidas[ite->vertice] = 1;
            NelementosUniao++;
        }
        else{
            NelementosIntersecao++;
        }
        ite = ite->proximo;
    }
    return ((double)(NelementosIntersecao)) / ((double)(NelementosUniao));
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
    
    for(int i=0;i<Nclusters;i++){
        int ok=1;
        for(int j=0;j<Nelementos;j++)
            if(ensemble[i][j] == 0)
                ok = 0;
        if(!ok){
            for(int j=0;j<Nelementos;j++)
                ensemble[i][j]++;
        }
    }
    Queue **metaVertices = MALLOC(Queue*,NHYPEREDGESMAX);
    
    int NmetaVertices = 0;
    for(int i=0;i<Nclusters;i++){
        int NhyperEdges = 0;
        for(int j=0;j<Nelementos;j++){
            NhyperEdges = MAX(NhyperEdges,ensemble[i][j]);
        }
        for(int j=1; j<=NhyperEdges;j++){
            metaVertices[NmetaVertices+j] = MakeQueue();
            for(int k=0;k<Nelementos;k++)
                if(ensemble[i][k] == j)
                    metaVertices[NmetaVertices+j] = InsertNode(k,metaVertices[NmetaVertices+j]);
        }
        NmetaVertices += NhyperEdges;
    }
   
    
    double distancias[NmetaVertices+1][NmetaVertices+1];

    for(int i=1;i<=NmetaVertices;i++){
        for(int j=1;j<=NmetaVertices;j++){
            distancias[i][j] = Similaridade(metaVertices[i],metaVertices[j],Nelementos);
        }
    }

    printf("hyperedges: \n");
    for(int i=1;i<=NmetaVertices;i++){
        PrintQueue(metaVertices[i],stdout);
    }
    printf("\nsemelhanças: \n");
    for(int i=1;i<=NmetaVertices;i++){
        for(int j=1;j<=NmetaVertices;j++)
            printf("%lf ",distancias[i][j]);
        printf("\n");
    }
    return 0;
}