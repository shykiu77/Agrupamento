#include "ensemble.h"

#define NHYPEREDGESMAX 1000
#define UBfactor "5"

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

int main(int argc,char **argv){
    int Nelements,Nclusters;
	scanf("%d%d",&Nelements,&Nclusters);
    int **ensemble = MALLOC(int*,Nclusters);
	for(int i=0;i<Nclusters;i++)
		ensemble[i] = MALLOC(int,Nelements);
	
    

	for(int i=0;i<Nclusters;i++)
		for(int j=0;j<Nelements;j++)
			scanf("%d",&ensemble[i][j]);
    
    for(int i=0;i<Nclusters;i++){
        int ok=1;
        for(int j=0;j<Nelements;j++)
            if(ensemble[i][j] == 0)
                ok = 0;
        if(!ok){
            for(int j=0;j<Nelements;j++)
                ensemble[i][j]++;
        }
    }
    Queue **saida = MALLOC(Queue*,NHYPEREDGESMAX);
    
    int NhyperEdgesTotal = 0;
    for(int i=0;i<Nclusters;i++){
        int NhyperEdges = 0;
        for(int j=0;j<Nelements;j++){
            NhyperEdges = MAX(NhyperEdges,ensemble[i][j]);
        }
        for(int j=1; j<=NhyperEdges;j++){
            saida[NhyperEdgesTotal+j] = MakeQueue();
            for(int k=0;k<Nelements;k++)
                if(ensemble[i][k] == j)
                    saida[NhyperEdgesTotal+j] = InsertNode(k+1,saida[NhyperEdgesTotal+j]);
        }
        NhyperEdgesTotal += NhyperEdges;
    }

    for(int i=1;i<=NhyperEdgesTotal;i++){
        if(saida[i]->head == NULL)
            NhyperEdgesTotal--;
    }
   
    
    

    char datasetName[20];
    scanf("%s",datasetName);
    char fileOutput[40];

    //TODO: generalizar saidas.
    strcpy(fileOutput,"Saidas/Wine/HGPA.");
    strcat(fileOutput,datasetName);

    FILE *pontFile = fopen(fileOutput,"w");
    if(pontFile){
        fprintf(pontFile,"%d %d\n",NhyperEdgesTotal,Nelements);
        for(int i=1;i<=NhyperEdgesTotal;i++)
        if(saida[i]->head != NULL)
            PrintQueue(saida[i],pontFile);
        fclose(pontFile);
    }
    else{
        printf("error opening file\n");
        return 1;
    }

    
    printf("Se você esta vendo essa mensagem então provavelmente não tem o shmetis instalado\n");
    execl("Bin/shmetis","shmetis",fileOutput,argv[1],UBfactor,NULL);
    
    return 0;
}