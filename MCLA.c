#include "ensemble.h"
#include <sys/types.h> 




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


int *normalize_meta_clusters(int *cluster,int Nelements){
	int *taken = CALLOC(int,NCLUSTERSMAX+1);
	int *newCluster = MALLOC(int,Nelements+1);

	int Nclusters = 0;

	for(int i=1;i<=Nelements;i++){
		if( taken[cluster[i]] == 0 ){
			newCluster[i] = taken[cluster[i]] = ++Nclusters;
		}
		else{
			newCluster[i] = taken[cluster[i]];
		}
	}

	return newCluster;
}


int main(int argc,char **argv){
    clock_t start = clock();
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
    Queue **metaVertices = MALLOC(Queue*,NCLUSTERSMAX);
    
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

    int NmetaArestas = (NmetaVertices*NmetaVertices)/2;
    int NmetaArestasZero = 0;
    for(int i=1;i<=NmetaVertices;i++)
        for(int j=1;j<=NmetaVertices;j++)
            if((int)(distancias[i][j]*NmetaVertices) == 0 || i == j)
                NmetaArestasZero++;
            
    NmetaArestas -= NmetaArestasZero/2;

    char fileOutput[60];
    strcpy(fileOutput,argv[2]);
    strcat(fileOutput,"_MCLA.tempFile");  
    FILE *pontFile = fopen(fileOutput,"w");
    if(pontFile){
        fprintf(pontFile,"%d %d 001\n",NmetaVertices,NmetaArestas);
        for(int i=1;i<=NmetaVertices;i++){
            for(int j=1;j<=NmetaVertices;j++)
                if(i!=j && (int)(distancias[i][j]*NmetaVertices)!=0 )
                    fprintf(pontFile,"%d %d ",j,(int)(distancias[i][j]*NmetaVertices));
            
            fprintf(pontFile,"\n");
        }
        fclose(pontFile);
    }
    else{
        printf("error opening file\n");
        return 1;
    }
   
    if(fork() == 0){
        execl("/usr/bin/gpmetis","gpmetis","-ptype=rb",fileOutput,argv[1],NULL);
    }
    else{
        wait(NULL);
        char fileInput[50];
        strcpy(fileInput,fileOutput);
        strcat(fileInput,".part.");
        strcat(fileInput,argv[1]);
        pontFile = fopen(fileInput,"r");
        int cont=0;
        char line[3];
        char METISresult[NmetaVertices+2];
        if(pontFile){
            while(fgets(line,sizeof(line),pontFile)){
                METISresult[++cont] = line[0];
            }
            fclose(pontFile);
            int *metaClusters = MALLOC(int,NmetaVertices+1);
            for(int i=1;i<=NmetaVertices;i++)   
                metaClusters[i] = METISresult[i] - '0';



            metaClusters = normalize_meta_clusters(metaClusters,NmetaVertices);
            
        
            int NmetaClusters = 0;
            for(int i=1;i<=NmetaVertices;i++)
                NmetaClusters = MAX(NmetaClusters,metaClusters[i]);
            


            double metaHyperEdge[NmetaClusters+1][Nelementos];
            for(int i=1;i<=NmetaClusters;i++)
                for(int j=0;j<Nelementos;j++)
                    metaHyperEdge[i][j] = 0;

           
            


            for(int i=1;i<=NmetaVertices;i++){
                Node *ite = metaVertices[i]->head;
                while(ite!=NULL){
                    metaHyperEdge[metaClusters[i]][ite->vertice]++;
                    ite = ite->proximo;
                }
            }            
            
            int finalClustering[Nelementos];
            for(int i=0;i<Nelementos;i++){
                int cluster = 0;
                int associacao = 0;
                for(int j=1;j<=NmetaClusters;j++){
                    if(metaHyperEdge[j][i] > associacao){
                        associacao = metaHyperEdge[j][i];
                        cluster = j;
                    }
                }
                finalClustering[i] = cluster;
            }
            clock_t end = clock();
            char programOutput[40];
            strcpy(programOutput,argv[2]);
            strcat(programOutput,"_MCLA");
            pontFile = fopen(programOutput,"w");


            for(int i=0;i<Nelementos;i++)
                fprintf(pontFile,"%d ",finalClustering[i]);
            fprintf(pontFile,"\nTempo usado: %lf",(double)(end-start)/CLOCKS_PER_SEC);

            fclose(pontFile);

           
        }
        else{
            return 1;
        }
    }

    
    return 0;
}